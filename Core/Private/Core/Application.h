/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H 1

#include "ImplPrerequisites.h"
#include <Core/IApplication.h>
#include <Core/Event.h>

namespace greaper::core
{
	inline constexpr sizet StoredFrameTimeCount = 20;

	class Application final : public IApplication
	{
		enum PropertiesIndices
		{
			ApplicationName,
			ApplicationVersion,
			CompilationInfo,
			LoadedLibraries,
			FixedUpdateMaxRate,
			UpdateMaxRate,

			COUNT
		};
		static constexpr uint32 MAX_ACCUM_FIXED_UPDATES = 200;
		static constexpr uint32 NEW_FIXED_UPDATES_PER_FRAME = 4;

		IGreaperLibrary* m_Library;
		OnCloseEvent_t m_OnClose;
		OnInterfaceActivationEvent_t m_OnInterfaceActivation;
		InitializationEvt_t m_OnInitialization;
		ActivationEvt_t m_OnActivation;

		Vector<IProperty*> m_Properties;

		bool m_IsActive;
		bool m_IsInitialized;
		bool m_HasToStop;

		struct LibInfo
		{
			IGreaperLibrary* Lib = nullptr;
			UnorderedMap<StringView, size_t> IntefaceNameMap;
			UnorderedMap<Uuid, size_t> InterfaceUuidMap;
			Vector<IInterface*> Interfaces;
		};

		UnorderedMap<StringView, size_t> m_LibraryNameMap;
		UnorderedMap<Uuid, size_t> m_LibraryUuidMap;
		Vector<LibInfo> m_Libraries;

		mutable Mutex m_ActiveMutex;
		UnorderedMap<StringView, size_t> m_ActiveInterfaceNameMap;
		UnorderedMap<Uuid, size_t> m_ActiveInterfaceUuidMap;
		Vector<IInterface*> m_ActiveInterfaces;

		Vector<IInterface*> m_InterfaceToChange;
		Vector<IInterface*> m_InterfacesToRemove;
		Vector<IInterface*> m_InterfacesToAdd;

		Timepoint_t m_StartTime;
		Timepoint_t m_LastUpdateTime;
		Timepoint_t m_LastFixedUpdateTime;
		uint64 m_FrameCount;
		float m_UpdateStep;
		std::chrono::nanoseconds m_UpdateStepNanos;
		float m_FixedUpdateStep;
		std::chrono::nanoseconds m_FixedUpdateStepNanos;
		float m_LastUpdateDelta;
		float m_FrameTimes[StoredFrameTimeCount];
		float m_UpdateDeltaAvg;
		float m_UpdateDeltaMin;
		float m_UpdateDeltaMax;
		uint32 m_RemainingFixedUpdates;
		IProperty::ModificationEventHandler_t m_OnUpdateMaxRateEvtHnd;
		IProperty::ModificationEventHandler_t m_OnFixedUpdateMaxRateEvtHnd;

		void AddGreaperLibrary(IGreaperLibrary* library);
		void LoadConfigLibraries();

		void ClearFrameTimes()noexcept;
		void UpdateFrameTimes()noexcept;

		void OnUpdateMaxRateChange(IProperty* prop)
		{
			UNUSED(prop);
			const auto value = GetUpdateMaxRate()->GetValue();
			if (value == 0)
				m_UpdateStep = 0.f;
			else
				m_UpdateStep = 1.f / value;
		}
		void OnFixedUpdateMaxRateChange(IProperty* prop)
		{
			UNUSED(prop);
			const auto value = GetFixedUpdateMaxRate()->GetValue();
			if (value == 0)
				m_FixedUpdateStep = 0.f;
			else
				m_FixedUpdateStep = 1.f / value;
		}
		void UpdateActiveInterfaceList();
		void UpdateTick();
		void ComputeFixedUpdateStep(uint64& step, uint32& iterations);

	public:
		Application();
		~Application();

		const Uuid& GetInterfaceUUID()const override { return InterfaceUUID; }

		const StringView& GetInterfaceName()const override { return InterfaceName; }

		IGreaperLibrary* GetLibrary()const override { return m_Library; }

		void Initialize(IGreaperLibrary* library)override;

		void Deinitialize()override;

		void OnActivate()override;

		void OnDeactivate()override;

		void InitProperties()override;

		void DeinitProperties()override;

		bool IsActive()const override { return m_IsActive; }

