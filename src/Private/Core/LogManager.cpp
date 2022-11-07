/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "LogManager.h"
#include "Application.h"

using namespace greaper;
using namespace core;

SPtr<LogManager> gLogManager = {};
extern SPtr<Application> gApplication;

void LogManager::OnAsyncChanged(IProperty* prop)
{
	if (prop == nullptr || !IsActive())
		return;

	auto* async = (AsyncLogProp_t*)prop;
	bool asyncVal = async->GetValueCopy();
	if (m_Threaded == asyncVal)
		return; // no change

	if (asyncVal)
		StartThreadMode();
	else
		StopThreadMode();
}

void LogManager::StartThreadMode()
{
#if LOGMANAGER_USE_MPMC
	m_Threaded = true;
#else
	VerifyNot(m_Library.expired(), "Trying to set as async LogManager, but its library has expired.");
	auto lib = m_Library.lock();

	auto thmgrRes = gApplication->GetActiveInterface(IThreadManager::InterfaceUUID);
	if (thmgrRes.HasFailed())
	{
		lib->LogError("Trying to set as async LogManager, but couldn't obtain a ThreadManager, reason: " + thmgrRes.GetFailMessage());
		m_Threaded = false;
		GetAsyncLog().lock()->SetValue(false, true);
		return;
	}
	auto thmgr = (PThreadManager)thmgrRes.GetValue();
	if (thmgr == nullptr)
	{
		lib->LogError("Trying to set as async LogManager, but couldn't obtain a ThreadManager.");
		m_Threaded = false;
		GetAsyncLog().lock()->SetValue(false, true);
		return;
	}

	ThreadConfig thcfg;
	thcfg.ThreadFN = [this]() { RunFn(); };
	thcfg.Name = "AsyncLogger"sv;
	
	m_Threaded = true;
	auto thRes = thmgr->CreateThread(thcfg);
	if (thRes.HasFailed())
	{
		lib->LogError("Trying to enable async LogManager, but couldn't create a Thread, reason: " + thRes.GetFailMessage());
		m_Threaded = false;
		GetAsyncLog().lock()->SetValue(false, true);
		return;
	}
	m_AsyncThread = thRes.GetValue();
#endif
}

void LogManager::StopThreadMode()
{
#if LOGMANAGER_USE_MPMC
	m_Threaded = false;
#else
	//uint64 count = 0;
	while (m_AsyncThread != nullptr)
	{
		//++count;
		m_QueueSignal.notify_all();
		if (m_AsyncThread->TryJoin())
			m_AsyncThread.reset();
		else
			THREAD_YIELD();
	}
	//DEBUG_OUTPUT((std::to_string(count) + '\n').c_str());
#endif
}

void LogManager::RunFn()
{
#if !LOGMANAGER_USE_MPMC
	while (m_Threaded)
	{
		auto lck = UniqueLock<decltype(m_QueueMutex)>(m_QueueMutex);
		while (m_QueuedMessages.empty() && m_Threaded)
			m_QueueSignal.wait(lck);

		for (const auto& logData : m_QueuedMessages)
			LogToWriters(logData);

		m_QueuedMessages.clear();

		if (!m_Threaded)
			break; // stop thread
	}
#endif
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
	for (const auto& mgr : managers)
	{
		if (mgr.get() == this)
		{
			gLogManager = mgr;
			break;
		}
	}

}

void LogManager::OnDeinitialization() noexcept
{
	if (m_Threaded)
	{
		m_Threaded = false;
#if !LOGMANAGER_USE_MPMC
		m_QueueSignal.notify_all();
		m_AsyncThread->Join();
#endif
	}
	{
		LOCK(m_WriterMutex);
		m_Writers.clear();
	}
	gLogManager.reset();
}

