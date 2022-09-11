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

	auto* async = (AsyncLogProp_t*)prop;
	if (m_Threaded == async->GetValue())
		return; // no change

	m_Threaded = async->GetValue();

	VerifyNot(m_Library.expired(), "Trying to change async LogManager, but its library has expired.");
	auto lib = m_Library.lock();
	auto wApp = lib->GetApplication();

	if (async->GetValue()) // init threaded mode
	{
		if (wApp.expired())
		{
			lib->LogError("Trying to enable async LogManager, but this library does not have an application connected.");
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
		auto app = wApp.lock();
		auto thmgrRes = app->GetActiveInterface(IThreadManager::InterfaceUUID);
		if (thmgrRes.HasFailed())
		{
			lib->LogError("Trying to enable async LogManager, but couldn't obtain a ThreadManager, reason: " + thmgrRes.GetFailMessage());
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
		auto thmgr = (PThreadManager)thmgrRes.GetValue();
		if (thmgr == nullptr)
		{
			lib->LogError("Trying to enable async LogManager, but couldn't obtain a ThreadManager.");
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
			lib->LogError("Trying to enable async LogManager, but couldn't create a Thread, reason: " + thRes.GetFailMessage());
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
		//m_AsyncThread = thRes.GetValue();
	}
	else // stop threaded mode
	{
		//if (m_AsyncThread == nullptr)
		//	return;

		if (wApp.expired())
		{
			lib->LogError("Trying to disable async LogManager, but this library does not have an application connected.");
			return;
		}
		auto app = wApp.lock();
		auto thmgrRes = app->GetActiveInterface(IThreadManager::InterfaceUUID);
		if (thmgrRes.HasFailed())
		{
			lib->LogError("Trying to enable async LogManager, but couldn't obtain a ThreadManager, reason: " + thmgrRes.GetFailMessage());
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
		auto thmgr = (PThreadManager)thmgrRes.GetValue();
		if (thmgr == nullptr)
		{
			lib->LogError("Trying to enable async LogManager, but couldn't obtain a ThreadManager, reason: " + thmgrRes.GetFailMessage());
			m_Threaded = false;
			async->SetValue(false, true);
			return;
		}
	}
}

void greaper::core::LogManager::RunFn()
{
	
}

void LogManager::OnInitialization() noexcept
{

}

void LogManager::OnDeinitialization() noexcept
{

}

void LogManager::OnActivation(SPtr<IInterface> oldDefault) noexcept
{
	UNUSED(oldDefault);
}

void LogManager::OnDeactivation(SPtr<IInterface> newDefault) noexcept
{
	UNUSED(newDefault);
}

void LogManager::InitProperties()noexcept
{
	if (m_Library.expired())
		return; // no base library weird

	auto lib = m_Library.lock();

	if (m_Properties.size() != (sizet)COUNT)
		m_Properties.resize((sizet)COUNT, WIProperty());

	WPtr<AsyncLogProp_t> asyncLogProp;
	auto result = lib->GetProperty(AsyncLogName);
	if (result.IsOk())
	{
		asyncLogProp = result.GetValue();
	}
	else
	{
		auto asyncLogResult = CreateProperty<bool>(m_Library, AsyncLogName, false, ""sv, false, true, nullptr);
		Verify(asyncLogResult.IsOk(), "Couldn't create the property '%s' msg: %s", AsyncLogName.data(), asyncLogResult.GetFailMessage().c_str());
		asyncLogProp = asyncLogResult.GetValue();
	}

	m_Properties[(sizet)AsyncProp] = asyncLogProp;
}

void LogManager::DeinitProperties()noexcept
{

}

void LogManager::InitSerialization() noexcept
{

}

void LogManager::DeinitSerialization() noexcept
{

}

LogManager::LogManager()
	:m_OnLogMessage("OnLogMessage"sv)
	,m_Threaded(false)
{

}

LogManager::~LogManager() noexcept
{

}

void LogManager::Log(LogLevel_t level, const String& message)noexcept
{

}

void LogManager::_Log(const LogData& data)noexcept
{

}