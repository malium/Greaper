/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

//#include "../IGreaperLibrary.h"

namespace greaper
{
	template<class T, class _Alloc_>
	TResult<PProperty<T>> CreateProperty(WGreaperLib library, StringView propertyName, T initialValue, StringView propertyInfo,
		bool isConstant, bool isStatic, TPropertyValidator<T>* validator)
	{
		auto lib = library.lock();
		if (lib == nullptr)
			return Result::CreateFailure<PProperty<T>>("Couldn't create the property, expired library given"sv);

		TProperty<T>* propPtr = Construct<TProperty<T>, _Alloc_>(library, propertyName, std::move(initialValue), propertyInfo, isConstant, isStatic, validator);
		PProperty<T> prop(propPtr, Impl::SPtrDeleterFn_t<TProperty<T>>(&Impl::DefaultDeleter<TProperty<T>, _Alloc_>));
		const auto res = lib->RegisterProperty((PIProperty)prop);
		if (res.HasFailed())
		{
			return Result::CreateFailure<PProperty<T>>("Couldn't register the property\n" + res.GetFailMessage());
		}
		return Result::CreateSuccess(prop);
	}
	
	template<class T>
	INLINE TResult<WProperty<T>> GetProperty(const WGreaperLib& library, const String& name)
	{
		auto lib = library.lock();
		if (lib == nullptr)
			return Result::CreateFailure<WProperty<T>>("Couldn't retrieve the property, expired library given.");

		auto res = lib->GetProperty(name);
		if (res.HasFailed())
			return Result::CopyFailure<TProperty<T>*>(res);
		return Result::CreateSuccess(reinterpret_cast<TProperty<T>*>(res.GetValue()));
	}

	template<class T>
	INLINE TProperty<T>::TProperty(WGreaperLib library, const StringView& propertyName, T initialValue, const StringView& propertyInfo,
			bool isConstant, bool isStatic, TPropertyValidator<T>* validator) noexcept
		:m_Value(std::move(initialValue))
		,m_PropertyName(propertyName)
		,m_PropertyInfo(propertyInfo)
		,m_OnModificationEvent("PropertyModified"sv)
		,m_PropertyValidator(validator)
		,m_Library(std::move(library))
		,m_Static(isStatic)
		,m_Constant(isConstant)
	{
		if (m_PropertyValidator == nullptr)
			{
				m_PropertyValidator = Construct<PropertyValidatorNone<T>>();
			}
			m_PropertyValidator->Validate(m_Value, &m_Value);
			m_StringValue = TCategory::ToString(m_Value);
	}

	template<class T>
	NODISCARD INLINE const String& TProperty<T>::GetPropertyName()const noexcept
	{
		return m_PropertyName;
	}

	template<class T>
	NODISCARD INLINE const String& TProperty<T>::GetPropertyInfo()const noexcept
	{
		return m_PropertyInfo;
	}

	template<class T>
	NODISCARD INLINE TPropertyValidator<T>* TProperty<T>::GetPropertyValidator()const noexcept
	{
		return m_PropertyValidator;
	}

	template<class T>
	NODISCARD INLINE bool TProperty<T>::IsConstant()const noexcept
	{
		return m_Constant;
	}

	template<class T>
	NODISCARD INLINE bool TProperty<T>::IsStatic()const noexcept
	{
		return m_Static;
	}