void LogManager::OnActivation(const PInterface& oldDefault) noexcept
{
	if (oldDefault != nullptr)
	{
		const auto& other = (const PLogManager&)oldDefault;
		
		other->AccessMessages([this](CSpan<LogData> messages)
			{
				auto lckMsg = Lock(m_MessagesMutex);
				const auto msgCount = messages.GetSizeFn();
				if (m_Messages.capacity() < msgCount)
					m_Messages.reserve(msgCount);
				for (const auto& msg : messages)
				{
					m_Messages.push_back(msg);
				}
			});
	}
	
#if LOGMANAGER_USE_MPMC
	auto thmgrRes = gApplication->GetActiveInterface(IThreadManager::InterfaceUUID);
	if (thmgrRes.IsOk())
	{
		m_Scheduler = MPMCTaskScheduler::Create((WThreadManager)thmgrRes.GetValue(), "AsyncLogger"sv, 1);
	}
	else
	{
		m_Scheduler.reset();
	}
#endif

	auto asyncProp = GetAsyncLog().lock();

	bool asyncVal = asyncProp->GetValueCopy();
	if (asyncVal)
	{
		StartThreadMode();
	}
}

void LogManager::OnDeactivation(UNUSED const PInterface& newDefault) noexcept
{
	// deinit async logging
	if (m_Threaded)
	{
		StopThreadMode();
#if LOGMANAGER_USE_MPMC
		m_Scheduler.reset();
#endif
	}
	// clear messages
	{
		LOCK(m_MessagesMutex);
		m_Messages.clear();
	}
}

void LogManager::InitProperties()noexcept
{
	if (m_Library.expired())
		return; // no base library weird

	auto lib = m_Library.lock();

	if (m_Properties.size() != (sizet)COUNT)
		m_Properties.resize((sizet)COUNT, WIProperty());

	WPtr<AsyncLogProp_t> asyncLogPropW;
	auto result = lib->GetProperty(AsyncLogName);
	if (result.IsOk())
	{
		asyncLogPropW = result.GetValue();
	}
	else
	{
		auto asyncLogResult = CreateProperty<bool>(m_Library, AsyncLogName, false, ""sv, false, true, nullptr);
		Verify(asyncLogResult.IsOk(), "Couldn't create the property '%s' msg: %s", AsyncLogName.data(), asyncLogResult.GetFailMessage().c_str());
		asyncLogPropW = (WPtr<AsyncLogProp_t>)asyncLogResult.GetValue();
	}

	auto asyncLogProp = asyncLogPropW.lock();
	m_Threaded = false;
	asyncLogProp->GetOnModificationEvent()->Connect(m_OnAsyncProp, [this](IProperty* prop) { OnAsyncChanged(prop); });

	m_Properties[(sizet)AsyncProp] = asyncLogPropW;
}

void LogManager::DeinitProperties()noexcept
{
	m_OnAsyncProp.Disconnect();

	for (auto& prop : m_Properties)
		prop.reset();
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
	LOCK(m_WriterMutex);

	if (Contains(m_Writers, writer))
		return;

	writer->_Connect((WLogManager)gLogManager, m_Writers.size());
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
	LOCK(m_WriterMutex);

	if (writerID >= m_Writers.size())
		return;

	m_Writers[writerID].reset();
}

void LogManager::Log(LogLevel_t level, const String& message, StringView libraryName)noexcept
{
	_Log(LogData{ message, std::chrono::system_clock::now(), level, libraryName });
}

void LogManager::_Log(const LogData& data)noexcept
{
	if (!m_Threaded)
	{
		LogToWriters(data);
	}
	else
	{
#if LOGMANAGER_USE_MPMC
		m_Scheduler->AddTask("LogTask", [data, this]()
			{ 
				LogToWriters(data);
			});
#else
		m_QueueMutex.lock();
		m_QueuedMessages.push_back(data);
		m_QueueMutex.unlock();
		m_QueueSignal.notify_one();
#endif
	}

	{
		auto lck = Lock(m_MessagesMutex);
		m_Messages.push_back(data);
	}
}