		bool IsInitialized()const override { return m_IsInitialized; }

		void PreUpdate()override;

		void Update()override;

		void PostUpdate()override;

		void FixedUpdate()override;

		InitializationEvt_t* const GetInitializationEvent()override { return &m_OnInitialization; }

		ActivationEvt_t* const GetActivationEvent()override { return &m_OnActivation; }

		/*void SetConfig(ApplicationConfig config)override;

		const ApplicationConfig& GetConfig()const override { return m_Config; }*/

		void OnChangingDefault(IInterface* newDefault)override
		{
			UNUSED(newDefault);
			Break("Cannot change the full application at runtime.");
		}

		ChangingDefaultEvt_t* const GetChangingDefaultEvent() { return nullptr; }

		Result<IGreaperLibrary*> RegisterGreaperLibrary(const WStringView& libPath)override;

		Result<IGreaperLibrary*> GetGreaperLibrary(const StringView& libraryName)override;

		Result<IGreaperLibrary*> GetGreaperLibrary(const Uuid& libraryUUID)override;

		EmptyResult UnregisterGreaperLibrary(IGreaperLibrary* library)override;

		EmptyResult RegisterInterface(IInterface* interface)override;

		EmptyResult UnregisterInterface(IInterface* interface)override;

		EmptyResult ActivateInterface(IInterface* interface)override;

		EmptyResult DeactivateInterface(const Uuid& interfaceUUID)override;

		EmptyResult DeactivateInterface(const StringView& interfaceName)override;

		Result<IInterface*> GetActiveInterface(const Uuid& interfaceUUID)const  override;

		Result<IInterface*> GetActiveInterface(const StringView& interfaceName)const override;

		Result<IInterface*> GetInterface(const Uuid& interfaceUUID, const Uuid& libraryUUID)const override;

		Result<IInterface*> GetInterface(const StringView& interfaceName, const StringView& libraryName)const override;

		Result<IInterface*> GetInterface(const Uuid& interfaceUUID, const StringView& libraryName)const override;

		Result<IInterface*> GetInterface(const StringView& interfaceName, const Uuid& libraryUUID)const override;

		bool AppHasToStop()const override { return m_HasToStop; }

		void StopApplication()override;

		OnCloseEvent_t* const GetOnCloseEvent()override { return &m_OnClose; }

		OnInterfaceActivationEvent_t* const GetOnInterfaceActivationEvent() { return &m_OnInterfaceActivation; }

		ApplicationNameProp_t* GetApplicationName()override { return reinterpret_cast<ApplicationNameProp_t*>(m_Properties[(sizet)ApplicationName]); }

		CompilationInfoProp_t* GetCompilationInfo()override { return reinterpret_cast<CompilationInfoProp_t*>(m_Properties[(sizet)CompilationInfo]); }

		ApplicationVersionProp_t* GetApplicationVersion()override { return reinterpret_cast<ApplicationVersionProp_t*>(m_Properties[(sizet)ApplicationVersion]); }

		LoadedLibrariesProp_t* GetLoadedLibrariesNames()override { return reinterpret_cast<LoadedLibrariesProp_t*>(m_Properties[(sizet)LoadedLibraries]); }

		CRange<IProperty*> GetProperties() const override { return CreateRange(m_Properties); }
		
		Timepoint_t GetStartTime()const override { return m_StartTime; }

		Timepoint_t GetLastUpdateTime()const override { return m_LastUpdateTime; }

		Timepoint_t GetLastFixedUpdateTime()const override { return m_LastFixedUpdateTime; }

		uint64 GetFrameCount()const override { return m_FrameCount; }

		float GetUpdateDelta()const override { return m_LastUpdateDelta; }

		float GetAvgUpdateDelta()const override { return m_UpdateDeltaAvg; }

		float GetMinUpdateDelta()const override { return m_UpdateDeltaMin; }

		float GetMaxUpdateDelta()const override { return m_UpdateDeltaMax; }

		FixedUpdateMaxRateProp_t* GetFixedUpdateMaxRate() override { return (FixedUpdateMaxRateProp_t*)m_Properties[(sizet)FixedUpdateMaxRate]; }

		UpdateMaxRateProp_t* GetUpdateMaxRate() override { return (UpdateMaxRateProp_t*)m_Properties[(sizet)UpdateMaxRate]; }
	};
}

#endif /* CORE_APPLICATION_H */
