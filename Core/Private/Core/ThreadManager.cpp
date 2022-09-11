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

void ThreadManager::OnInitialization() noexcept
{

}

void ThreadManager::OnDeinitialization() noexcept
{

}

void ThreadManager::OnActivation(SPtr<IInterface> oldDefault) noexcept
{

}

void ThreadManager::OnDeactivation(SPtr<IInterface> newDefault) noexcept
{

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
	SharedLock lock(m_ThreadMutex);

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
	return CreateResult<WThread>(thread);
}

Result<WThread> ThreadManager::GetThread(const String& threadName) const noexcept
{
	SharedLock lock(m_ThreadMutex);

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
	return CreateResult<WThread>(thread);
}

Result<PThread> ThreadManager::CreateThread(const ThreadConfig& config) noexcept
{
	return CreateFailure<PThread>("Not Implemented"sv);
}

void ThreadManager::DestroyThread(PThread thread) noexcept
{

}
