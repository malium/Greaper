/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef DISP_PREREQUISITES_H
#define DISP_PREREQUISITES_H 1

#include <Core/CorePrerequisites.h>
#include <Math/MathPrerequisites.h>

#ifdef GREAPER_DISP_VERSION
#undef GREAPER_DISP_VERSION
#endif
#define GREAPER_DISP_VERSION VERSION_SETTER(0, 0, 1, 0)

namespace greaper::disp
{
	class IWindowManager; using PWindowManager = SPtr<IWindowManager>; using WWindowManager = WPtr<IWindowManager>;
	class IWindow; using PWindow = SPtr<IWindow>; using WWindow = WPtr<IWindow>;


	class Monitor; using PMonitor = SPtr<Monitor>; using WMonitor = WPtr<Monitor>;
	class VideoMode; using PVideoMode = SPtr<VideoMode>; using WVideoMode = WPtr<VideoMode>;
}

#endif /* DISP_PREREQUISITES_H */