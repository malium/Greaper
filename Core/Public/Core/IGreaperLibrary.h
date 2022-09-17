/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_GREAPER_LIBRARY_H
#define CORE_GREAPER_LIBRARY_H 1

#include "Library.h"

#include <utility>
#include "Uuid.h"
#include "Result.h"
#include "ILogManager.h"
#include "IApplication.h"
#include "Property.h"

namespace greaper
{
	class IInterface;
	class IGreaperLibrary
	{
	protected:
		EmptyResult RegisterProperty(const SPtr<IProperty>& property)noexcept;

		virtual void Initialize()noexcept = 0;

		virtual void InitManagers()noexcept = 0;

		virtual void InitProperties()noexcept = 0;

		virtual void InitReflection()noexcept = 0;

		virtual void DeinitReflection()noexcept = 0;

		virtual void DeinitProperties()noexcept = 0;

		virtual void DeinitManagers()noexcept = 0;

		virtual void Deinitialize()noexcept = 0;

	public:
		static constexpr Uuid LibraryUUID = Uuid{  };
		static constexpr StringView LibraryName = StringView{ "Unknown Greaper Library" };

		virtual ~IGreaperLibrary() = default;

		void InitLibrary(PLibrary newInterface, SPtr<IApplication> app)noexcept;

		void DeinitLibrary()noexcept;

		virtual const Uuid& GetLibraryUuid()const noexcept = 0;

		virtual const StringView& GetLibraryName()const noexcept = 0;

		WPtr<IApplication> GetApplication()const noexcept;

		WPtr<Library> GetOSLibrary()const noexcept;

		CRange<SPtr<IInterface>> GetManagers()const noexcept;

		CRange<SPtr<IProperty>> GetProperties()const noexcept;

		Result<WPtr<IProperty>> GetProperty(const StringView& name)const noexcept;

		virtual uint32 GetLibraryVersion()const noexcept = 0;

		bool IsInitialized()const noexcept;

		InitState_t GetInitializationState()const noexcept;

		void LogVerbose(const String& message)const noexcept;

		void Log(const String& message)const noexcept;

		void LogWarning(const String& message)const noexcept;

		void LogError(const String& message)const noexcept;

		void LogCritical(const String& message)const noexcept;

		template<class T, class _Alloc_>
		friend Result<SPtr<TProperty<T>>> CreateProperty(WPtr<IGreaperLibrary>, StringView, T, StringView,
			bool, bool, TPropertyValidator<T>*);

	private:
		PLibrary m_Library;
		mutable Vector<LogData> m_InitLogs;
		SPtr<ILogManager> m_LogManager;
		bool m_LogActivated = false;
		IApplication::OnInterfaceActivationEvent_t::HandlerType m_OnNewLog;
		IInterface::ActivationEvt_t::HandlerType m_OnLogActivation;
		InitState_t m_InitializationState = InitState_t::Stopped;

		void OnNewLog(const SPtr<IInterface>& active)noexcept;

		void OnLogActivation(bool active, IInterface* oldLog, const SPtr<IInterface>& newLog)noexcept;

		void DumpStoredLogs()noexcept;

	protected:
		SPtr<IApplication> m_Application;
		Vector<PInterface> m_Managers;
		UnorderedMap<StringView, sizet> m_PropertyMap;
		Vector<SPtr<IProperty>> m_Properties;
	};

	using PGreaperLib = SPtr<IGreaperLibrary>;
	using WGreaperLib = WPtr<IGreaperLibrary>;

	INLINE EmptyResult IGreaperLibrary::RegisterProperty(const SPtr<IProperty>& property) noexcept
	{
		const auto nameIT = m_PropertyMap.find(property->GetPropertyName());
		if (nameIT != m_PropertyMap.end())
		{
			const auto index = nameIT->second;
			if (index < m_Properties.size())
			{
				auto oProp = m_Properties[index];
				if (oProp == nullptr)
				{
					m_Properties[index] = property;
					return CreateEmptyResult();
				}
				return CreateEmptyFailure(Format("Trying to register a Property '%s', but its already registered.", property->GetPropertyName().c_str()));
			}
			m_PropertyMap.erase(nameIT);
		}
		const auto index = m_Properties.size();
		m_Properties.push_back(property);
		m_PropertyMap.insert_or_assign(property->GetPropertyName(), index);
		return CreateEmptyResult();
	}

