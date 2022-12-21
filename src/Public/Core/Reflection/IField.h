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
		StringView m_FieldName;
		ReflectedTypeID_t m_FieldTypeID;
		bool m_IsArray;
		std::function<void* (void*)> m_GetValueFn;
		std::function<void(void*, void*)> m_SetValueFn;

	public:
		IField(StringView fieldName = "unnamed"sv, ReflectedTypeID_t fieldTypeID = RTI_Unknown, bool isArray = false,
			std::function<void*(void*)> getValueFn = nullptr, std::function<void(void*, void*)> setValueFn = nullptr)noexcept
			:m_FieldName(std::move(fieldName))
			,m_FieldTypeID(fieldTypeID)
			,m_IsArray(isArray)
			,m_GetValueFn(getValueFn)
			,m_SetValueFn(setValueFn)
		{

		}

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


		NODISCARD INLINE void* GetValue(void* obj)const noexcept { return m_GetValueFn != nullptr ? m_GetValueFn(obj) : nullptr; }
		INLINE void SetValue(void* obj, void* value)const noexcept { if (m_SetValueFn != nullptr) m_SetValueFn(obj, value); }
		NODISCARD INLINE constexpr bool IsArray()const noexcept { return m_IsArray; }
		NODISCARD INLINE constexpr ReflectedTypeID_t GetTypeID()const noexcept { return m_FieldTypeID; }
		NODISCARD INLINE constexpr StringView GetFieldName()const noexcept { return m_FieldName; }
	};

	template<class T>
	class TField final : public IField
	{
	protected:
		using Type = RemoveEverything_t<T>;
		using tInfo = TypeInfo<Type>;
		static_assert(!std::is_same_v<tInfo::Type, void>, "[refl::TField<T>] instantiated with an Unknown TypeID.");

		using ArrayValueType = tInfo::Type::ArrayValueType;

	public:
		INLINE TField(StringView fieldName, std::function<const void*(const void*)> getValueFn, std::function<void(void*, void*)> setValueFn)noexcept
			:IField(std::move(fieldName), tInfo::ID, std::is_same_v<tInfo::Type, ContainerType<Type>>)
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
			tInfo::Type::SetArrayValue((Type&)*((Type*)arr), (const ArrayValueType&)*((const ArrayValueType*)value), index);
		}
	};

	struct TestObj
	{
		int32 A;
		float B;
	};
	TField<int32> TestA("A"sv, [](const void* obj) -> const void* { return &((const TestObj*)obj)->A; }, [](void* obj, void* value) { ((TestObj*)obj)->A = *((int32*)value); });
	class TestClass
	{
		int32 A;
	public:
		void SetA(int32 a)noexcept { A = a; }
		const int32& GetA()const noexcept { return A; }
	};
	TField<int32> testClass("A"sv,
		[](const void* obj) -> const void* { return &(((const TestClass*)obj)->GetA()); },
		[](void* obj, void* value) { ((TestClass*)obj)->SetA(*((int32*)value)); });

	void a()
	{
		TestObj obj;
		auto a = TestA.GetValue(&obj);
		auto json = TestA.CreateJSON(a);
	}
}

#endif /* CORE_REFLECTION_I_FIELD_H */