/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

/***********************************************************************************
*                                 VERSION                                          *
***********************************************************************************/

/// Creates a 32 bit number from major, minor, patch and revision versions, each value has 8bit space.
#define VERSION_SETTER(major, minor, patch, rev) (((major & 0xFF) << 24) | ((minor & 0xFF) << 16) | ((patch & 0xFF) << 8) | (rev & 0xFF)) & 0xFFFFFFFF
#define VERSION_GET_MAJOR(version) ((version & 0xFF000000) >> 24) & 0xFF
#define VERSION_GET_MINOR(version) ((version & 0x00FF0000) >> 16) & 0xFF
#define VERSION_GET_PATCH(version) ((version & 0x0000FF00) >> 8) & 0xFF
#define VERSION_GET_REV(version) (version & 0x000000FF) & 0xFF

#ifdef GREAPER_PHAL_VERSION
#undef GREAPER_PHAL_VERSION
#endif
#define GREAPER_PHAL_VERSION VERSION_SETTER(1, 6, 1, 0)

/***********************************************************************************
*                            LANGUAJE DETECTION                                    *
***********************************************************************************/
#ifndef  __cplusplus
#error Greaper needs C++.
#endif

// We use at least C++17.
#if __cplusplus < 201703L
#if defined(_MSC_VER)
#if _MSC_VER < 1900
#error "Unsopported C++ version!"
#endif
#else
#error "Unsopported C++ version!"
#endif
#endif

#if (!defined(_68K_) && !defined(_MPPC_) && !defined(_X86_) && !defined(_IA64_) && !defined(_AMD64_) && !defined(_ARM_) && defined(_M_AMD64)) || defined(__x86_64__)
#define ARCHITECTURE_X64 1
#define ARCHITECTURE_X86 0
#else
#define ARCHITECTURE_X64 0
#define ARCHITECTURE_X86 1
#endif

// Currently we only support X64 architecture.
//#if !defined(GREAPER_ENABLE_X86) || (defined(GREAPER_ENABLE_X86) && GREAPER_ENABLE_X86 == 0)
//#if !ARCHITECTURE_X64
//#error "Unsopported Architecture!"
//#endif
//#endif

/***********************************************************************************
*                             COMPILER DETECTION                                   *
***********************************************************************************/

#ifndef COMPILER_CLANG
#if defined(__clang__)
#define COMPILER_CLANG 1
#define COMPILER_VERSION ((__clang_major__ * 10) + __clang_minor__)
#else
#define COMPILER_CLANG 0
#endif
#endif

#ifndef COMPILER_GCC
#if defined(__GNUC__) && !defined(COMPILER_VERSION)
#define COMPILER_GCC 1
#define COMPILER_VERSION __GNUC__
#else
#define COMPILER_GCC 0
#endif
#endif

#ifndef COMPILER_ICC
#if (defined(__INTEL_COMPILER) || defined(__ICC) || defined(__ECC) || defined(__ICL)) && !defined(COMPILER_VERSION)
#define COMPILER_ICC 1
#define COMPILER_VERSION __INTEL_COMPILER
#else
#define COMPILER_ICC 0
#endif
#endif

#ifndef COMPILER_MSVC
#if defined(_MSC_VER) && !defined(COMPILER_VERSION)
#define COMPILER_MSVC 1
#define COMPILER_VERSION _MSC_VER
#else
#define COMPILER_MSVC 0
#endif
#endif

#ifndef COMPILER_VERSION
#error "Unknown compiler!"
#endif

/***********************************************************************************
*                             PLATFORM DETECTION                                   *
***********************************************************************************/

#ifndef PLATFORM_WINDOWS
#if (defined(_WIN32) || defined(_WIN64) || defined(__WIN32__) || defined(__TOS_WIN__) || defined(__WINDOWS__))
#define PLT_WINDOWS 1
#else
#define PLT_WINDOWS 0
#endif
#endif

#ifndef PLT_LINUX
#if (defined(linux) || defined(__linux__) || defined(__linux))
#define PLT_LINUX 1
#else
#define PLT_LINUX 0
#endif
#endif

#if !PLT_WINDOWS && !PLT_LINUX
#error "Unsupported Platform!"
#endif

/* DEBUG MACRO */
#ifndef GREAPER_DEBUG
#if defined(_DEBUG) || defined(DEBUG)
#define GREAPER_DEBUG 1
#else
#define GREAPER_DEBUG 0
#endif
#endif
#ifndef GREAPER_FRELEASE // Full release, disable all non essencial checks!
#define GREAPER_FRELEASE 0
#endif

/* Greaper Library suffix name */
#define GREAPER_LIBRARY_SUFFIX_DBG "_Debug"
#define GREAPER_LIBRARY_SUFFIX_RLS "_Release"

#if GREAPER_FRELEASE
#define GREAPER_LIBSUFFIX ""
#else
#if GREAPER_DEBUG
#define GREAPER_LIBSUFFIX GREAPER_LIBRARY_SUFFIX_DBG
#else
#define GREAPER_LIBSUFFIX GREAPER_LIBRARY_SUFFIX_RLS
#endif
#endif

/** MSVC Compiler warnings disable */
#if COMPILER_MSVC
#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS 1
#endif
#endif

