/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_APPLICATION_H
#define CORE_APPLICATION_H 1

#include "ImplPrerequisites.h"
#include <Core/IApplication.h>
#include <Core/Property.h>

namespace greaper::core
{
	//inline constexpr sizet StoredFrameTimeCount = 20;

	class Application final : public IApplication
	{
		enum PropertiesIndices
		{
			AppInstance,
			CommandLine,
			ApplicationName,
			ApplicationVersion,
			CompilationInfo,
			LoadedLibraries,
			/*FixedUpdateRate,
			UpdateMaxRate,*/

			COUNT
		};
		/*static constexpr uint32 MAX_ACCUM_FIXED_UPDATES = 200;
		static constexpr uint32 NEW_FIXED_UPDATES_PER_FRAME = 4;*/

		WGreaperLib m_Library;
		OnCloseEvent_t m_OnClose;
		mutable OnInterfaceActivationEvent_t m_OnInterfaceActivation;
		mutable InitializationEvt_t m_OnInitialization;
		mutable ActivationEvt_t m_OnActivation;

		Vector<WIProperty> m_Properties;

		bool m_IsActive;
		bool m_IsInitialized;
		bool m_HasToStop;

		struct LibInfo
		{
			PGreaperLib Lib;
			UnorderedMap<StringView, size_t> IntefaceNameMap;
			UnorderedMap<Uuid, size_t> InterfaceUuidMap;
			Vector<PInterface> Interfaces;
		};

		UnorderedMap<StringView, size_t> m_LibraryNameMap;
		UnorderedMap<Uuid, size_t> m_LibraryUuidMap;
		Vector<LibInfo> m_Libraries;

		mutable Mutex m_ActiveMutex;
		UnorderedMap<StringView, size_t> m_ActiveInterfaceNameMap;
		UnorderedMap<Uuid, size_t> m_ActiveInterfaceUuidMap;
		Vector<PInterface> m_ActiveInterfaces;

		Vector<PInterface> m_InterfaceToChange;
		Vector<PInterface> m_InterfacesToRemove;
		Vector<PInterface> m_InterfacesToAdd;

		/*Timepoint_t m_StartTime;
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
		IProperty::ModificationEventHandler_t m_OnFixedUpdateMaxRateEvtHnd;*/

		void AddGreaperLibrary(PGreaperLib library)noexcept;
		void LoadConfigLibraries()noexcept;

		/*void ClearFrameTimes()noexcept;
		void UpdateFrameTimes()noexcept;*/

		/*void OnUpdateMaxRateChange(IProperty* prop)
		{
			UNUSED(prop);
			const auto value = GetUpdateMaxRate()->GetValue();
			if (value == 0)
				m_UpdateStep = 0.f;
			else
				m_UpdateStep = 1.f / value;
		}
		void OnFixedUpdateRateChange(IProperty* prop)
		{
			UNUSED(prop);
			const auto value = GetFixedUpdateRate()->GetValue();
			if (value == 0)
				m_FixedUpdateStep = 0.f;
			else
				m_FixedUpdateStep = 1.f / value;
		}*/
		void UpdateActiveInterfaceList()noexcept;
		/*void UpdateTick();
		void ComputeFixedUpdateStep(uint64& step, uint32& iterations);*/

	public:
		Application();
		~Application();

		const Uuid& GetInterfaceUUID()const noexcept override { return InterfaceUUID; }

		const StringView& GetInterfaceName()const noexcept override { return InterfaceName; }

		WGreaperLib GetLibrary()const noexcept override { return m_Library; }

		void Initialize(WPtr<IGreaperLibrary> library)noexcept override;

		void Deinitialize()noexcept override;

		void OnActivate()noexcept override;

		void OnDeactivate()noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		bool IsActive()const noexcept override { return m_IsActive; }

		bool IsInitialized()const noexcept override { return m_IsInitialized; }

		/*void PreUpdate()override;

		void Update()override;

		void PostUpdate()override;

		void FixedUpdate()override;*/

		InitializationEvt_t* GetInitializationEvent()const noexcept override { return &m_OnInitialization; }

		ActivationEvt_t* GetActivationEvent()const noexcept override { return &m_OnActivation; }

