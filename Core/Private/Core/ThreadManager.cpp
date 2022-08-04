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

void ThreadManager::Initialize(IGreaperLibrary* library)
{
	if (m_IsInitialized)
		return;

	m_Library = library;

	m_IsInitialized = true;
}

void ThreadManager::Deinitialize()
{
	if (!m_IsInitialized)
		return;

	m_IsInitialized = false;
}

void greaper::core::ThreadManager::OnActivate()
{
	if (m_IsActive)
		return;

	m_IsActive = true;
}

void greaper::core::ThreadManager::OnDeactivate()
{
	if (!m_IsActive)
		return;

	m_IsActive = false;
}

void greaper::core::ThreadManager::InitProperties()
{
}

void greaper::core::ThreadManager::DeinitProperties()
{
}

void greaper::core::ThreadManager::OnChangingDefault(IInterface* newDefault)
{
}

greaper::core::ThreadManager::ThreadManager()
{
}

greaper::core::ThreadManager::~ThreadManager() noexcept
{
}

Result<IThread*> greaper::core::ThreadManager::GetThread(ThreadID_t id) const
{
	return CreateFailure<IThread*>("Not Implemented"sv);
}

Result<IThread*> greaper::core::ThreadManager::GetThread(const String& threadName) const
{
	return CreateFailure<IThread*>("Not Implemented"sv);
}

Result<IThread*> greaper::core::ThreadManager::CreateThread(const ThreadConfig& config)
{
	return CreateFailure<IThread*>("Not Implemented"sv);
}

void greaper::core::ThreadManager::DestroyThread(IThread* thread)
{
}

Result<IThreadPool*> greaper::core::ThreadManager::GetThreadPool(const String& poolName) const
{
	return CreateFailure<IThreadPool*>("Not Implemented"sv);
}

Result<IThreadPool*> greaper::core::ThreadManager::CreateThreadPool(const ThreadPoolConfig& config)
{
	return CreateFailure<IThreadPool*>("Not Implemented"sv);
}

void greaper::core::ThreadManager::DestroyThreadPool(IThreadPool* pool)
{
}
