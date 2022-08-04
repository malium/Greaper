/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_THREAD_POOL_H
#define CORE_I_THREAD_POOL_H 1

#include "Task.h"

namespace greaper
{
	/*class IPooledThread;

	class HPooledTask
	{
		IPooledThread* m_Thread;
		uint32 m_ID;

		HPooledTask(IPooledThread* thread = nullptr, uint32 id = 0);

		friend class ThreadPool;

	public:
		void BlockUntilComplete() noexcept;
	};

	class IPooledThread
	{
	public:
		virtual ~IPooledThread() = default;

		virtual void Initialize(uint32 thID) = 0;

		virtual void Start(Task task, uint32 id) noexcept = 0;

		virtual void DestroyThread() = 0;

		virtual bool IsIdle() noexcept = 0;

		virtual Duration_t IdleTime() noexcept = 0;

		virtual uint32 GetID()const noexcept = 0;

		virtual void BlockUntilComplete() = 0;
	};

	struct ThreadPoolConfig
	{
		StringView Name = "unnamed"sv;
		uint32 DefaultCapacity = 1;
		uint32 MaxCapacity = 1;
		uint32 IdleTimeoutSeconds = 60;
	};

	class IThreadPool
	{
	public:
		virtual ~IThreadPool() = default;
		
		virtual HPooledTask RunTask(Task task) = 0;

		virtual void StopAll() = 0;

		virtual void ClearUnused() = 0;

		virtual uint32 GetAvailableThreadNum()const noexcept = 0;

		virtual uint32 GetActiveThreadNum()const noexcept = 0;

		virtual uint32 GetAllocatedThreadNum()const noexcept = 0;

		virtual const StringView& GetName()const noexcept = 0;
	};


	HPooledTask::HPooledTask(IPooledThread* thread, uint32 id) 
		:m_Thread(thread)
		,m_ID(id)
	{

	}


	void HPooledTask::BlockUntilComplete() noexcept
	{
		if(m_Thread == nullptr || m_Thread->GetID() != m_ID)
			return;

		m_Thread->BlockUntilComplete();
	}*/

	struct ThreadPoolConfig
	{
		StringView Name = "unnamed"sv;
		uint32 DefaultCapacity = 1;
		uint32 MaxCapacity = 1;
		uint32 IdleTimeoutSeconds = 60;
	};

	class IThreadPool;
}

#endif /* CORE_I_THREAD_POOL_H */