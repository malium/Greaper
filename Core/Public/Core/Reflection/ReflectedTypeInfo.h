/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_REFLECTED_TYPE_INFO_H
#define CORE_REFLECTION_REFLECTED_TYPE_INFO_H 1

#include "../CorePrerequisites.h"
#include "ReflectedField.h"
#include "../Memory.h"
#include "../Result.h"

namespace greaper
{
	class IReflectedType;

	class IReflectedTypeInfo
	{
		Vector<IReflectedField*> m_Fields;

	protected:
		void AddNewField(IReflectedField* field);

	public:
		IReflectedTypeInfo() = default;

		virtual ~IReflectedTypeInfo()noexcept;

		virtual Vector<IReflectedTypeInfo*>& GetDerivedClasses()const = 0;

		virtual IReflectedTypeInfo* GetBaseType()const = 0;

		virtual bool IsDerivedFrom(IReflectedTypeInfo* base)const = 0;

		virtual SPtr<IReflectedType> NewRelfectedObject()const = 0;

		virtual const String& GetReflectedName()const = 0;

		virtual ReflectedTypeID_t GetReflectedTypeID()const = 0;

		virtual void OnSerializationStarted(IReflectedType* obj)
		{
			UNUSED(obj);
		}

		virtual void OnSerializationEnded(IReflectedType* obj)
		{
			UNUSED(obj);
		}

		virtual void OnDeserializationStarted(IReflectedType* obj)
		{
			UNUSED(obj);
		}

		virtual void OnDeserializationEnded(IReflectedType* obj)
		{
			UNUSED(obj);
		}

		INLINE uint32 GetFieldCount()const noexcept { return m_Fields.size(); }

		Result<IReflectedField*> GetField(sizet index)const noexcept;

		Result<IReflectedField*> FindField(const String& name)const noexcept;

		Result<IReflectedField*> FindField(ReflectedFieldID_t fieldID)const noexcept;

		virtual void _RegisterDerivedClass(IReflectedTypeInfo* derivedClass) = 0;

		virtual IReflectedTypeInfo* _Clone()const = 0;
	};

	class IReflectedType
	{
	public:
		virtual ~IReflectedType() noexcept = default;

		virtual IReflectedTypeInfo* GetReflectedType()const = 0;

		INLINE bool IsDerivedFrom(IReflectedTypeInfo* base) noexcept
		{
			return GetReflectedType()->IsDerivedFrom(base);
		}

		INLINE ReflectedTypeID_t GetTypeID()const noexcept
		{
			return GetReflectedType()->GetReflectedTypeID();
		}

		INLINE const String& GetTypeName()const noexcept
		{
			return GetReflectedType()->GetReflectedName();
		}
	};

	INLINE void IReflectedTypeInfo::AddNewField(IReflectedField* field)
	{
		VerifyNotNull(field, "Trying to add a nullptr field.");

		const auto uniqueID = field->Info.FieldID;
		auto temp = FindField(uniqueID);

		Verify(temp.HasFailed(), "Trying to add a field with ID:%d, but already exists one with that ID.", uniqueID);

		temp = FindField(field->Name);

		Verify(temp.HasFailed(), "Trying to add a field with Name:'%s', but already exists one with that ID.", field->Name);

		m_Fields.push_back(field);
	}

	INLINE IReflectedTypeInfo::~IReflectedTypeInfo() noexcept
	{
		for (auto* field : m_Fields)
		{
			Destroy(field);
		}
	}

	INLINE Result<IReflectedField*> IReflectedTypeInfo::GetField(sizet index)const noexcept
	{
		if (m_Fields.size() <= index)
			return CreateFailure<IReflectedField*>("Trying to get an out of bounds index.");
		return CreateResult(m_Fields[index]);
	}

	INLINE Result<IReflectedField*> IReflectedTypeInfo::FindField(const String& name)const noexcept
	{
		IReflectedField* field = nullptr;

		for (auto* f : m_Fields)
		{
			if (f->Name == name)
			{
				field = f;
				break;
			}
		}

		if (field == nullptr)
			return CreateFailure<IReflectedField*>(Format("Couldn't find field '%s'.", name.c_str()));
		return CreateResult(field);
	}

	INLINE Result<IReflectedField*> IReflectedTypeInfo::FindField(ReflectedFieldID_t fieldID)const noexcept
	{
		IReflectedField* field = nullptr;

		for (auto* f : m_Fields)
		{
			if (f->Info.FieldID == fieldID)
			{
				field = f;
				break;
			}
		}

		if (field == nullptr)
			return CreateFailure<IReflectedField*>(Format("Couldn't find field with fieldID '%d'.", fieldID));
		return CreateResult(field);
	}
}

#endif /* CORE_REFLECTION_REFLECTED_TYPE_INFO_H */