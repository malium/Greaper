/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_I_FIELD_H
#define CORE_REFLECTION_I_FIELD_H 1

#include "../CorePrerequisites.h"
#include "PlainType.h"

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
		constexpr IField()noexcept = default;

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
		virtual const void* GetArrayValue(const void* arr, sizet index)const = 0;
		virtual void SetArrayValue(void* arr, const void* value, sizet index)const = 0;

		NODISCARD INLINE constexpr bool IsArray()const noexcept { return m_IsArray; }
		NODISCARD INLINE constexpr ReflectedTypeID_t GetTypeID()const noexcept { return m_FieldID; }
		NODISCARD INLINE constexpr StringView GetFieldName()const noexcept { return m_FieldName; }
	};

	template<class T>
	class TField final : public IField
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
			return tInfo::Type::GetArraySize((const Type&)*((const Type*)arr));
		}
		INLINE void SetArraySize(void* arr, sizet size)const override
		{
			tInfo::Type::SetArraySize((Type&)*((Type*)arr), size);
		}
		INLINE const void* GetArrayValue(const void* arr, sizet index)const override
		{
			return (const void*)&(tInfo::Type::GetArrayValue((const Type&)*((const Type*)arr), index));
		}
		INLINE void SetArrayValue(void* arr, const void* value, sizet index)const override
		{
			tInfo::Type::SetArrayValue((Type&)*((Type*)arr), (const Type::value_type&)*((const Type::value_type*)value), index);
		}
	};

#if 0
	constexpr TField<int32> test{ "test"sv };
	constexpr ReflectedTypeID_t fieldTypeID = test.GetTypeID();
	constexpr StringView fieldName = test.GetFieldName();
	constexpr bool fieldIsArray = test.IsArray();
#endif
}

#endif /* CORE_REFLECTION_I_FIELD_H */