	INLINE void IGreaperLibrary::InitLibrary(PLibrary lib, SPtr<IApplication> app) noexcept
	{
		VerifyEqual(m_InitializationState, InitState_t::Stopped, "Trying to initialize a library that is not fully stopped.");

		using namespace std::placeholders;

		m_InitializationState = InitState_t::Starting;

		m_Library = std::move(lib);
		m_Application = std::move(app);

		Initialize();
		InitManagers();
		InitProperties();
		InitReflection();

		if (m_Application != nullptr)
		{
			auto logMgrRes = m_Application->GetActiveInterface(ILogManager::InterfaceUUID);
			if (logMgrRes.IsOk())
			{
				m_LogManager = logMgrRes.GetValue();
			}
		}

		if (m_LogManager != nullptr)
		{
			m_LogManager->GetActivationEvent()->Connect(m_OnLogActivation,
														[this](bool active, IInterface* oldLog, const PInterface& newLog)
														{ OnLogActivation(active, oldLog, newLog); });
			m_LogActivated = m_LogManager->IsActive();
			if (m_LogActivated)
				DumpStoredLogs();
		}
		else
		{
			m_Application->GetOnInterfaceActivationEvent()->Connect(m_OnNewLog,
																	[this](const PInterface& newInterface)
																	{ OnNewLog(newInterface); });
		}

		m_InitializationState = InitState_t::Started;
	}

	INLINE void IGreaperLibrary::DeinitLibrary() noexcept
	{
		VerifyEqual(m_InitializationState, InitState_t::Started, "Trying to deinitialize a library that is not fully started.");

		m_InitializationState = InitState_t::Stopping;

		m_OnLogActivation.Disconnect();
		m_OnNewLog.Disconnect();
		m_LogActivated = false;
		m_LogManager.reset();

		DeinitReflection();
		DeinitProperties();
		DeinitManagers();
		Deinitialize();

		m_InitializationState = InitState_t::Stopped;
	}

	INLINE WPtr<IApplication> IGreaperLibrary::GetApplication() const noexcept { return (WApplication)m_Application; }

	INLINE WPtr<Library> IGreaperLibrary::GetOSLibrary() const noexcept { return (WLibrary)m_Library; }

	INLINE CRange<SPtr<IInterface>> IGreaperLibrary::GetManagers() const noexcept { return CreateRange(m_Managers); }

	INLINE CRange<SPtr<IProperty>> IGreaperLibrary::GetProperties() const noexcept { return CreateRange(m_Properties); }

	INLINE Result<WPtr<IProperty>> IGreaperLibrary::GetProperty(const StringView& name) const noexcept
	{
		const auto nameIT = m_PropertyMap.find(name);
		if (nameIT == m_PropertyMap.end())
			return CreateFailure<WIProperty>(Format("Couldn't find the property '%s' registered in the GreaperLibrary '%s'.", name.data(), GetLibraryName().data()));
		if (nameIT->second >= m_Properties.size())
			return CreateFailure<WIProperty>(Format("The property '%s' registered in the GreaperLibrary '%s', is registered to an out of bounds index.", name.data(), GetLibraryName().data()));
		auto prop = m_Properties[nameIT->second];
		return CreateResult((WIProperty)prop);
	}

	INLINE void IGreaperLibrary::Log(const String& message) const noexcept
	{
		if (m_LogActivated && m_LogManager != nullptr)
			m_LogManager->Log(LogLevel_t::INFORMATIVE, message, GetLibraryName());
		else
			m_InitLogs.push_back(LogData{ message, std::chrono::system_clock::now(), LogLevel_t::INFORMATIVE, GetLibraryName() });
	}

	INLINE void IGreaperLibrary::LogVerbose(const String& message) const noexcept
	{
		if (m_LogActivated && m_LogManager != nullptr)
			m_LogManager->Log(LogLevel_t::VERBOSE, message, GetLibraryName());
		else
			m_InitLogs.push_back(LogData{ message, std::chrono::system_clock::now(), LogLevel_t::VERBOSE, GetLibraryName() });
	}

	INLINE void IGreaperLibrary::LogWarning(const String& message) const noexcept
	{
		if (m_LogActivated && m_LogManager != nullptr)
			m_LogManager->Log(LogLevel_t::WARNING, message, GetLibraryName());
		else
			m_InitLogs.push_back(LogData{ message, std::chrono::system_clock::now(), LogLevel_t::WARNING, GetLibraryName() });
	}

	INLINE void IGreaperLibrary::LogError(const String& message) const noexcept
	{
		if (m_LogActivated && m_LogManager != nullptr)
			m_LogManager->Log(LogLevel_t::ERROR, message, GetLibraryName());
		else
			m_InitLogs.push_back(LogData{ message, std::chrono::system_clock::now(), LogLevel_t::ERROR, GetLibraryName() });
	}

	INLINE void IGreaperLibrary::LogCritical(const String& message) const noexcept
	{
		if (m_LogActivated && m_LogManager != nullptr)
			m_LogManager->Log(LogLevel_t::CRITICAL, message, GetLibraryName());
		else
			m_InitLogs.push_back(LogData{ message, std::chrono::system_clock::now(), LogLevel_t::CRITICAL, GetLibraryName() });
	}

	INLINE void IGreaperLibrary::OnNewLog(const SPtr<IInterface>& newInterface) noexcept
	{
		using namespace std::placeholders;

		if (newInterface == nullptr)
			return;

		if (newInterface->GetInterfaceUUID() == ILogManager::InterfaceUUID)
		{
			m_LogManager = newInterface;
			m_LogActivated = m_LogManager->IsActive();
			m_OnLogActivation.Disconnect();
			m_LogManager->GetActivationEvent()->Connect(m_OnLogActivation,
					[this](bool active, IInterface* oldLog, const PInterface& newLog)
					{ OnLogActivation(active, oldLog, newLog); });
			m_OnNewLog.Disconnect();
			if (m_LogActivated)
				DumpStoredLogs();
		}
	}

