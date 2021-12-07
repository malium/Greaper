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

namespace greaper
{
	class IObjectFactory;
	using ObjectID_t = uint64;
	inline constexpr ObjectID_t InvalidObjectID = std::numeric_limits<ObjectID_t>::max();

	class ObjectHandle
	{
		IObjectFactory* m_Factory;
		ObjectID_t m_ObjectID;

	public:
		constexpr ObjectHandle(IObjectFactory* factory = nullptr, ObjectID_t objectID = InvalidObjectID) noexcept;

		ObjectHandle(const ObjectHandle& other) noexcept;
		ObjectHandle& operator=(const ObjectHandle& other) noexcept;

		ObjectHandle(ObjectHandle&& other) noexcept;
		ObjectHandle& operator=(ObjectHandle&& other) noexcept;

		~ObjectHandle() noexcept;

		INLINE constexpr ObjectID_t GetObjectID()const noexcept { return m_ObjectID; }

		INLINE constexpr IObjectFactory* GetFactory()const noexcept { return m_Factory; }

		void* GetObject()const noexcept;

		INLINE constexpr bool IsValid()const noexcept { return m_Factory != nullptr && m_ObjectID != InvalidObjectID; }

		INLINE constexpr explicit operator bool()const noexcept { return IsValid(); }
	};

	class IObjectFactory
	{
	public:
		virtual ~IObjectFactory() noexcept = default;

		virtual void ClearFactory() = 0;

		virtual sizet GetObjectCount()const = 0;

		virtual Result<ObjectHandle> GetHandle(ObjectID_t objectID) = 0;

		virtual void OnHandleDestroyed(const ObjectHandle& handle) = 0;

		virtual Result<ObjectID_t> CreateObject() = 0;

		virtual EmptyResult DestroyObject(const ObjectHandle& handle) = 0;

		virtual void* GetObject(ObjectID_t objectID)const = 0;
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
			int64 RefereceCount;
		};
		Vector<ObjectInfo> m_ObjectList;
		Vector<ObjectID_t> m_FreeList;
		mutable Mutex m_Mutex;

	public:
		TObjectFactory() = default;
		TObjectFactory(sizet capacity) noexcept
		{
			LOCK(m_Mutex);
			m_ObjectList.reserve(capacity);
		}
		TObjectFactory(const TObjectFactory&) = delete;
		TObjectFactory& operator=(const TObjectFactory&) = delete;
		
		INLINE ~TObjectFactory() noexcept
		{
			ClearFactory();
		}

		INLINE void ClearFactory()override
		{
			for (auto& info : m_ObjectList)
			{
				if (info.Obj == nullptr)
					continue;
				Destroy(info.Obj);
			}
			m_ObjectList.clear();
			m_FreeList.clear();
		}

		INLINE sizet GetObjectCount()const override
		{
			LOCK(m_Mutex);
			return m_ObjectList.size() - m_FreeList.size();
		}

		INLINE Result<ObjectHandle> GetHandle(ObjectID_t objectID)override
		{
			if (objectID == InvalidObjectID)
				return CreateFailure<ObjectHandle>("Trying to obtain a handle from an InvalidObjectID."sv);

			LOCK(m_Mutex);

			if (m_ObjectList.size() <= objectID)
				return CreateFailure<ObjectHandle>("Trying to obtain a handle but the objectID is outside of bounds."sv);

			ObjectInfo& info = m_ObjectList[objectID];

			if (info.Obj == nullptr)
				return CreateFailure<ObjectHandle>("Trying to obtain a handle but the objectID points to a null Object."sv);
			
			++info.RefereceCount;

			return CreateResult(ObjectHandle(this, objectID));
		}

		INLINE void OnHandleDestroyed(const ObjectHandle& handle)override
		{
			if (!handle.IsValid())
				return;

			if (handle.GetFactory() != this)
				return; // different factory!!

			LOCK(m_Mutex);

			if (m_ObjectList.size() <= handle.GetObjectID())
				return; // outside of bounds

			ObjectInfo& info = m_ObjectList[handle.GetObjectID()];

			if (info.Obj == nullptr)
				return; // Object already destroyed

			--info.RefereceCount;
			
			// No more references -> destroy object
			if (info.RefereceCount <= 0)
			{
				Destroy(info.Obj);
				info.Obj = nullptr;
				m_FreeList.push_back(handle.GetObjectID()); // Add the index to the free list
			}
		}

