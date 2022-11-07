/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_PLATFORM_H
#define CORE_PLATFORM_H 1

#include "CorePrerequisites.h"

#if PLT_WINDOWS
#include "Win/WinPlatform.h"
#elif PLT_LINUX
#include "Lnx/LnxPlatform.h"
#endif

#endif /* CORE_PLATFORM_H */
