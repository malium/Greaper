/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

//#include "../IGreaperLibrary.h"
#include "../FileStream.h"

namespace greaper
{
	inline EmptyResult IGreaperLibrary::RegisterProperty(const PIProperty& property) noexcept
	{
		const auto nameIT = m_PropertyMap.find(property->GetPropertyName());
		if (nameIT != m_PropertyMap.end())
		{
			const auto index = nameIT->second;
			if (index < m_Properties.size())
			{
				const auto& oProp = m_Properties[index];
				if (oProp == nullptr)
				{
					m_Properties[index] = property;
					return Result::CreateSuccess();
				}
				return Result::CreateFailure(Format("Trying to register a Property '%s', but its already registered.", property->GetPropertyName().c_str()));
			}
			m_PropertyMap.erase(nameIT);
		}
		const auto index = m_Properties.size();
		m_Properties.push_back(property);
		m_PropertyMap.insert_or_assign(property->GetPropertyName(), index);
		return Result::CreateSuccess();
	}

	INLINE void IGreaperLibrary::Initialize() noexcept
	{
		/* No-op */
	}

	INLINE void IGreaperLibrary::Deinitialize() noexcept
	{
		/* No-op */
	}

	INLINE void IGreaperLibrary::AddManagers() noexcept
	{
		/* No-op */
	}

	INLINE void IGreaperLibrary::RegisterManagers() noexcept
	{
		auto libRes = m_Application->GetGreaperLibrary(GetLibraryUuid());
		Verify(libRes.IsOk(), "Couldn't get the GreaperLibrary %s from application.", GetLibraryName().data());
		auto lib = libRes.GetValue();
		for (const auto& mgr : m_Managers)
		{
			mgr->Initialize((WGreaperLib)lib);
			auto res = m_Application->RegisterInterface(mgr);
			if(res.HasFailed())
				LogWarning(res.GetFailMessage());
		}
	}

	INLINE void IGreaperLibrary::UnregisterManagers() noexcept
	{
		for(auto it = m_Managers.rbegin(); it != m_Managers.rend(); ++it)
		{
			auto& mgr = *it;
			EmptyResult res;
			if(mgr->IsActive())
			{
				res = m_Application->DeactivateInterface(mgr->GetInterfaceUUID());
				if(res.HasFailed())
					LogWarning(res.GetFailMessage());
			}
			res = m_Application->UnregisterInterface(mgr);
			if(res.HasFailed())
				LogWarning(res.GetFailMessage());
			
			mgr->Deinitialize();
			mgr.reset();
		}
	}

	INLINE void IGreaperLibrary::RemoveManagers() noexcept
	{
		m_Managers.clear();
	}

	INLINE void IGreaperLibrary::AddProperties() noexcept
	{
		for(const auto& mgr : m_Managers)
			mgr->InitProperties();
	}

	INLINE void IGreaperLibrary::RemoveProperties() noexcept
	{
		for(const auto& mgr : m_Managers)
			mgr->DeinitProperties();
		m_Properties.clear();
		m_PropertyMap.clear();
	}

	NODISCARD INLINE bool IGreaperLibrary::ShouldImportExportConfig() const noexcept
	{
		return true;
	}

	INLINE void IGreaperLibrary::ExportConfig() noexcept
	{
		const auto configPath = std::filesystem::current_path() / "Config";
		const auto configFileName = String{GetLibraryName()} + ".json";
		const auto configFilePath = configPath / configFileName;

		std::filesystem::create_directories(configPath);
		std::filesystem::remove(configFilePath);
		FileStream stream{ configFilePath, FileStream::READ | FileStream::WRITE };

		// Something went wrong
		if(!stream.IsWritable())
		{
			LogError("Something went wrong while creating the config file");
			return;
		}

		auto json = SPtr<cJSON>(cJSON_CreateObject(), cJSON_Delete);
		for (const auto& prop : m_Properties)
		{
			if(prop->IsStatic())
				continue; // Static are regenerated each library init, never stored
			refl::ComplexType<IProperty>::ToJSON(*prop, json.get(), prop->GetPropertyName());
		}
		auto text = SPtr<char>(cJSON_Print(json.get()));
		const auto textLength = strlen(text.get());
		auto written = stream.Write(text.get(), textLength);
		if(written != textLength)
		{
			LogWarning(Format("Something went wrong while writting the config file, TextLength:%" PRIiPTR " Written:%" PRIiPTR ".", textLength, written));
			return; // Added in case we need to expand this function
		}
	}

