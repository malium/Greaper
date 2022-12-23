/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_REFLECTION_I_FIELD_H
#define CORE_REFLECTION_I_FIELD_H 1

#include "../CorePrerequisites.h"
#include "PlainType.h"
#include "../Concurrency.h"

struct cJSON;

namespace greaper::refl
{
	class IField
	{
	protected:
		StringView m_FieldName;
		std::function<const void* (const void*)> m_GetValueFn = nullptr;
		std::function<void(void*, const void*)> m_SetValueFn = nullptr;

	public:
		IField(StringView fieldName = "unnamed"sv,
			std::function<const void*(const void*)> getValueFn = nullptr, std::function<void(void*, const void*)> setValueFn = nullptr)noexcept
			:m_FieldName(std::move(fieldName))
			,m_GetValueFn(std::move(getValueFn))
			,m_SetValueFn(std::move(setValueFn))
		{

		}

		virtual TResult<ssizet> ToStream(const void* complexPtr, IStream& stream)const = 0;
		virtual TResult<ssizet> FromStream(void* complexPtr, IStream& stream)const = 0;
		virtual TResult<std::tuple<void*, ssizet, RecursiveMutex*>> CreateFromStream(IStream& stream)const = 0;
		virtual SPtr<cJSON> CreateJSON(const void* complexPtr)const = 0;
		virtual cJSON* ToJSON(const void* complexPtr, cJSON* json)const = 0;
		virtual EmptyResult FromJSON(void* complexPtr, cJSON* json)const = 0;
		virtual TResult<std::tuple<void*, RecursiveMutex*>> CreateFromJSON(cJSON* json)const = 0;
		virtual String ToString(const void* complexPtr)const = 0;
		virtual EmptyResult FromString(const String& str, void* complexPtr)const = 0;
		virtual TResult<std::tuple<void*, RecursiveMutex*>> CreateFromString(const String& str)const = 0;
		virtual int64 GetDynamicSize(const void* complexPtr)const = 0;
		virtual int64 GetStaticSize()const = 0;

		virtual sizet GetArraySize(const void* complexPtr)const = 0;
		virtual void SetArraySize(void* complexPtr, sizet size)const = 0;
		virtual const void* GetArrayValue(const void* complexPtr, sizet index)const = 0;
		virtual void SetArrayValue(void* complexPtr, const void* value, sizet index)const = 0;


		NODISCARD INLINE const void* GetValue(const void* complexPtr)const noexcept
		{
			if (m_GetValueFn != nullptr)
				return m_GetValueFn(complexPtr);
			return nullptr;
		}
		INLINE void SetValue(void* complexPtr, const void* value)const noexcept
		{
			if (m_SetValueFn != nullptr)
				m_SetValueFn(complexPtr, value);
		}
		virtual bool IsArray()const noexcept = 0;
		virtual ReflectedTypeID_t GetTypeID()const noexcept = 0;
		NODISCARD INLINE constexpr StringView GetFieldName()const noexcept { return m_FieldName; }
	};

	template<class T>
	class TField final : public IField
	{
	public:
		using Type = RemoveEverything_t<T>;
		using tInfo = TypeInfo<Type>;
		using ArrayValueType = tInfo::Type::ArrayValueType;

		static_assert(!std::is_same_v<tInfo::Type, void>, "[refl::TField<T>] instantiated with an Unknown TypeID.");

		INLINE TField(StringView fieldName, std::function<const void*(const void*)> getValueFn, std::function<void(void*, const void*)> setValueFn)noexcept
			:IField(std::move(fieldName), std::move(getValueFn), std::move(setValueFn))
		{

		}

		INLINE bool IsArray()const noexcept override { return std::is_same_v<tInfo::Type, ContainerType<Type>>; }

		INLINE ReflectedTypeID_t GetTypeID()const noexcept override { return tInfo::ID; }

		INLINE TResult<ssizet> ToStream(const void* complexPtr, IStream& stream)const override
		{
			const Type* valuePtr = (const Type*)GetValue(complexPtr);
			if(valuePtr == nullptr)
				return Result::CreateFailure<ssizet>("[refl::TField<T>]::ToStream failed to obtain the value.");

			return tInfo::Type::ToStream(*valuePtr, stream);
		}
		INLINE TResult<ssizet> FromStream(void* complexPtr, IStream& stream)const override
		{
			Type temp;
			TResult<ssizet> res = tInfo::Type::FromStream(temp, stream);
			if(res.HasFailed())
				return res;
			SetValue(complexPtr, &temp);
			return res;
		}
		INLINE SPtr<cJSON> CreateJSON(const void* complexPtr)const override
		{
			const Type* valuePtr = (const Type*)GetValue(complexPtr);
			if(valuePtr == nullptr)
				return nullptr;
			return tInfo::Type::CreateJSON(*valuePtr, m_FieldName);
		}
		INLINE cJSON* ToJSON(const void* complexPtr, cJSON* json)const override
		{
			const Type* valuePtr = (const Type*)GetValue(complexPtr);
			if(valuePtr == nullptr)
				return nullptr;
			return tInfo::Type::ToJSON(*valuePtr, json, m_FieldName);
		}
		INLINE EmptyResult FromJSON(void* complexPtr, cJSON* json)const override
		{
			Type temp;
			EmptyResult res = tInfo::Type::FromJSON(temp, json, m_FieldName);
			if(res.HasFailed())
				return res;
			SetValue(complexPtr, &temp);
			return res;
		}
		NODISCARD INLINE String ToString(const void* complexPtr)const override
		{
			const Type* valuePtr = (const Type*)GetValue(complexPtr);
			if(valuePtr == nullptr)
				return String{};
			return tInfo::Type::ToString(*valuePtr);
		}
		INLINE EmptyResult FromString(const String& str, void* complexPtr)const override
		{
			Type temp;
			EmptyResult res = tInfo::Type::FromString(str, temp);
			if(res.HasFailed())
				return res;
			SetValue(complexPtr, &temp);
			return res;
		}
		NODISCARD INLINE int64 GetDynamicSize(const void* complexPtr)const override
		{
			const Type* valuePtr = (const Type*)GetValue(complexPtr);
			if(valuePtr == nullptr)
				return 0ll;
			return tInfo::Type::GetDynamicSize(*valuePtr);
		}
		NODISCARD INLINE int64 GetStaticSize()const override
		{
			return tInfo::Type::StaticSize;
		}
		NODISCARD INLINE sizet GetArraySize(const void* complexPtr)const override
		{
			const Type* arrPtr = (const Type*)GetValue(complexPtr);
			if(arrPtr == nullptr)
				return 0ll;
			return tInfo::Type::GetArraysize(*arrPtr);
		}
		INLINE void SetArraySize(void* complexPtr, sizet size)const override
		{
			Type* arrPtr = (Type*)GetValue(complexPtr);
			if(arrPtr == nullptr)
				return;
			tInfo::Type::SetArraysize(*arrPtr, size);
		}
		INLINE const void* GetArrayValue(const void* complexPtr, sizet index)const override
		{
			const Type* arrPtr = (const Type*)GetValue(complexPtr);
			if(arrPtr == nullptr)
				return nullptr;
			return &tInfo::type::GetArrayValue(*arrPtr, index);
		}
		INLINE void SetArrayValue(void* complexPtr, const void* value, sizet index)const override
		{
			Type* arrPtr = (Type*)GetValue(complexPtr);
			if(arrPtr == nullptr)
				return;
			tInfo::Type::SetArrayValue(*arrPtr, *((const ArrayValueType*)value), index);
		}
	};
}

#endif /* CORE_REFLECTION_I_FIELD_H */