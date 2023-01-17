/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef DISP_GLFW_WINDOW_H
#define DISP_GLFW_WINDOW_H 1

#include "ImplPrerequisites.h"
#include <Display/Base/IWindow.h>

struct GLFWwindow;

namespace greaper::disp
{
	class GLFWWindow : public IWindow
	{
		GLFWwindow* m_WindowHandle;
	public:


	};
}

#endif /* DISP_GLFW_WINDOW_H */