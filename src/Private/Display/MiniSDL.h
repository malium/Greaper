/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef DISP_MINI_SDL_H
#define DISP_MINI_SDL_H 1

#include <Core/CorePrerequisites.h>
#include <Core/IGreaperLibrary.h>
#include <External/SDL/SDL.h>
#include <External/SDL/SDL_vulkan.h>

namespace greaper::disp
{
	struct MiniSDL
	{
		int32 (*SDL_Init)(uint32 flags);
		void (*SDL_Quit)();
		const achar* (*SDL_GetError)();

		// Window
		SDL_Window* (*SDL_CreateWindow)(const achar* title, int32 x, int32 y, int32 w, int32 h, uint32 flags);
		void (*SDL_DestroyWindow)(SDL_Window* window);
		int32 (*SDL_PollEvent)(SDL_Event* event);
		void (*SDL_StartTextInput)();
		void (*SDL_StopTextInput)();
		uint32 (*SDL_GetMouseState)(int32* x, int32* y);
		const uint8* (*SDL_GetKeyboardState)(int32* numkeys);

		// GL
		SDL_GLContext (*SDL_GL_CreateContext)(SDL_Window* window);
		void (*SDL_GL_DeleteContext)(SDL_GLContext context);
		int32 (*SDL_GL_MakeCurrent)(SDL_Window* window, SDL_GLContext context);
		SDL_GLContext (*SDL_GL_GetCurrentContext)();
		void (*SDL_GL_SwapWindow)(SDL_Window* window);
		int32 (*SDL_GL_GetAttribute)(SDL_GLattr attr, int32* value);
		int32 (*SDL_GL_SetAttribute)(SDL_GLattr attr, int32 value);
		int32 (*SDL_GL_SetSwapInterval)(int32 interval);

		// Vulkan
		int32 (*SDL_Vulkan_LoadLibrary)(const achar* path);
		void (*SDL_Vulkan_UnloadLibrary)();
		SDL_bool (*SDL_Vulkan_GetInstanceExtensions)(SDL_Window* window, uint32* pCount, const achar** pNames);
		SDL_bool (*SDL_Vulkan_CreateSurface)(SDL_Window* window, VkInstance instance, VkSurfaceKHR* surface);
	};
}

#endif /* DISP_MINI_SDL_H */