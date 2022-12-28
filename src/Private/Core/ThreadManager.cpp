/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "ThreadManager.h"
#include <Core/Base/IThread.h>

using namespace greaper;
using namespace greaper::core;

SPtr<ThreadManager> gThreadManager = {};

void ThreadManager::OnThreadDestruction(const PThread& thread) noexcept
{
	if (thread == nullptr || !IsActive())
		return; // thread was nullptr or manager not active

	LOCK(m_ThreadMutex);
	sizet threadIDX;
	if (const auto findIDIT = m_ThreadIDMap.find(thread->GetID());
		findIDIT != m_ThreadIDMap.end())
	{
		threadIDX = findIDIT->second;
	}
	else if (const auto findNameIT = m_ThreadNameMap.find(thread->GetName());
		findNameIT != m_ThreadNameMap.end())
	{
		threadIDX = findNameIT->second;
	}
	else
	{
		return; // Not found
	}

	m_Threads[threadIDX].reset();
}

void ThreadManager::OnInitialization() noexcept
{
	VerifyNot(m_Library.expired(), "Trying to initialize ThreadManager, but its library is expired.");
	auto lib = m_Library.lock();
	auto managers = lib->GetManagers();
	for(const auto& mgr : managers)
	{
		if (mgr.get() == this)
		{
			gThreadManager = mgr;
			break;
		}
	}
}

void ThreadManager::OnDeinitialization() noexcept
{
	gThreadManager.reset();
}

void ThreadManager::OnActivation(const PInterface& oldDefault) noexcept
{
	if (oldDefault != nullptr)
	{
		const auto& other = (const PThreadManager&)oldDefault;
		// Copy threads
		other->AccessThreads([this](CSpan<PThread> threads)
			{
				auto lck = Lock(m_ThreadMutex);
				const auto count = threads.GetSizeFn();
				if (m_Threads.capacity() < count)
					m_Threads.reserve(count);
				for (const auto& thread : threads)
				{
					if (thread == nullptr)
						continue;

					m_ThreadIDMap.insert_or_assign(thread->GetID(), m_Threads.size());
					m_ThreadNameMap.insert_or_assign(thread->GetName(), m_Threads.size());
					m_Threads.push_back(thread);
				}
			});
	}
	else
	{
		// Add Main Thread
		LOCK(m_ThreadMutex);
		auto curTh = PThread(AllocT<Thread>());
		new((void*)curTh.get())Thread((WThreadManager)gThreadManager, CUR_THHND(), CUR_THID(), "Main");

		m_ThreadNameMap.insert_or_assign(curTh->GetName(), m_Threads.size());
		m_ThreadIDMap.insert_or_assign(curTh->GetID(), m_Threads.size());
		m_Threads.push_back(curTh);
	}
	m_ThreadDestructionEvent.Connect(m_DestructionEventHnd, [this](const PThread& thread) {OnThreadDestruction(thread); });
}

void ThreadManager::OnDeactivation(UNUSED const PInterface& newDefault) noexcept
{
	m_DestructionEventHnd.Disconnect();

	// Clear threads
	LOCK(m_ThreadMutex);
	m_Threads.clear();
	m_ThreadIDMap.clear();
	m_ThreadNameMap.clear();
}

void ThreadManager::InitProperties()noexcept
{

}

void ThreadManager::DeinitProperties() noexcept
{
	
}

ThreadManager::ThreadManager()
	:m_ThreadCreationEvent("OnNewThread"sv)
	,m_ThreadDestructionEvent("OnThreadFinish"sv)
{

}

ThreadManager::~ThreadManager() noexcept
{

}

TResult<WThread> ThreadManager::GetThread(ThreadID_t id) const noexcept
{
	LOCK(m_ThreadMutex);

	const auto findIDIT = m_ThreadIDMap.find(id);
	if (findIDIT == m_ThreadIDMap.end())
	{
		return Result::CreateFailure<WThread>(Format("Cannot find the thread with ID: %d.", id));
	}
	const auto thIdx = findIDIT->second;
	const auto& thread = m_Threads[thIdx];
	if (thread == nullptr)
	{
		return Result::CreateFailure<WThread>(Format("Trying to get a thread with ID: %d, that is already finished.", id));
	}
	return Result::CreateSuccess((WThread)thread);
}

TResult<WThread> ThreadManager::GetThread(const String& threadName) const noexcept
{
	LOCK(m_ThreadMutex);

	const auto findNameIT = m_ThreadNameMap.find(threadName);
	if (findNameIT == m_ThreadNameMap.end())
	{
		return Result::CreateFailure<WThread>(Format("Cannot find the thread with name:'%s'.", threadName.c_str()));
	}
	const auto thIdx = findNameIT->second;
	const auto& thread = m_Threads[thIdx];
	if (thread == nullptr)
	{
		return Result::CreateFailure<WThread>(Format("Trying to get a thread with name:'%s', that is already finished.", threadName.c_str()));
	}
	return Result::CreateSuccess((WThread)thread);
}

TResult<PThread> ThreadManager::CreateThread(const ThreadConfig& config) noexcept
{
	LOCK(m_ThreadMutex);
	auto thread = PThread(AllocT<Thread>());
	new((void*)thread.get())Thread((WThreadManager)gThreadManager, thread, config);

	m_ThreadNameMap.insert_or_assign(thread->GetName(), m_Threads.size());
	m_ThreadIDMap.insert_or_assign(thread->GetID(), m_Threads.size());
	m_Threads.push_back(thread);
	return Result::CreateSuccess(thread);
}