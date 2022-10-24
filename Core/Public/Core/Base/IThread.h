/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_THREAD_H
#define CORE_I_THREAD_H 1

#include "../CorePrerequisites.h"
#include "../Enumeration.h"

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
}
#if PLT_WINDOWS
#include "../Win/WinThreadImpl.inl"
#elif PLT_LINUX
#include "../Lnx/LnxThreadImpl.inl"
#endif

#endif /* CORE_I_THREAD_H */