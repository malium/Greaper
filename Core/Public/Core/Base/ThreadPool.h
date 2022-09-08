/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_THREAD_POOL_H
#define CORE_THREAD_POOL_H 1

#include "../Concurrency.h"
#include "Task.h"

namespace greaper
{
	struct ThreadPoolConfig
	{
		StringView Name = "unnamed"sv;
		uint32 DefaultCapacity = 1;
		uint32 MaxCapacity = 1;
		uint32 IdleTimeoutSeconds = 60;
	};

	class ThreadPool
	{

	};
}

#endif /* CORE_THREAD_POOL_H */