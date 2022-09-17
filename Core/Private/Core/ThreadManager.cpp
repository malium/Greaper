/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "ThreadManager.h"

#if PLT_WINDOWS
#include <Core/Win/WinThreadImpl.inl>
#elif PLT_LINUX
#include <Core/Lnx/LnxThreadImpl.inl>
#endif

using namespace greaper;
using namespace greaper::core;

extern SPtr<ThreadManager> gThreadManager = SPtr<ThreadManager>();

#if PLT_WINDOWS
using ThreadImpl = WinThreadImpl;
#elif PLT_LINUX
using ThreadImpl = LnxThreadImp;
#endif

void ThreadManager::OnInitialization() noexcept
{
	VerifyNot(m_Library.expired(), "Trying to initialize ThreadManager, but its library is expired.");
	auto lib = m_Library.lock();
	auto managers = lib->GetManagers();
	for (auto it = managers.begin(); it != managers.end(); ++it)
	{
		auto mgr = *it;
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

void ThreadManager::OnActivation(const SPtr<IInterface>& oldDefault) noexcept
{
	if (oldDefault != nullptr)
	{
		const auto& other = (const PThreadManager&)oldDefault;
		// Copy threads
		auto lckTh = Lock(m_ThreadMutex);
		auto range = other->GetThreads();
		auto lckRng = Lock(range.Protection);
		const auto thCount = range.SizeFn();
		if (m_Threads.capacity() < thCount)
			m_Threads.reserve(thCount);

		for (auto it = range.begin(); it != range.end(); ++it)
		{
			const auto& thread = *it;
			if (thread == nullptr)
				continue;
			m_ThreadIDMap.insert_or_assign(thread->GetID(), m_Threads.size());
			m_ThreadNameMap.insert_or_assign(thread->GetName(), m_Threads.size());
			m_Threads.push_back(thread);
		}
	}
	else
	{
		// Add Main Thread
		LOCK(m_ThreadMutex);
		auto curTh = SPtr<ThreadImpl>(AllocT<ThreadImpl>());
		new((void*)curTh.get())ThreadImpl((WThreadManager)gThreadManager, CUR_THHND(), CUR_THID(), "Main");

		m_ThreadNameMap.insert_or_assign(curTh->GetName(), m_Threads.size());
		m_ThreadIDMap.insert_or_assign(curTh->GetID(), m_Threads.size());
		m_Threads.push_back((PThread)curTh);
	}
}

void ThreadManager::OnDeactivation(const SPtr<IInterface>& newDefault) noexcept
{
	UNUSED(newDefault);

	// Clear threads
	LOCK(m_ThreadMutex);
	m_Threads.clear();
	m_ThreadIDMap.clear();
	m_ThreadNameMap.clear();
}

void ThreadManager::InitProperties()noexcept
{

}

void ThreadManager::DeinitProperties()noexcept
{

}

void ThreadManager::InitSerialization() noexcept
{

}

void ThreadManager::DeinitSerialization() noexcept
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

Result<WThread> ThreadManager::GetThread(ThreadID_t id) const noexcept
{
	LOCK(m_ThreadMutex);

	const auto findIDIT = m_ThreadIDMap.find(id);
	if (findIDIT == m_ThreadIDMap.end())
	{
		return CreateFailure<WThread>(Format("Cannot find the thread with ID: %d.", id));
	}
	const auto thIdx = findIDIT->second;
	auto thread = m_Threads[thIdx];
	if (thread == nullptr)
	{
		return CreateFailure<WThread>(Format("Trying to get a thread with ID: %d, that is already finished.", id));
	}
	return CreateResult((WThread)thread);
}

Result<WThread> ThreadManager::GetThread(const String& threadName) const noexcept
{
	LOCK(m_ThreadMutex);

	const auto findNameIT = m_ThreadNameMap.find(threadName);
	if (findNameIT == m_ThreadNameMap.end())
	{
		return CreateFailure<WThread>(Format("Cannot find the thread with name:'%s'.", threadName.c_str()));
	}
	const auto thIdx = findNameIT->second;
	auto thread = m_Threads[thIdx];
	if (thread == nullptr)
	{
		return CreateFailure<WThread>(Format("Trying to get a thread with name:'%s', that is already finished.", threadName.c_str()));
	}
	return CreateResult((WThread)thread);
}

Result<PThread> ThreadManager::CreateThread(const ThreadConfig& config) noexcept
{
	LOCK(m_ThreadMutex);
	auto thread = SPtr<ThreadImpl>(AllocT<ThreadImpl>());
	new((void*)thread.get())ThreadImpl((WThreadManager)gThreadManager, (WThread)thread, config);

	m_ThreadNameMap.insert_or_assign(thread->GetName(), m_Threads.size());
	m_ThreadIDMap.insert_or_assign(thread->GetID(), m_Threads.size());
	m_Threads.push_back((PThread)thread);
	return CreateResult((PThread)thread);
}