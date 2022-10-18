/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN32_UUID_H
#define CORE_WIN32_UUID_H 1

#include "Win32Base.h"

#if WIN32_USE_GREAPER_HEADERS

extern "C" {

#define RPCRTAPI DECLSPEC_IMPORT
typedef long RPC_STATUS;
#define  RPC_ENTRY __stdcall
#define __RPC_FAR

typedef struct _GUID {
	unsigned long  Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} GUID;

typedef GUID UUID;

RPCRTAPI
	RPC_STATUS
	RPC_ENTRY
	UuidCreate(
		UUID __RPC_FAR* Uuid
	);
}
#else
#include <rpc.h>
#endif

#endif /* CORE_WIN32_UUID_H */