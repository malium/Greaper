/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_PROPERTY_H
#define CORE_PROPERTY_H 1

#include "Memory.h"
#include "Base/PropertyValidator.h"
//#include "Base/PropertyConverter.h"
#include "Reflection/ReflectedPlainType.h"
#include "Reflection/ReflectedPlainContainer.h"
#include "Event.h"
#include "IGreaperLibrary.h"
#include "Concurrency.h"

namespace greaper
{
	/**
	* @brief Base property interface, provides the foundation to use properties
	* without the templated interaction.
	*/
	class IProperty
	{
	public:
		using ModificationEvent_t = Event<IProperty*>;
		using ModificationEventHandler_t = ModificationEvent_t::HandlerType;
		using ModificationEventFunction_t = ModificationEvent_t::HandlerFunction;
		
		virtual ~IProperty() = default;

		virtual const String& GetPropertyName()const noexcept = 0;
		virtual const String& GetPropertyInfo()const noexcept = 0;
		virtual bool IsConstant()const noexcept = 0;
		virtual bool IsStatic()const noexcept = 0;
		virtual bool SetValueFromString(const String& value) noexcept = 0;
		virtual const String& GetStringValue()const noexcept = 0;
		virtual ModificationEvent_t*const GetOnModificationEvent() noexcept = 0;
		virtual IGreaperLibrary* GetLibrary()const noexcept = 0;
	};

	/**
	 * @brief Stores configuration information, that information must be able to be 
	 * serialized into a string. Works like a ConsoleVariable (ID Software like).
	 * A breaf documentation can be attached to the Property on the PropertyInfo
	 * variable. PropertyName must be unique in each library, otherwise a conflict
	 * will ocurr. Properties can be static, created from the sources and not serialized,
	 * can be constant, they cannot be modified once created. Properties use a
	 * PropertyValidator, which as the name states, validates if the Property has
	 * a valid value, there are several types of validators and you can provide yours,
	 * extending TPropertyValidator.
	 * In order to have other types of properties you must provide an extension to 
	 * TPropertyConverter with the given type, in order to be able to serialize them to
	 * string and from it.
	 * 
	 * @tparam T - Base type of the property, must be a serializable type or a pod type 
	 */
	template<class T>
	class TProperty : public IProperty
	{
		T m_Value;
		String m_PropertyName;
		String m_PropertyInfo;
		String m_StringValue;	// When a property is changed, needs to update this value
		ModificationEvent_t m_OnModificationEvent;
		TPropertyValidator<T>* m_PropertyValidator;
		IGreaperLibrary* m_Library;
		mutable RWMutex m_Mutex;

		bool m_Static;		// Created at the start of the program cannot be saved
		bool m_Constant;	// Cannot be modified

		TProperty(IGreaperLibrary* library, const StringView& propertyName, T initialValue, const StringView& propertyInfo = StringView{},
			bool isConstant = false, bool isStatic = false, TPropertyValidator<T>* validator = nullptr) noexcept
			:m_Value(std::move(initialValue))
			, m_PropertyName(propertyName)
			, m_PropertyInfo(propertyInfo)
			, m_OnModificationEvent("PropertyModified"sv)
			, m_PropertyValidator(validator)
			, m_Library(library)
			, m_Static(isStatic)
			, m_Constant(isConstant)
		{
			if (m_PropertyValidator == nullptr)
			{
				m_PropertyValidator = new PropertyValidatorNone<T>();
			}
			m_PropertyValidator->Validate(m_Value, &m_Value);
			m_StringValue = ReflectedPlainType<T>::ToString(m_Value);
			//m_StringValue = TPropertyConverter<T>::ToString(m_Value);
		}

		template<class T, class _Alloc_>
		friend Result<TProperty<T>*> CreateProperty(greaper::IGreaperLibrary*, const StringView&, T, const StringView&,
			bool, bool, TPropertyValidator<T>*);
	public:
		using value_type = T;

		TProperty(const TProperty&) = delete;
		TProperty& operator=(const TProperty&) = delete;

