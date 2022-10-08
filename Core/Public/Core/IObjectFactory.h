/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_OBJECT_FACTORY_H
#define CORE_I_OBJECT_FACTORY_H 1

#include "CorePrerequisites.h"
#include "Result.h"
#include "Concurrency.h"
#include "IObject.h"

namespace greaper
{
	class IObjectFactory;

	class ObjectHandle
	{
		IObjectFactory* m_Factory;
		Uuid m_ObjectUUID;

	public:
		constexpr explicit ObjectHandle(IObjectFactory* factory = nullptr, Uuid objectID = IObject::InvalidUUID) noexcept;

		ObjectHandle(const ObjectHandle& other) noexcept;
		ObjectHandle& operator=(const ObjectHandle& other) noexcept;

		ObjectHandle(ObjectHandle&& other) noexcept;
		ObjectHandle& operator=(ObjectHandle&& other) noexcept;

		~ObjectHandle() noexcept;

		INLINE constexpr Uuid GetObjectUUID()const noexcept { return m_ObjectUUID; }

		INLINE constexpr IObjectFactory* GetFactory()const noexcept { return m_Factory; }

		IObject* GetObject()const noexcept;

		INLINE constexpr bool IsValid()const noexcept { return m_Factory != nullptr && m_ObjectUUID != IObject::InvalidUUID; }

		INLINE constexpr explicit operator bool()const noexcept { return IsValid(); }
	};

	class IObjectFactory
	{
	public:
		virtual ~IObjectFactory() noexcept = default;

		virtual void ClearFactory() = 0;

		virtual sizet GetObjectCount()const = 0;

		virtual TResult<ObjectHandle> GetHandle(Uuid objectUUID) = 0;

		virtual void OnHandleDestroyed(const ObjectHandle& handle) = 0;

		virtual TResult<Uuid> CreateObject() = 0;

		virtual EmptyResult DestroyObject(const ObjectHandle& handle) = 0;

		virtual IObject* GetObject(const ObjectHandle& objectID)const = 0;
	};

	template<class TObject>
	class TObjectHandle final : public ObjectHandle
	{
	public:
		INLINE TObject* operator->() noexcept
		{
			void* obj = GetObject();

			VerifyNotNull(obj, "Trying to access a nullptr ObjectHandle.");

			return static_cast<TObject*>(obj);
		}

		INLINE const TObject* operator->()const noexcept
		{
			void* obj = GetObject();

			VerifyNotNull(obj, "Trying to access a nullptr ObjectHandle.");

			return static_cast<TObject*>(obj);
		}

		INLINE TObject& operator*() noexcept
		{
			void* obj = GetObject();

			VerifyNotNull(obj, "Trying to access a nullptr ObjectHandle.");

			return *static_cast<TObject*>(obj);
		}

		INLINE const TObject& operator*()const noexcept
		{
			void* obj = GetObject();

			VerifyNotNull(obj, "Trying to access a nullptr ObjectHandle.");

			return *static_cast<TObject*>(obj);
		}
	};

	template<class TObject>
	class TObjectFactory final : public IObjectFactory
	{
		struct ObjectInfo
		{
			TObject* Obj;
			int64 ReferenceCount;
		};
		Vector<ObjectInfo> m_ObjectList;
		UnorderedMap<Uuid, sizet> m_UUID2IDX;
		Vector<sizet> m_FreeIndicesList;
		mutable Mutex m_Mutex;

	public:
		TObjectFactory() = default;
		explicit TObjectFactory(sizet capacity) noexcept
		{
			LOCK(m_Mutex);
			m_ObjectList.reserve(capacity);
			m_UUID2IDX.reserve(capacity);
			m_FreeIndicesList.reserve(capacity);
		}
		TObjectFactory(const TObjectFactory&) = delete;
		TObjectFactory& operator=(const TObjectFactory&) = delete;
		
		INLINE ~TObjectFactory() noexcept override
		{
			ClearFactory();
		}

		INLINE void ClearFactory()override
		{
			LOCK(m_Mutex);
			for (auto& info : m_ObjectList)
			{
				if (info.Obj == nullptr)
					continue;
				Destroy(info.Obj);
			}
			m_ObjectList.clear();
			m_FreeIndicesList.clear();
			m_UUID2IDX.clear();
		}

		INLINE sizet GetObjectCount()const override
		{
			LOCK(m_Mutex);
			return m_ObjectList.size() - m_FreeIndicesList.size();
		}

		INLINE TResult<ObjectHandle> GetHandle(Uuid objectUUID)override
		{
			if (objectUUID == IObject::InvalidUUID)
				return Result::CreateFailure<ObjectHandle>("Trying to obtain a handle from an Invalid ObjectUUID."sv);

			LOCK(m_Mutex);

			const auto findIT = m_UUID2IDX.find(objectUUID);
			if(findIT == m_UUID2IDX.end())
				return Result::CreateFailure<ObjectHandle>("Trying to obtain a handle but the object is not in this Factory."sv);

			const auto idx = findIT->second;
			if (m_ObjectList.size() <= idx)
				return Result::CreateFailure<ObjectHandle>("Trying to obtain a handle but the object is no longer in the Factory."sv);

			ObjectInfo& info = m_ObjectList[idx];

			if (info.Obj == nullptr)
				return Result::CreateFailure<ObjectHandle>("Trying to obtain a handle but the given ObjectUUID points to a null Object."sv);
			
			++info.ReferenceCount;

			return Result::CreateSuccess(ObjectHandle(this, objectUUID));
		}