	template<class T>
	inline bool TProperty<T>::SetValue(const T& value, bool triggerEvent, bool ignoreConstness) noexcept
	{
		VerifyNot(m_Library.expired(), "Trying to set a value to a disconnected Property.");
		auto lib = m_Library.lock();
		if (m_Constant && !ignoreConstness)
		{
			lib->LogWarning(Format("Trying to change a constant property, '%s'.", m_PropertyName.c_str()));
			return false;
		}
		auto lock = Lock<RWMutex>(m_Mutex);
		T old = m_Value;
		T newValue;
		if (!m_PropertyValidator->Validate(value, &newValue))
		{
			const String nValueStr = ReflectedPlainType<T>::ToString(m_Value);
			lib->LogWarning(Format("Couldn't validate the new value of Property '%s', oldValue '%s', newValue '%s'.",
				m_PropertyName.c_str(), m_StringValue.c_str(), nValueStr.c_str()));
			return false;
		}
		m_Value = newValue;
		if (old == m_Value)
		{
			const String nValueStr = ReflectedPlainType<T>::ToString(m_Value);
			lib->LogVerbose(Format("Property '%s', has mantain the same value, current '%s', tried '%s'.",
				m_PropertyName.c_str(), m_StringValue.c_str(), nValueStr.c_str()));
			return false; // Property has not changed;
		}
		const auto oldStringValue = String{ m_StringValue };
		m_StringValue = ReflectedPlainType<T>::ToString(m_Value);
		lib->LogVerbose(Format("Property '%s', has changed from '%s' to '%s'.",
			m_PropertyName.c_str(), oldStringValue.c_str(), m_StringValue.c_str()));
		if (triggerEvent)
			m_OnModificationEvent.Trigger(this);
		return true;
	}

	template<class T>
	INLINE bool TProperty<T>::SetValueFromString(const String& value) noexcept
	{
		T temp;
		TCategory::FromString(value, temp);
		//ReflectedPlainType<T>::FromString(temp, value);
		return SetValue(temp);
	}

	template<class T>
	NODISCARD inline T TProperty<T>::GetValueCopy()const noexcept
	{
		auto lck = SharedLock(m_Mutex);
		return { m_Value };
	}

	template<class T>
	INLINE void TProperty<T>::AccessValue(const std::function<void(const T&)>& accessFn)const noexcept
	{
		auto lck = SharedLock(m_Mutex);
		accessFn(m_Value);
	}

	template<class T>
	NODISCARD INLINE String TProperty<T>::GetStringValueCopy()const noexcept
	{
		auto lck = SharedLock(m_Mutex);
		return { m_StringValue };
	}

	template<class T>
	INLINE void TProperty<T>::AccessStringValue(const std::function<void(const String&)>& accessFn)const noexcept
	{
		auto lck = SharedLock(m_Mutex);
		accessFn(m_StringValue);
	}

	template<class T>
	INLINE TResult<ssizet> TProperty<T>::_ValueToStream(IStream& stream) const noexcept
	{
		auto lck = SharedLock(m_Mutex);
		return TCategory::ToStream(m_Value, stream);
	}

	template<class T>
	INLINE TResult<ssizet> TProperty<T>::_ValueFromStream(IStream& stream) noexcept
	{
		auto lck = Lock(m_Mutex);
		return TCategory::FromStream(m_Value, stream);
	}

	template<class T>
	INLINE cJSON* TProperty<T>::_ValueToJSON(cJSON* json, StringView name) const noexcept
	{
		auto lck = SharedLock(m_Mutex);
		return TCategory::ToJSON(m_Value, json, name);
	}

	template<class T>
	INLINE EmptyResult TProperty<T>::_ValueFromJSON(cJSON* json, StringView name) noexcept
	{
		auto lck = Lock(m_Mutex);
		return TCategory::FromJSON(m_Value, json, name);
	}

	template<class T>
	INLINE int64 TProperty<T>::_GetDynamicSize() const noexcept
	{
		auto lck = SharedLock(m_Mutex);
		return TCategory::GetDynamicSize(m_Value);
	}

	template<class T>
	NODISCARD INLINE IProperty::ModificationEvent_t* TProperty<T>::GetOnModificationEvent()const noexcept
	{
		return &m_OnModificationEvent;
	}
	
	template<class T>
	NODISCARD INLINE const WGreaperLib& TProperty<T>::GetLibrary()const noexcept
	{
		return m_Library;
	}
}