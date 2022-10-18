/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_WINDOW_MANAGER_H
#define CORE_I_WINDOW_MANAGER_H 1

#include "Interface.h"
#include "Base/IWindow.h"
#include "Result.h"
#include "Base/DisplayAdapter.h"

namespace greaper
{
	class IWindowManager : public IInterface
	{
	public:
		static constexpr Uuid InterfaceUUID = Uuid{ 0x9FFF5170, 0x9A5E4E9C, 0x9CFD0FAB, 0xBF2C3E75 };
		static constexpr StringView InterfaceName = "WindowManager"sv;

		virtual ~IWindowManager()noexcept = default;

		virtual TResult<IWindow*> CreateWindow(const WindowDesc& desc = {}) = 0;

		virtual EmptyResult DestroyWindow(IWindow* window) = 0;

		virtual TResult<IWindow*> FindWindow(const WString& title)const = 0;

		virtual const Vector<IWindow*>& GetWindows()const = 0;

		virtual const Vector<DisplayAdapter>& GetAdpaters()const = 0;

		virtual const DisplayAdapter& GetDefaultDisplayAdapter()const = 0;
	};

	using WWindowManager = WPtr<IWindowManager>;
	using PWindowManager = SPtr<IWindowManager>;
}

#endif /* CORE_I_WINDOW_MANAGER_H */