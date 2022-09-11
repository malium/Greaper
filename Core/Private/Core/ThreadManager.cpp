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

void ThreadManager::Initialize(WGreaperLib library)noexcept
{
	if (m_IsInitialized)
		return;

	m_Library = library;

	m_IsInitialized = true;
}

void ThreadManager::Deinitialize()noexcept
{
	if (!m_IsInitialized)
		return;

	m_IsInitialized = false;
}

void greaper::core::ThreadManager::OnActivate()noexcept
{
	if (m_IsActive)
		return;

	m_IsActive = true;
}

void greaper::core::ThreadManager::OnDeactivate()noexcept
{
	if (!m_IsActive)
		return;

	m_IsActive = false;
}

void greaper::core::ThreadManager::InitProperties()noexcept
{
}

void greaper::core::ThreadManager::DeinitProperties()noexcept
{
}

void greaper::core::ThreadManager::OnChangingDefault(WInterface newDefault)noexcept
{
}

greaper::core::ThreadManager::ThreadManager()
	:m_IsActive(false)
	,m_IsInitialized(false)
	,m_OnInitialization("OnInitialization"sv)
	,m_OnActivation("OnActivation"sv)
	,m_OnChangingDefault("OnChangingDefault"sv)
	,m_ThreadCreationEvent("OnNewThread"sv)
	,m_ThreadDestructionEvent("OnThreadFinish"sv)
{

}

greaper::core::ThreadManager::~ThreadManager() noexcept
{

}

Result<WThread> greaper::core::ThreadManager::GetThread(ThreadID_t id) const noexcept
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

Result<WThread> greaper::core::ThreadManager::GetThread(const String& threadName) const noexcept
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

Result<PThread> greaper::core::ThreadManager::CreateThread(const ThreadConfig& config) noexcept
{
	return CreateFailure<PThread>("Not Implemented"sv);
}

void greaper::core::ThreadManager::DestroyThread(PThread thread) noexcept
{

}

//Result<IThreadPool*> greaper::core::ThreadManager::GetThreadPool(const String& poolName) const
//{
//	return CreateFailure<IThreadPool*>("Not Implemented"sv);
//}
//
//Result<IThreadPool*> greaper::core::ThreadManager::CreateThreadPool(const ThreadPoolConfig& config)
//{
//	return CreateFailure<IThreadPool*>("Not Implemented"sv);
//}
//
//void greaper::core::ThreadManager::DestroyThreadPool(IThreadPool* pool)
//{
//}
