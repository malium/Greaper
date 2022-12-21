/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_I_FIELD_H
#define CORE_REFLECTION_I_FIELD_H 1

#include "../CorePrerequisites.h"

struct cJSON;

namespace greaper::refl
{
	class IField
	{
	protected:
		StringView m_FieldName = "unnamed"sv;
		ReflectedTypeID_t m_FieldID = RTI_Unknown;
		bool m_IsArray = false;

	public:
		virtual TResult<ssizet> ToStream(const void* value, IStream& stream)const = 0;
		virtual TResult<ssizet> FromStream(void* value, IStream& stream)const = 0;
		virtual SPtr<cJSON> CreateJSON(const void* value)const = 0;
		virtual cJSON* ToJSON(const void* value, cJSON* json)const = 0;
		virtual EmptyResult FromJSON(void* value, cJSON* json)const = 0;
		virtual String ToString(const void* value)const = 0;
		virtual EmptyResult FromString(const String& str, void* value)const = 0;
		virtual int64 GetDynamicSize(const void* value)const = 0;
		virtual int64 GetStaticSize()const = 0;

		virtual sizet GetArraySize(const void* arr)const = 0;
		virtual void SetArraySize(void* arr, sizet size)const = 0;
		virtual void* GetArrayValue(void* arr, sizet index)const = 0;
		virtual void SetArrayValue(void* arr, const void* value, sizet index)const = 0;

		NODISCARD INLINE bool IsArray()const noexcept { return m_IsArray; }
		NODISCARD INLINE ReflectedTypeID_t GetTypeID()const noexcept { return m_FieldID; }
		NODISCARD INLINE StringView GetFieldName()const noexcept { return m_FieldName; }
	};

	template<class T>
	class TField : public IField
	{
	protected:
		using Type = RemoveEverything_t<T>;
		using tInfo = TypeInfo<Type>;
		static_assert(std::is_same_v<tInfo::Type, void>, "[refl::TField<T>] instantiated with an Unknown TypeID.");

		bool m_IsArray = std::is_same_v<tInfo, Container<Type>>;
		ReflectedFieldID_t m_FieldID = tInfo::ID;

	public:
		INLINE constexpr TField(StringView fieldName)noexcept
			:m_FieldName(fieldName)
		{
			
		}

		INLINE TResult<ssizet> ToStream(const void* value, IStream& stream)const override
		{
			return tInfo::Type::ToStream((const Type&)*((const Type*)value), stream);
		}
		INLINE TResult<ssizet> FromStream(void* value, IStream& stream)const override
		{
			return tInfo::Type::FromStream((Type&)*((Type*)value), stream);
		}
		INLINE SPtr<cJSON> CreateJSON(const void* value)const override
		{
			return tInfo::Type::CreateJSON((const Type&)*((const Type*)value), m_FieldName);
		}
		INLINE cJSON* ToJSON(const void* value, cJSON* json)const override
		{
			return tInfo::Type::ToJSON((const Type&)*((const Type*)value), json, m_FieldName);
		}
		INLINE EmptyResult FromJSON(void* value, cJSON* json)const override
		{
			return tInfo::Type::FromJSON((Type&)*((Type*)value), json, m_FieldName);
		}
		NODISCARD INLINE String ToString(const void* value)const override
		{
			return tInfo::Type::ToString((const Type&)*((const Type*)value));
		}
		INLINE EmptyResult FromString(const String& str, void* value)const override
		{
			return tInfo::Type::FromString(str, (Type&)*((Type*)value));
		}
		NODISCARD INLINE int64 GetDynamicSize(const void* value)const override
		{
			return tInfo::Type::GetDynamicSize((const Type&)*((const Type*)value));
		}
		NODISCARD INLINE int64 GetStaticSize()const override
		{
			return tInfo::Type::StaticSize;
		}
		NODISCARD INLINE sizet GetArraySize(const void* arr)const override
		{
			
		}
		INLINE void SetArraySize(void* arr, sizet size)const override
		{

		}
		INLINE void* GetArrayValue(void* arr, sizet index)const override
		{

		}
		INLINE void SetArrayValue(void* arr, const void* value, sizet index)const override
		{
			
		}
	};
}

#endif /* CORE_REFLECTION_I_FIELD_H */