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
{
}

greaper::core::ThreadManager::~ThreadManager() noexcept
{
}

Result<WThread> greaper::core::ThreadManager::GetThread(ThreadID_t id) const noexcept
{
	return CreateFailure<WThread>("Not Implemented"sv);
}

Result<WThread> greaper::core::ThreadManager::GetThread(const String& threadName) const noexcept
{
	return CreateFailure<WThread>("Not Implemented"sv);
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
