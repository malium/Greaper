/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_PROPERTY_H
#define CORE_PROPERTY_H 1

#include <utility>
#include "Memory.h"
#include "Base/PropertyValidator.h"
#include "Reflection/PlainType.h"
#include "Reflection/ContainerType.h"
//#include "Base/PropertyConverter.h"
//#include "Reflection/ReflectedPlainType.h"
//#include "Reflection/ReflectedPlainContainer.h"
#include "Event.h"
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
		virtual String GetStringValueCopy()const noexcept = 0;
		virtual void AccessStringValue(const std::function<void(const String&)>& accessFn)const noexcept = 0;
		virtual ModificationEvent_t* GetOnModificationEvent()const noexcept = 0;
		virtual const WGreaperLib& GetLibrary()const noexcept = 0;
		virtual std::size_t GetValueHash()const noexcept = 0;
		virtual const ReflectedTypeID_t& _ValueTypeID()const noexcept = 0;
		virtual TResult<ssizet> _ValueToStream(IStream& stream)const noexcept = 0;
		virtual TResult<ssizet> _ValueFromStream(IStream& stream)noexcept = 0;
		virtual cJSON* _ValueToJSON(cJSON* json, StringView name)const noexcept = 0;
		virtual EmptyResult _ValueFromJSON(cJSON* json, StringView name)noexcept = 0;
		virtual int64 _GetDynamicSize()const noexcept = 0;
		virtual int64 _GetStaticSize()const noexcept = 0;
	};

	template<class T, class _Alloc_ = GenericAllocator>
	TResult<PProperty<T>> CreateProperty(WGreaperLib library, StringView propertyName, T initialValue, StringView propertyInfo = {},
		bool isConstant = false, bool isStatic = false, TPropertyValidator<T>* validator = nullptr);

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
	class TProperty final : public IProperty
	{
		T m_Value;
		String m_PropertyName;
		String m_PropertyInfo;
		String m_StringValue;	// When a property is changed, needs to update this value
		mutable ModificationEvent_t m_OnModificationEvent;
		TPropertyValidator<T>* m_PropertyValidator;
		WGreaperLib m_Library;
		mutable RWMutex m_Mutex;

		bool m_Static;		// Created at the start of the program cannot be saved
		bool m_Constant;	// Cannot be modified

		TProperty(WGreaperLib library, const StringView& propertyName, T initialValue, const StringView& propertyInfo = StringView{},
			bool isConstant = false, bool isStatic = false, TPropertyValidator<T>* validator = nullptr) noexcept;

		template<class _T_, class _Alloc_>
		friend TResult<PProperty<T>> CreateProperty(WGreaperLib library, StringView propertyName, _T_ initialValue, StringView propertyInfo,
			bool isConstant, bool isStatic, TPropertyValidator<_T_>* validator);
		MemoryFriend();
	public:
		using value_type = T;

		TProperty(const TProperty&) = delete;
		TProperty& operator=(const TProperty&) = delete;

		const String& GetPropertyName()const noexcept override;
		
		const String& GetPropertyInfo()const noexcept override;

		TPropertyValidator<T>* GetPropertyValidator()const noexcept;
		bool IsConstant()const noexcept override;
		bool IsStatic()const noexcept override;
		bool SetValue(const T& value, bool triggerEvent = true, bool ignoreConstness = false) noexcept;
		bool SetValueFromString(const String& value) noexcept override;
		T GetValueCopy()const noexcept;
		void AccessValue(const std::function<void(const T&)>& accessFn)const noexcept;
		String GetStringValueCopy()const noexcept override;
		void AccessStringValue(const std::function<void(const String&)>& accessFn)const noexcept;
		const ReflectedTypeID_t& _ValueTypeID()const noexcept override;
		TResult<ssizet> _ValueToStream(IStream& stream)const noexcept override;
		TResult<ssizet> _ValueFromStream(IStream& stream)noexcept override;
		cJSON* _ValueToJSON(cJSON* json, StringView name)const noexcept override;
		EmptyResult _ValueFromJSON(cJSON* json, StringView name)noexcept override;
		int64 _GetDynamicSize()const noexcept override;
		int64 _GetStaticSize()const noexcept override;
		ModificationEvent_t* GetOnModificationEvent()const noexcept override;
		const WGreaperLib& GetLibrary()const noexcept override;
		std::size_t GetValueHash()const noexcept override;
	};
}

namespace std
{
	template<>
	struct hash<greaper::IProperty>
	{
		NODISCARD INLINE size_t operator()(const greaper::IProperty& p)const noexcept
		{
			size_t seed = 0;
			HashCombine(seed, p.GetPropertyName());
			HashCombineHash(seed, p.GetValueHash());
			return seed;
		}
	};
	template<class T>
	struct hash<greaper::TProperty<T>>
	{
		NODISCARD INLINE size_t operator()(const greaper::TProperty<T>& p)const noexcept
		{
			return hash<greaper::IProperty>((const greaper::IProperty&)p);
		}
	};
}

#endif /* CORE_PROPERTY_H */