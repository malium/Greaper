/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "LogManager.h"
#include <Core/IApplication.h>

using namespace greaper;
using namespace core;

void LogManager::OnAsyncChanged(IProperty* prop)
{
	if (prop == nullptr)
		return;

	auto* async = (PropertyBool*)prop;
	if (m_Threaded == async->GetValue())
		return; // no change

	m_Threaded = async->GetValue();

	if (async->GetValue()) // init threaded mode
	{
		VerifyNotNull(m_Library, "");
		auto* app = m_Library->GetApplication();
		if (app == nullptr)
		{
			m_Library->LogError("Trying to enable async LogManager, but this library does not have an application connected.");
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
		auto thmgrRes = app->GetActiveInterface(IThreadManager::InterfaceUUID);
		if (thmgrRes.HasFailed())
		{
			m_Library->LogError("Trying to enable async LogManager, but couldn't obtain a ThreadManager, reason: " + thmgrRes.GetFailMessage());
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
		auto* thmgr = (IThreadManager*)thmgrRes.GetValue();
		if (thmgr == nullptr)
		{
			m_Library->LogError("Trying to enable async LogManager, but couldn't obtain a ThreadManager, reason: " + thmgrRes.GetFailMessage());
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
		ThreadConfig thcfg;
		thcfg.ThreadFN = std::bind(&LogManager::RunFn, this);
		thcfg.Name = "AsyncLogger";
		auto thRes = thmgr->CreateThread(thcfg);
		if (thRes.HasFailed())
		{
			m_Library->LogError("Trying to enable async LogManager, but couldn't create a Thread, reason: " + thRes.GetFailMessage());
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
		m_AsyncThread = thRes.GetValue();
	}
	else // stop threaded mode
	{
		if (m_AsyncThread == nullptr)
			return;

		VerifyNotNull(m_Library, "");
		auto* app = m_Library->GetApplication();
		if (app == nullptr)
		{
			m_Library->LogError("Trying to disable async LogManager, but this library does not have an application connected.");
			return;
		}
		auto thmgrRes = app->GetActiveInterface(IThreadManager::InterfaceUUID);
		if (thmgrRes.HasFailed())
		{
			m_Library->LogError("Trying to enable async LogManager, but couldn't obtain a ThreadManager, reason: " + thmgrRes.GetFailMessage());
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
		auto* thmgr = (IThreadManager*)thmgrRes.GetValue();
		if (thmgr == nullptr)
		{
			m_Library->LogError("Trying to enable async LogManager, but couldn't obtain a ThreadManager, reason: " + thmgrRes.GetFailMessage());
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
	}
}

void greaper::core::LogManager::RunFn()
{

}

void LogManager::Initialize(IGreaperLibrary* library)
{
	if (m_IsInitialized)
		return;

	m_Library = library;

	m_OnInitialization.Trigger(true);
	m_IsInitialized = true;
}

void LogManager::Deinitialize()
{
	if (!m_IsInitialized)
		return;

	m_IsInitialized = false;
}

void LogManager::OnActivate()
{
	if (m_IsActive)
		return;

	m_IsActive = true;
}

void LogManager::OnDeactivate()
{
	if (!m_IsActive)
		return;

	m_IsActive = false;
}

void LogManager::InitProperties()
{

}

void LogManager::DeinitProperties()
{

}

void LogManager::OnChangingDefault(IInterface* newDefault)
{

}

LogManager::LogManager()
	:m_Library(nullptr)
	,m_IsActive(false)
	,m_IsInitialized(false)
	,m_OnInitialization("OnInitialization"sv)
	,m_OnActivation("OnActivation"sv)
	,m_OnChangingDefault("OnChangingDefault"sv)
	,m_OnLogMessage("OnLogMessage"sv)
	,m_AsyncProp(nullptr)
	,m_Threaded(false)
{

}

LogManager::~LogManager() noexcept
{

}

void LogManager::Log(LogLevel_t level, const String& message)
{

}

void LogManager::_Log(const LogData& data)
{

}