		[[nodiscard]] const String& GetPropertyName()const noexcept override
		{
			return m_PropertyName;
		}
		[[nodiscard]] const String& GetPropertyInfo()const noexcept override
		{
			return m_PropertyInfo;
		}
		[[nodiscard]] TPropertyValidator<T>* GetPropertyValidator()const noexcept
		{
			return m_PropertyValidator;
		}
		[[nodiscard]] bool IsConstant()const noexcept override
		{
			return m_Constant;
		}
		[[nodiscard]] bool IsStatic()const noexcept override
		{
			return m_Static;
		}
		bool SetValue(const T& value, bool triggerEvent = true) noexcept
		{
			if (m_Constant)
			{
				m_Library->LogWarning(Format("Trying to change a constant property, '%s'.", m_PropertyName.c_str()));
				return false;
			}
			auto lock = Lock<RWMutex>(m_Mutex);
			T old = m_Value;
			T newValue;
			if (!m_PropertyValidator->Validate(value, &newValue))
			{
				//const String nValueStr = TPropertyConverter<T>::ToString(value);
				const String nValueStr = ReflectedPlainType<T>::ToString(m_Value);
				m_Library->LogWarning(Format("Couldn't validate the new value of Property '%s', oldValue '%s', newValue '%s'.",
					m_PropertyName.c_str(), m_StringValue.c_str(), nValueStr.c_str()));
				return false;
			}
			m_Value = newValue;
			if (old == m_Value)
			{
				//const String nValueStr = TPropertyConverter<T>::ToString(value);
				const String nValueStr = ReflectedPlainType<T>::ToString(m_Value);
				m_Library->LogVerbose(Format("Property '%s', has mantain the same value, current '%s', tried '%s'.",
					m_PropertyName.c_str(), m_StringValue.c_str(), nValueStr.c_str()));
				return false; // Property has not changed;
			}
			const auto oldStringValue = String{ m_StringValue };
			//m_StringValue = TPropertyConverter<T>::ToString(m_Value);
			m_StringValue = ReflectedPlainType<T>::ToString(m_Value);
			m_Library->LogVerbose(Format("Property '%s', has changed from '%s' to '%s'.",
				m_PropertyName.c_str(), oldStringValue.c_str(), m_StringValue.c_str()));
			if (triggerEvent)
				m_OnModificationEvent.Trigger(this);
			return true;
		}
		bool SetValueFromString(const String& value) noexcept override
		{
			//return SetValue(TPropertyConverter<T>::FromString(value));
			T temp;
			ReflectedPlainType<T>::FromString(temp, value);
			return SetValue(temp);
		}
		[[nodiscard]] const T& GetValue()const noexcept
		{
			auto lock = SharedLock(m_Mutex);
			return m_Value;
		}
		[[nodiscard]] const String& GetStringValue()const noexcept override
		{
			auto lock = SharedLock(m_Mutex);
			return m_StringValue;
		}
		[[nodiscard]] ModificationEvent_t*const GetOnModificationEvent() noexcept override
		{
			return &m_OnModificationEvent;
		}
		[[nodiscard]] IGreaperLibrary* GetLibrary()const noexcept override
		{
			return m_Library;
		}
	};

	// Greaper Core specialization
	using PropertyBool = TProperty<bool>;
	using PropertyInt = TProperty<int32>;
	using PropertyFloat = TProperty<float>;
	using PropertyString = TProperty<String>;
	using PropertyStringVec = TProperty<StringVec>;

	// A way to retrieve the RTI_ID from the type Property
	template<> struct ReflectedTypeToID<IProperty> { static constexpr ReflectedTypeID_t ID = RTI_Property; };
	template<typename T> struct ReflectedTypeToID<TProperty<T>> { static constexpr ReflectedTypeID_t ID = RTI_Property; };

	template<class T, class _Alloc_ = greaper::GenericAllocator>
	Result<TProperty<T>*> CreateProperty(IGreaperLibrary* library, const StringView& propertyName, T initialValue, const StringView& propertyInfo = StringView{},
		bool isConstant = false, bool isStatic = false, TPropertyValidator<T>* validator = nullptr)
	{
		TProperty<T>* property = Construct<TProperty<T>, _Alloc_>(propertyName, std::move(initialValue), propertyInfo, isConstant, isStatic, validator);
		EmptyResult res = library->RegisterProperty((IProperty*)property);
		if (res.HasFailed())
		{
			Destroy<TProperty<T>, _Alloc_>(property);
			return CreateFailure<TProperty<T>*>("Couldn't register the property\n" + res.GetFailMessage());
		}
		return CreateResult(property);
	}

	template<class T>
	Result<TProperty<T>*> GetProperty(IGreaperLibrary* library, const String& name)
	{
		auto res = library->GetProperty(name);
		if (res.HasFailed())
			return CopyFailure<TProperty<T>*>(res);
		return CreateResult(reinterpret_cast<TProperty<T>*>(res.GetValue()));
	}
}

#endif /* CORE_PROPERTY_H */