	INLINE void IGreaperLibrary::ImportConfig() noexcept
	{
		if(m_Properties.empty())
			return; // No config to import

		const auto configPath = std::filesystem::current_path() / "Config";
		const auto configFileName = String{GetLibraryName()} + ".json";
		const auto configFilePath = configPath / configFileName;

		std::filesystem::create_directories(configPath);
		FileStream stream{ configFilePath, FileStream::READ };
		// Could not be opened because it didn't exist
		if(!stream.IsReadable())
			return; 

		String fileTxt{};
		const auto fileLength = stream.Size();
		if(fileLength <= 0) 
		{ // Empty or something went wrong
			LogWarning("Config file could not be read, length <= 0.");
			return;
		}
		fileTxt.resize(fileLength);
		auto readAmount = stream.Read(fileTxt.data(), fileLength);
		stream.Close(); // Close the file as soon as possible
		if(readAmount != fileLength)
		{ // Couldn't read it all? Something went wrong
			LogWarning(Format("Something went wrong while reading the config file, FileSize:%" PRIiPTR " Read:%" PRIiPTR ".", fileLength, readAmount));
			return;
		}
		auto json = SPtr<cJSON>(cJSON_Parse(fileTxt.c_str()), cJSON_Delete);
		for(auto& prop : m_Properties)
		{
			if (prop->IsStatic())
				continue; // Static are regenerated each library init, never stored
			refl::ComplexType<IProperty>::FromJSON(*prop, json.get(), prop->GetPropertyName());
		}
	}

	INLINE void IGreaperLibrary::InitLibrary(PLibrary lib, PApplication app) noexcept
	{
		using namespace std::placeholders;

		VerifyEqual(m_InitializationState, InitState_t::Stopped, "Trying to initialize a library that is not fully stopped.");

		const auto [major, minor, patch, rev] = GetGreaperVersionValues(GetLibraryVersion());
		Log(Format("Initializing %s library ver. %d.%d.%d.%d...", GetLibraryName().data(), 
			major, minor, patch, rev));

		m_InitializationState = InitState_t::Starting;

		m_Library = std::move(lib);
		m_Application = std::move(app);

		Initialize();
		AddManagers();
		RegisterManagers();
		AddProperties();
		if(ShouldImportExportConfig())
			ImportConfig();

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

		Log(Format("%s has been initialized.", GetLibraryName().data()));
		m_InitializationState = InitState_t::Started;
	}

	INLINE void IGreaperLibrary::DeinitLibrary() noexcept
	{
		VerifyEqual(m_InitializationState, InitState_t::Started, "Trying to deinitialize a library that is not fully started.");

		Log(Format("Deinitializing %s...", GetLibraryName().data()));

		m_InitializationState = InitState_t::Stopping;

		if(ShouldImportExportConfig())
			ExportConfig();
		RemoveProperties();
		UnregisterManagers();
		RemoveManagers();
		Deinitialize();
		m_Application.reset();

		Log(Format("%s has been deinitialized.", GetLibraryName().data()));

		m_OnLogActivation.Disconnect();
		m_OnNewLog.Disconnect();
		m_LogActivated = false;
		m_LogManager.reset();

		m_InitializationState = InitState_t::Stopped;
	}

	inline WApplication IGreaperLibrary::GetApplication() const noexcept { return (WApplication)m_Application; }

	INLINE WLibrary IGreaperLibrary::GetOSLibrary() const noexcept { return (WLibrary)m_Library; }

	inline CSpan<PInterface> IGreaperLibrary::GetManagers() const noexcept { return CreateSpan(m_Managers); }

	inline CSpan<PIProperty> IGreaperLibrary::GetProperties() const noexcept { return CreateSpan(m_Properties); }

	inline TResult<WIProperty> IGreaperLibrary::GetProperty(const StringView& name) const noexcept
	{
		const auto nameIT = m_PropertyMap.find(name);
		if (nameIT == m_PropertyMap.end())
			return Result::CreateFailure<WIProperty>(Format("Couldn't find the property '%s' registered in the GreaperLibrary '%s'.", name.data(), GetLibraryName().data()));
		if (nameIT->second >= m_Properties.size())
			return Result::CreateFailure<WIProperty>(Format("The property '%s' registered in the GreaperLibrary '%s', is registered to an out of bounds index.", name.data(), GetLibraryName().data()));
		auto& prop = m_Properties[nameIT->second];
		return Result::CreateSuccess((WIProperty)prop);
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

	INLINE void IGreaperLibrary::OnNewLog(const PInterface& newInterface) noexcept
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

	INLINE void IGreaperLibrary::OnLogActivation(bool active, UNUSED IInterface* oldLog, const PInterface& newLog) noexcept
	{
		using namespace std::placeholders;

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
	
	INLINE bool IGreaperLibrary::IsInitialized() const noexcept { return m_InitializationState == InitState_t::Started; }
	
	INLINE InitState_t IGreaperLibrary::GetInitializationState() const noexcept { return m_InitializationState; }
}