		/*void SetConfig(ApplicationConfig config)override;

		const ApplicationConfig& GetConfig()const override { return m_Config; }*/

		void OnChangingDefault(WInterface newDefault)noexcept override
		{
			UNUSED(newDefault);
			Break("Cannot change the full application at runtime.");
		}

		ChangingDefaultEvt_t* GetChangingDefaultEvent()const noexcept override { return nullptr; }

		Result<SPtr<IGreaperLibrary>> RegisterGreaperLibrary(const WStringView& libPath)override;

		Result<SPtr<IGreaperLibrary>> GetGreaperLibrary(const StringView& libraryName)override;

		Result<SPtr<IGreaperLibrary>> GetGreaperLibrary(const Uuid& libraryUUID)override;

		EmptyResult UnregisterGreaperLibrary(SPtr<IGreaperLibrary> library)override;

		EmptyResult RegisterInterface(PInterface interface)override;

		EmptyResult UnregisterInterface(PInterface interface)override;

		EmptyResult ActivateInterface(PInterface interface)override;

		EmptyResult DeactivateInterface(const Uuid& interfaceUUID)override;

		EmptyResult DeactivateInterface(const StringView& interfaceName)override;

		Result<PInterface> GetActiveInterface(const Uuid& interfaceUUID)const  override;

		Result<PInterface> GetActiveInterface(const StringView& interfaceName)const override;

		Result<PInterface> GetInterface(const Uuid& interfaceUUID, const Uuid& libraryUUID)const override;

		Result<PInterface> GetInterface(const StringView& interfaceName, const StringView& libraryName)const override;

		Result<PInterface> GetInterface(const Uuid& interfaceUUID, const StringView& libraryName)const override;

		Result<PInterface> GetInterface(const StringView& interfaceName, const Uuid& libraryUUID)const override;

		/*bool AppHasToStop()const override { return m_HasToStop; }

		void StopApplication()override;

		OnCloseEvent_t* const GetOnCloseEvent()override { return &m_OnClose; }*/

		OnInterfaceActivationEvent_t* GetOnInterfaceActivationEvent()const noexcept { return &m_OnInterfaceActivation; }

		WPtr<ApplicationNameProp_t> GetApplicationName()const noexcept override { return m_Properties[(sizet)ApplicationName]; }

		WPtr<CompilationInfoProp_t> GetCompilationInfo()const noexcept override { return m_Properties[(sizet)CompilationInfo]; }

		WPtr<ApplicationVersionProp_t> GetApplicationVersion()const noexcept override { return m_Properties[(sizet)ApplicationVersion]; }

		WPtr<LoadedLibrariesProp_t> GetLoadedLibrariesNames()const noexcept override { return m_Properties[(sizet)LoadedLibraries]; }

		CRange<WPtr<IProperty>> GetProperties()const noexcept override { return CreateRange(m_Properties); }
		
		/*Timepoint_t GetStartTime()const override { return m_StartTime; }

		Timepoint_t GetLastUpdateTime()const override { return m_LastUpdateTime; }

		Timepoint_t GetLastFixedUpdateTime()const override { return m_LastFixedUpdateTime; }

		uint64 GetFrameCount()const override { return m_FrameCount; }

		float GetFixedUpdateDelta()const override { return m_FixedUpdateStep; }

		float GetUpdateDelta()const override { return m_LastUpdateDelta; }

		float GetAvgUpdateDelta()const override { return m_UpdateDeltaAvg; }

		float GetMinUpdateDelta()const override { return m_UpdateDeltaMin; }

		float GetMaxUpdateDelta()const override { return m_UpdateDeltaMax; }

		FixedUpdateRateProp_t* GetFixedUpdateRate() override { return (FixedUpdateRateProp_t*)m_Properties[(sizet)FixedUpdateRate]; }

		UpdateMaxRateProp_t* GetUpdateMaxRate() override { return (UpdateMaxRateProp_t*)m_Properties[(sizet)UpdateMaxRate]; }*/

		WPtr<CommandLineProp_t> GetCommandLine()const noexcept override { return m_Properties[(sizet)CommandLine]; }

		WPtr<AppInstanceProp_t> GetAppInstance()const noexcept override { return m_Properties[(sizet)AppInstance]; }
	};
}

#endif /* CORE_APPLICATION_H */