/* Standard C function calling convention */
#ifndef CDECL
#if COMPILER_CLANG || COMPILER_GCC
#define CDECL __attribute__((cdecl))
#elif COMPILER_ICC || COMPILER_MSVC
#define CDECL __cdecl
#else
#define CDECL
#endif
#endif

/* Standard calling convention */
#ifndef STDCALL
#if COMPILER_CLANG || COMPILER_GCC
#define STDCALL __attribute__((stdcall))
#elif COMPILER_ICC || COMPILER_MSVC
#define STDCALL __stdcall
#else
#define STDCALL
#endif
#endif

/* Thread local variable declaration */
#ifndef GREAPER_THLOCAL
#if COMPILER_MSVC || (COMPILER_ICC && PLT_WINDOWS)
#define GREAPER_THLOCAL __declspec(thread)
#else
#define GREAPER_THLOCAL __thread
#endif
#endif

#include "Base/BasicTypes.h"

#if PLT_WINDOWS
#include "Win/Prerequisites.h"
#elif PLT_LINUX
#include "Lnx/Prerequisites.h"
#endif

//#include "Base/BasicTypeInfo.h"

#ifndef PlatformAlloc
#define PlatformAlloc(bytes) ::malloc(bytes)
#define PlatformDealloc(mem) ::free(mem)
#endif
#ifndef PlatformAlignedAlloc
#define PlatformAlignedAlloc(bytes, alignment) ::aligned_alloc(alignment, bytes)
#define PlatformAlignedDealloc(mem) ::free(mem)
#endif

/***********************************************************************************
*                                    ENDIANESS                                     *
***********************************************************************************/
// Currently we just support LittleEndian
#ifndef PLATFORM_ENDIANESS
#define PLATFORM_LITTLE_ENDIAN 1
#define PLATFORM_BIG_ENDIAN 2
#define PLATFORM_ENDIANESS PLATFORM_LITTLE_ENDIAN
#endif

/***********************************************************************************
*                                      OTHER                                       *
***********************************************************************************/

/* Interface class definition (no virtual table) */
#ifndef NOVTABLE
#define NOVTABLE
#endif

/* Full function display */
#ifndef FUNCTION_FULL
#define FUNCTION_FULL
#endif
/* Dynamic Link Library extension */
#ifndef GREAPER_DLLEXT
#define GREAPER_DLLEXT ".so"
#endif
/* Dynamic Link library macro */
#ifndef DLLIMPORT
#define DLLIMPORT
#endif
#ifndef DLLEXPORT
#define DLLEXPORT
#endif
#ifdef __cplusplus
#define BEGIN_C extern "C" {
#define END_C }
#else
#define BEGIN_C
#define END_C
#endif
#ifndef FUNCTION_VARARGS_END
#define FUNCTION_VARARGS_END(fmtPlace, varArgsPlace) __attribute__((format(printf, fmtPlace, varArgsPlace)))
#endif
/* Force code to be inlined */
#ifndef INLINE
#define INLINE
#endif
/* Force code to NOT be inlined */
#ifndef NOINLINE
#define NOINLINE
#endif
/* Indicate that the function never returns. */
#ifndef FUNCTION_NO_RETURN_START
#define FUNCTION_NO_RETURN_START
#endif
/* Wrap a function signature in this to indicate that the function never returns. */
#ifndef FUNCTION_NO_RETURN_END
#define FUNCTION_NO_RETURN_END 
#endif
/* CPU cache line size, important to use in order to avoid cache misses */
#ifndef CACHE_LINE_SIZE
#define CACHE_LINE_SIZE 64
#endif
/* String constants */
#ifndef PATH_SEPARATOR
#define PATH_SEPARATOR '/'
#endif
#ifndef PATH_SEPARATOR_WIDE
#define PATH_SEPARATOR_WIDE L'/'
#endif
/* Unreferenced Parameter warnings removal */
#ifndef UNUSED
#define UNUSED(p) (p)
#endif
/* Alignment */
#ifndef ALIGN_BEGIN
#if COMPILER_MSVC || (COMPILER_CLANG && PLT_WINDOWS)
#define ALIGN_BEGIN(n) __declspec(align(n))
#else
#define ALIGN_BEGIN(n)
#endif
#endif
#ifndef ALIGN_END
#if (COMPILER_CLANG || COMPILER_GCC) && !COMPILER_MSVC
#define ALIGN_END(n) __attribute__((aligned(n)))
#else
#define ALIGN_END(n)
#endif
#endif
/* Compiler Debug breakpoint */
#ifndef TRIGGER_BREAKPOINT
#define TRIGGER_BREAKPOINT() 
#endif
/* Debug Console Output */
#ifndef DEBUG_OUTPUT
#define DEBUG_OUTPUT(x) std::cout << x << std::endl
#endif
/* High Performance Graphics enable utility */
#ifndef USE_HPG
#define USE_HPG()\
DLLEXPORT unsigned long NvOptimusEnablement = 1;\
DLLEXPORT int AmdPowerXpressRequestHighPerformance = 1
#endif

using FuncPtr = void(*)();

#include "Base/Config.h"
#include "Base/BasicTypeInfo.h"
#include "Base/Utils.h"
