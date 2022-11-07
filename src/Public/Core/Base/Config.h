/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

/**
*	Enables/Diasbles the use of VERIFY macro in the project, even though
*	the Break(msg) function will still trigger.
*/
#ifndef GREAPER_ENABLE_BREAK
#if GREAPER_FRELEASE
#define GREAPER_ENABLE_BREAK 0
#else
#define GREAPER_ENABLE_BREAK 1
#endif
#endif

#ifndef GREAPER_DEBUG_ALLOCATION
#if GREAPER_FRELEASE
#define GREAPER_DEBUG_ALLOCATION 0
#else
#define GREAPER_DEBUG_ALLOCATION 1
#endif
#endif

#ifndef GREAPER_DEBUG_BREAK
#if GREAPER_FRELEASE
#define GREAPER_DEBUG_BREAK 0
#else
#define GREAPER_DEBUG_BREAK 1
#endif
#endif