		INLINE Result<ObjectID_t> CreateObject()override
		{
			auto createFn = [this](ObjectInfo& info)
			{
				info.Obj = Construct<TObject>();
				info.RefereceCount = 0;
			};
			LOCK(m_Mutex);
			ObjectID_t objectID = InvalidObjectID;
			if (m_ObjectList.size() != m_ObjectList.max_size())
			{
				if (m_FreeList.empty())
					return CreateFailure<ObjectID_t>("Trying to create a new Object but the Factory is at its limits!"sv);

				ObjectID_t& freeIndex = m_FreeList.front();
				m_FreeList.erase(m_FreeList.begin());

				ObjectInfo& info = m_ObjectList[freeIndex];
				createFn(info);
				objectID = freeIndex;
			}
			else
			{
				if (m_ObjectList.size() == m_ObjectList.capacity())
				{
					sizet nCapacity = Min(m_ObjectList.size() * 3 / 2, m_ObjectList.max_size());
					m_ObjectList.reserve(nCapacity);
				}

				objectID = m_ObjectList.size();
				m_ObjectList.push_back(ObjectInfo{});
				ObjectInfo& info = m_ObjectList[objectID];
				createFn(info);
			}
			return CreateResult(objectID);
		}

		INLINE void* GetObject(ObjectID_t objectID)const override
		{
			if (objectID == InvalidObjectID)
				return nullptr;

			LOCK(m_Mutex);
			if (m_ObjectList.size() <= objectID)
				return nullptr;

			const ObjectInfo& info = m_ObjectList[objectID];

			return info.Obj;
		}
	};

	INLINE constexpr ObjectHandle::ObjectHandle(IObjectFactory* factory, ObjectID_t objectID) noexcept
		:m_Factory(factory)
		, m_ObjectID(objectID)
	{

	}

	INLINE ObjectHandle::ObjectHandle(const ObjectHandle& other) noexcept
	{
		if (other.IsValid())
		{
			auto res = other.GetFactory()->GetHandle(other.GetObjectID()); // Adds a reference
			if (res.IsOk())
			{
				*this = std::move(res.GetValue());
				return;
			}
		}
		m_Factory = nullptr;
		m_ObjectID = InvalidObjectID;
	}

	INLINE ObjectHandle& ObjectHandle::operator=(const ObjectHandle& other) noexcept
	{
		if (this != &other)
		{
			if (IsValid())
				m_Factory->OnHandleDestroyed(*this);

			if (other.IsValid())
			{
				auto res = other.GetFactory()->GetHandle(other.GetObjectID()); // Adds a reference
				if (res.IsOk())
				{
					*this = std::move(res.GetValue());
					return;
				}
			}
			m_Factory = nullptr;
			m_ObjectID = InvalidObjectID;
		}
		return *this;
	}

	INLINE ObjectHandle::ObjectHandle(ObjectHandle&& other) noexcept
	{
		m_Factory = other.m_Factory;
		m_ObjectID = other.m_ObjectID;

		other.m_Factory = nullptr;
		other.m_ObjectID = InvalidObjectID;
	}

	INLINE ObjectHandle& ObjectHandle::operator=(ObjectHandle&& other) noexcept
	{
		if (this != &other)
		{
			if (IsValid())
				m_Factory->OnHandleDestroyed(*this);

			m_Factory = other.m_Factory;
			m_ObjectID = other.m_ObjectID;

			other.m_Factory = nullptr;
			other.m_ObjectID = InvalidObjectID;
		}
		return *this;
	}

	INLINE ObjectHandle::~ObjectHandle() noexcept
	{
		if (IsValid())
			m_Factory->OnHandleDestroyed(*this); // Removes a reference
	}

	INLINE void* ObjectHandle::GetObject()const noexcept
	{
		if (!IsValid())
			return nullptr;

		void* object = m_Factory->GetObject(m_ObjectID);

		return object;
	}

	INLINE constexpr bool operator==(const ObjectHandle& left, const ObjectHandle& right) noexcept
	{
		return left.GetFactory() == right.GetFactory() && left.GetObjectID() == right.GetObjectID();
	}

	INLINE constexpr bool operator!=(const ObjectHandle& left, const ObjectHandle& right) noexcept
	{
		return !(left == right);
	}
}

#endif /* CORE_I_OBJECT_FACTORY_H */