	INLINE void IGreaperLibrary::OnLogActivation(bool active, IInterface* oldLog, const SPtr<IInterface>& newLog) noexcept
	{
		using namespace std::placeholders;
		UNUSED(oldLog);

		if (!active) // a new log manager was activated or the current was deactivated
		{
			if (newLog != nullptr) // new log manager
			{
				m_LogManager = newLog;
				m_LogActivated = m_LogManager->IsActive();
				m_OnLogActivation.Disconnect();
				m_LogManager->GetActivationEvent()->Connect(m_OnLogActivation,
						[this](bool active, IInterface* oldLog, const PInterface& newLog)
						{ OnLogActivation(active, oldLog, newLog); });
				if (m_LogActivated)
					DumpStoredLogs();
			}
			else // the current log manager was deactivated
			{
				m_OnLogActivation.Disconnect();
				m_LogManager.reset();
				m_LogActivated = false;
				m_Application->GetOnInterfaceActivationEvent()->Connect(m_OnNewLog,
						[this](const PInterface& newInterface)
						{ OnNewLog(newInterface); });
			}
		}
		else // the current log manager was activated, we shouldn't arrive here
		{
			m_LogManager = newLog;
			m_LogActivated = m_LogManager->IsActive();
			m_OnLogActivation.Disconnect();
			m_LogManager->GetActivationEvent()->Connect(m_OnLogActivation,
					[this](bool active, IInterface* oldLog, const PInterface& newLog)
					{ OnLogActivation(active, oldLog, newLog); });
			if (m_LogActivated)
				DumpStoredLogs();
		}
	}

	INLINE void IGreaperLibrary::DumpStoredLogs() noexcept
	{
		for (const auto& log : m_InitLogs)
			m_LogManager->_Log(log);
		m_InitLogs.clear();
	}

	template<class T>
	class TGreaperLibrary : public IGreaperLibrary
	{
	public:
		const Uuid& GetLibraryUuid()const noexcept override { return T::LibraryUUID; }

		const StringView& GetLibraryName()const noexcept override { return T::LibraryName; }
	};

	template<class T>
	struct ValidGreaperLibrary
	{
		static_assert(std::is_base_of_v<IGreaperLibrary, T>, "Trying to validate a GreaperLibrary that does not derive from IGreaperLibrary");
		static constexpr bool UUIDValid = T::LibraryUUID != IGreaperLibrary::LibraryUUID;
		static constexpr bool NameValid = T::LibraryName != IGreaperLibrary::LibraryName;

		static constexpr bool Valid = UUIDValid && NameValid;
	};

	//// Property methods to avoid circle dependency
	template<class T, class _Alloc_>
	Result<SPtr<TProperty<T>>> CreateProperty(WPtr<IGreaperLibrary> library, StringView propertyName, T initialValue, StringView propertyInfo,
		bool isConstant, bool isStatic, TPropertyValidator<T>* validator)
	{
		auto lib = library.lock();
		if (lib == nullptr)
			return CreateFailure<PProperty<T>>("Couldn't create the property, expired library given"sv);

		auto* propPtr = Construct<TProperty<T>, _Alloc_>(library, propertyName, std::move(initialValue), propertyInfo, isConstant, isStatic, validator);
		auto prop = PProperty<T>(propPtr);

		const auto res = lib->RegisterProperty((PIProperty)prop);
		if (res.HasFailed())
		{
			//Destroy<TProperty<T>, _Alloc_>(property);
			return CreateFailure<PProperty<T>>("Couldn't register the property\n" + res.GetFailMessage());
		}
		return CreateResult(prop);
	}
	
	INLINE bool IGreaperLibrary::IsInitialized() const noexcept { return m_InitializationState == InitState_t::Started; }
	
	INLINE InitState_t IGreaperLibrary::GetInitializationState() const noexcept { return m_InitializationState; }
	
	template<class T>
	INLINE Result<WProperty<T>> GetProperty(const WPtr<IGreaperLibrary>& library, const String& name)
	{
		auto lib = library.lock();
		if (lib == nullptr)
			return CreateFailure<WProperty<T>>("Couldn't retrieve the property, expired library given.");

		auto res = lib->GetProperty(name);
		if (res.HasFailed())
			return CopyFailure<TProperty<T>*>(res);
		return CreateResult(reinterpret_cast<TProperty<T>*>(res.GetValue()));
	}

	template<class T>
	INLINE bool TProperty<T>::SetValue(const T& value, bool triggerEvent) noexcept
	{
		VerifyNot(m_Library.expired(), "Trying to set a value to a disconnected Property.");
		auto lib = m_Library.lock();
		if (m_Constant)
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
}

#endif /* CORE_GREAPER_LIBRARY_H */