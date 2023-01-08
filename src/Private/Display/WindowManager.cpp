/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "WindowManager.h"

using namespace greaper;
using namespace greaper::disp;

SPtr<WindowManager> gWindowManager = {};

void greaper::disp::WindowManager::OnInitialization() noexcept
{

}

void greaper::disp::WindowManager::OnDeinitialization() noexcept
{

}

void greaper::disp::WindowManager::OnActivation(const PInterface& oldDefault) noexcept
{

}

void greaper::disp::WindowManager::OnDeactivation(const PInterface& newDefault) noexcept
{

}

void greaper::disp::WindowManager::InitProperties() noexcept
{
}

void greaper::disp::WindowManager::DeinitProperties() noexcept
{

}

const DisplayAdapter& greaper::disp::WindowManager::GetMainDisplayAdapter() const
{
	static DisplayAdapter da{ Vector<Monitor>{}, Vector<VideoMode>{}, "", "", "", "", false };
	return da;
}

void greaper::disp::WindowManager::AccessDisplayAdapters(const std::function<void(CSpan<DisplayAdapter>)>& accessFn) const
{
}

TResult<PWindow> greaper::disp::WindowManager::CreateWindow(const WindowDesc& desc)
{
	return Result::CreateFailure<PWindow>(""sv);
}

TResult<PWindow> greaper::disp::WindowManager::GetWindow(const String& windowID) const
{
	return Result::CreateFailure<PWindow>(""sv);
}

void greaper::disp::WindowManager::AccessWindows(const std::function<void(CSpan<PWindow>)>& accessFn) const
{

}