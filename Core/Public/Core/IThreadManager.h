/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_THREAD_MANAGER_H
#define CORE_I_THREAD_MANAGER_H 1

#include "CorePrerequisites.h"
#include "Interface.h"
#include "Base/IThread.h"
//#include "Base/ThreadPool.h"
//#include "Base/IThreadPool.h"
#include "Result.h"
#include "Event.h"

namespace greaper
{
	class IThreadManager : public IInterface
	{
	public:
		static constexpr Uuid InterfaceUUID = Uuid{ 0x284B5BAD, 0x9B004E76, 0x8B7F76DD, 0xB45D740F };
		static constexpr StringView InterfaceName = "ThreadManager"sv;

		using ThreadCreationEvent_t = Event<PThread>;
		using ThreadDestructionEvent_t = Event<PThread>;

		virtual ~IThreadManager()noexcept = default;

		virtual Result<WThread> GetThread(ThreadID_t id)const noexcept = 0;

		virtual Result<WThread> GetThread(const String& threadName)const noexcept = 0;

		virtual Result<PThread> CreateThread(const ThreadConfig& config)noexcept = 0;

		virtual void DestroyThread(PThread thread)noexcept = 0;

		virtual ThreadCreationEvent_t* GetThreadCreationEvent()const noexcept = 0;

		virtual ThreadDestructionEvent_t* GetThreadDestructionEvent()const noexcept = 0;

		virtual CRange<PThread> GetThreads()const noexcept = 0;
	};

	using WThreadManager = WPtr<IThreadManager>;
	using PThreadManager = SPtr<IThreadManager>;
}

#endif /* CORE_I_THREAD_MANAGER_H */