		INLINE void OnHandleDestroyed(const ObjectHandle& handle)override
		{
			if (!handle.IsValid())
				return;

			if (handle.GetFactory() != this)
				return; // different factory!!

			LOCK(m_Mutex);

			const auto findIT = m_UUID2IDX.find(handle.GetObjectUUID());
			if (findIT == m_UUID2IDX.end())
				return; // not in the factory

			const auto idx = findIT->second;
			if (m_ObjectList.size() <= idx)
				return; // outside of bounds;

			ObjectInfo& info = m_ObjectList[idx];

			if (info.Obj == nullptr)
				return; // Object already destroyed

			--info.ReferenceCount;
			
			// No more references -> destroy object
			if (info.ReferenceCount <= 0)
			{
				Destroy(info.Obj);
				info.Obj = nullptr;
				m_UUID2IDX.erase(findIT);
                m_FreeIndicesList.push_back(idx); // Add the index to the free list
			}
		}

		INLINE TResult<Uuid> CreateObject()override
		{
			auto createFn = [this](ObjectInfo& info, Uuid objectUUID)
			{
				info.Obj = Construct<TObject>();
				info.ReferenceCount = 0;
				auto obj = reinterpret_cast<IObject*>(info.Obj);
				obj->m_UUID = objectUUID;
			};
			LOCK(m_Mutex);
			auto objectUUID = Uuid::GenerateRandom();
			if (m_ObjectList.size() == m_ObjectList.max_size())
			{
				if (m_FreeIndicesList.empty())
					return Result::CreateFailure<Uuid>("Trying to create a new Object but the Factory is at its limits!"sv);

				auto freeIndex = m_FreeIndicesList.front();
				m_FreeIndicesList.erase(m_FreeIndicesList.begin());

				ObjectInfo& info = m_ObjectList[freeIndex];
				createFn(info, objectUUID);
				m_UUID2IDX.insert_or_assign(objectUUID, freeIndex);
			}
			else
			{
				if (m_ObjectList.size() == m_ObjectList.capacity())
				{
					sizet nCapacity = Min(m_ObjectList.size() * 3 / 2, m_ObjectList.max_size());
					m_ObjectList.reserve(nCapacity);
				}

				const auto idx = m_ObjectList.size();
				m_ObjectList.push_back(ObjectInfo{});
				ObjectInfo& info = m_ObjectList[idx];
				
				createFn(info, objectUUID);
				m_UUID2IDX.insert_or_assign(objectUUID, idx);
			}
			return Result::CreateSuccess(objectUUID);
		}

		INLINE IObject* GetObject(const ObjectHandle& handle)const override
		{
			if (handle.GetObjectUUID() == IObject::InvalidUUID)
				return nullptr;

			LOCK(m_Mutex);
			const auto findIT = m_UUID2IDX.find(handle.GetObjectUUID());
			if (findIT == m_UUID2IDX.end())
				return nullptr;

			const auto idx = findIT->second;
			if (m_ObjectList.size() <= idx)
				return nullptr;
			
			const ObjectInfo& info = m_ObjectList[idx];

			return info.Obj;
		}
	};
	INLINE constexpr ObjectHandle::ObjectHandle(IObjectFactory* factory, Uuid objectID) noexcept
		:m_Factory(factory)
		, m_ObjectUUID(objectID)
	{

	}

	INLINE ObjectHandle::ObjectHandle(const ObjectHandle& other) noexcept
	{
		if (other.IsValid())
		{
			auto res = other.GetFactory()->GetHandle(other.GetObjectUUID()); // Adds a reference
			if (res.IsOk())
			{
				*this = std::move(res.GetValue());
				return;
			}
		}
		m_Factory = nullptr;
		m_ObjectUUID = IObject::InvalidUUID;
	}

	INLINE ObjectHandle& ObjectHandle::operator=(const ObjectHandle& other) noexcept
	{
		if (this != &other)
		{
			if (IsValid())
				m_Factory->OnHandleDestroyed(*this);

			if (other.IsValid())
			{
				auto res = other.GetFactory()->GetHandle(other.GetObjectUUID()); // Adds a reference
				if (res.IsOk())
				{
					*this = std::move(res.GetValue());
					return *this;
				}
			}
			m_Factory = nullptr;
			m_ObjectUUID = IObject::InvalidUUID;
		}
		return *this;
	}

	INLINE ObjectHandle::ObjectHandle(ObjectHandle&& other) noexcept
	{
		m_Factory = other.m_Factory;
		m_ObjectUUID = other.m_ObjectUUID;

		other.m_Factory = nullptr;
		other.m_ObjectUUID = IObject::InvalidUUID;
	}

	INLINE ObjectHandle& ObjectHandle::operator=(ObjectHandle&& other) noexcept
	{
		if (this != &other)
		{
			if (IsValid())
				m_Factory->OnHandleDestroyed(*this);

			m_Factory = other.m_Factory;
			m_ObjectUUID = other.m_ObjectUUID;

			other.m_Factory = nullptr;
			other.m_ObjectUUID = IObject::InvalidUUID;
		}
		return *this;
	}

	INLINE ObjectHandle::~ObjectHandle() noexcept
	{
		if (IsValid())
			m_Factory->OnHandleDestroyed(*this); // Removes a reference
	}

	INLINE IObject* ObjectHandle::GetObject()const noexcept
	{
		if (!IsValid())
			return nullptr;

		IObject* object = m_Factory->GetObject(*this);

		return object;
	}

	INLINE constexpr bool operator==(const ObjectHandle& left, const ObjectHandle& right) noexcept
	{
		return left.GetFactory() == right.GetFactory() && left.GetObjectUUID() == right.GetObjectUUID();
	}

	INLINE constexpr bool operator!=(const ObjectHandle& left, const ObjectHandle& right) noexcept
	{
		return !(left == right);
	}
}

#endif /* CORE_I_OBJECT_FACTORY_H */