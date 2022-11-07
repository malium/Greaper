/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN32_PSAPI_H
#define CORE_WIN32_PSAPI_H 1

#include "Win32Base.h"

#if WIN32_USE_GREAPER_HEADERS

typedef struct _MODULEINFO {
	LPVOID lpBaseOfDll;
	DWORD SizeOfImage;
	LPVOID EntryPoint;
} MODULEINFO, * LPMODULEINFO;

#if ARCHITECTURE_X64

#else

#endif

#else
#include <Psapi.h>
#endif

#endif /* CORE_WIN32_PSAPI_H */