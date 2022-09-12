/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "LogManager.h"
#include <Core/IApplication.h>

using namespace greaper;
using namespace core;

greaper::SPtr<greaper::core::LogManager> gLogManager = greaper::SPtr<greaper::core::LogManager>();

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

void LogManager::RunFn()
{
	while (m_Threaded)
	{
		auto lck = UniqueLock<Mutex>(m_QueueMutex);
		while (m_QueuedMessages.empty() || !m_Threaded)
			m_QueueSignal.wait(lck);

		if (!m_Threaded)
			break; // stop thread

		for (const auto& logData : m_QueuedMessages)
			LogToWriters(logData);

		m_QueuedMessages.clear();
	}
}

void LogManager::LogToWriters(const LogData& data)
{
	auto lck = Lock(m_WriterMutex);

	for (auto& writer : m_Writers)
	{
		if (writer == nullptr)
			continue;
		writer->WriteLog(data);
	}
}

void LogManager::OnInitialization() noexcept
{
	VerifyNot(m_Library.expired(), "Trying to initialize LogManager, but its library is expired.");
	auto lib = m_Library.lock();
	auto managers = lib->GetManagers();
	for (auto it = managers.begin(); it != managers.end(); ++it)
	{
		auto mgr = *it;
		if (mgr.get() == this)
		{
			gLogManager = mgr;
			break;
		}
	}
}

void LogManager::OnDeinitialization() noexcept
{
	gLogManager.reset();
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
	:m_Threaded(false)
{

}

LogManager::~LogManager() noexcept
{

}

void LogManager::AddLogWriter(SPtr<ILogWriter> writer) noexcept
{
	auto lck = Lock(m_WriterMutex);

	if (Contains(m_Writers, writer))
		return;

	writer->_Connect(gLogManager, m_Writers.size());
	m_Writers.push_back(writer);

	if (writer->WritePreviousMessages())
	{
		auto msgLck = Lock(m_MessagesMutex);
		for (const auto& logData : m_Messages)
			writer->WriteLog(logData);
	}
}

void LogManager::RemoveLogWriter(sizet writerID) noexcept
{
	auto lck = Lock(m_WriterMutex);

	if (writerID >= m_Writers.size())
		return;

	m_Writers[writerID].reset();
}

CRangeProtected<LogData, Mutex> LogManager::GetMessages() const noexcept
{
	return CreateRange(m_Messages, m_MessagesMutex);
}

void LogManager::Log(LogLevel_t level, const String& message, StringView libraryName)noexcept
{
	_Log(LogData{ message, Clock_t::now(), level, std::move(libraryName) });
}

void LogManager::_Log(const LogData& data)noexcept
{
	if (!m_Threaded)
	{
		LogToWriters(data);
	}
	else
	{
		m_QueueMutex.lock();
		m_QueuedMessages.push_back(data);
		m_QueueMutex.unlock();
		m_QueueSignal.notify_one();
	}

	{
		auto lck = Lock(m_MessagesMutex);
		m_Messages.push_back(data);
	}
}