/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_THREAD_H
#define CORE_I_THREAD_H 1

#include "../Memory.h"

#if PLT_WINDOWS
#include "../Win/WinThreading.h"
#elif PLT_LINUX
#include "../Lnx/LnxThreading.h"
#endif
#include "../IThreadManager.h"

ENUMERATION(ThreadState, STOPPED, SUSPENDED, RUNNING, UNMANAGED);

namespace greaper
{
	struct ThreadConfig
	{
		std::function<void()> ThreadFN = nullptr;
		uint32 StackSize = 0;
		bool StartSuspended = false;
		bool JoinAtDestruction = true;
		StringView Name = "Unnamed"sv;
	};

	class IThread
	{
	public:
		virtual ~IThread() = default;

		virtual void Detach() = 0;

		virtual void Join() = 0;

		virtual bool Joinable()const noexcept = 0;

		virtual bool TryJoin() = 0;

		virtual ThreadHandle GetOSHandle()const noexcept = 0;

		virtual void Terminate() = 0;

		virtual void Resume() = 0;

		virtual ThreadID_t GetID()const noexcept = 0;

		virtual bool JoinsAtDestruction()const noexcept = 0;

		virtual const String& GetName()const noexcept = 0;
		
		virtual ThreadState_t GetState()const noexcept = 0;
	};
}
#if PLT_WINDOWS
#include "../Win/WinThreadImpl.inl"
#elif PLT_LINUX
#include "../Lnx/LnxThreadImpl.inl"
#endif

#endif /* CORE_I_THREAD_H */