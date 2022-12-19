/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_GREAPER_LIBRARY_H
#define CORE_GREAPER_LIBRARY_H 1

#include "Library.h"
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
		EmptyResult RegisterProperty(const PIProperty& property)noexcept;

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

		void InitLibrary(PLibrary lib, PApplication app)noexcept;

		void DeinitLibrary()noexcept;

		virtual const Uuid& GetLibraryUuid()const noexcept = 0;

		virtual const StringView& GetLibraryName()const noexcept = 0;

		WApplication GetApplication()const noexcept;

		WLibrary GetOSLibrary()const noexcept;

		CSpan<PInterface> GetManagers()const noexcept;

		CSpan<PIProperty> GetProperties()const noexcept;

		TResult<WIProperty> GetProperty(const StringView& name)const noexcept;

		virtual uint32 GetLibraryVersion()const noexcept = 0;

		bool IsInitialized()const noexcept;

		InitState_t GetInitializationState()const noexcept;

		void LogVerbose(const String& message)const noexcept;

		void Log(const String& message)const noexcept;

		void LogWarning(const String& message)const noexcept;

		void LogError(const String& message)const noexcept;

		void LogCritical(const String& message)const noexcept;

		template<class T, class _Alloc_>
		friend TResult<PProperty<T>> CreateProperty(WGreaperLib, StringView, T, StringView,
			bool, bool, TPropertyValidator<T>*);

	private:
		PLibrary m_Library;
		mutable Vector<LogData> m_InitLogs;
		PLogManager m_LogManager;
		bool m_LogActivated = false;
		IApplication::OnInterfaceActivationEvent_t::HandlerType m_OnNewLog;
		IInterface::ActivationEvt_t::HandlerType m_OnLogActivation;
		InitState_t m_InitializationState = InitState_t::Stopped;

		void OnNewLog(const PInterface& newInterface)noexcept;

		void OnLogActivation(bool active, UNUSED IInterface* oldLog, const PInterface& newLog)noexcept;

		void DumpStoredLogs()noexcept;

	protected:
		PApplication m_Application;
		Vector<PInterface> m_Managers;
		UnorderedMap<StringView, sizet> m_PropertyMap;
		Vector<PIProperty> m_Properties;
	};

	template<class T>
	class TGreaperLibrary : public IGreaperLibrary
	{
	public:
		const Uuid& GetLibraryUuid()const noexcept final { return T::LibraryUUID; }

		const StringView& GetLibraryName()const noexcept final { return T::LibraryName; }
	};

	template<class T>
	struct ValidGreaperLibrary
	{
		static_assert(std::is_base_of_v<IGreaperLibrary, T>, "Trying to validate a GreaperLibrary that does not derive from IGreaperLibrary");
		static constexpr bool UUIDValid = T::LibraryUUID != IGreaperLibrary::LibraryUUID;
		static constexpr bool NameValid = T::LibraryName != IGreaperLibrary::LibraryName;

		static constexpr bool Valid = UUIDValid && NameValid;
	};
}

#include "Base/IGreaperLibrary.inl"
//// Property methods to avoid circle dependency
#include "Base/Property.inl"
//// Interface methods to avoid circle dependency
#include "Base/IInterface.inl"
#include "Reflection/Property.h"

#endif /* CORE_GREAPER_LIBRARY_H */