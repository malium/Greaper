/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "WindowManager.h"

using namespace greaper;
using namespace greaper::disp;

SPtr<WindowManager> gWindowManager = {};

void WindowManager::QueryMonitors()
{

}

void WindowManager::OnInitialization() noexcept
{

}

void WindowManager::OnDeinitialization() noexcept
{

}

void WindowManager::OnActivation(const PInterface& oldDefault) noexcept
{
	const auto& prev = (const PWindowManager&)oldDefault;
	m_Monitors.clear();
	m_MainMonitor = 0;
	if(prev != nullptr)
	{
		prev->AccessMonitors([this](CSpan<SPtr<Monitor>> span)
		{
			const auto size = span.GetSizeFn();
			if(m_Monitors.size() < size)
				m_Monitors.reserve(size);
			for(const auto& elem : span)
			{
				if(elem->IsPrimary())
					m_MainMonitor = m_Monitors.size();
				m_Monitors.push_back(elem);
			}
		});
	}
	else
	{
		QueryMonitors();
	}
}

void WindowManager::OnDeactivation(const PInterface& newDefault) noexcept
{

}

void WindowManager::InitProperties() noexcept
{
}

void WindowManager::DeinitProperties() noexcept
{

}

SPtr<Monitor> WindowManager::GetMainMonitor() const
{
	LOCK(m_MonitorMutex);
	if(m_Monitors.size() > m_MainMonitor)
		return m_Monitors[m_MainMonitor];
	return SPtr<Monitor>();
}

void WindowManager::AccessMonitors(const std::function<void(CSpan<SPtr<Monitor>>)>& accessFn) const
{
	LOCK(m_MonitorMutex);
	accessFn(CreateSpan(m_Monitors));
}

TResult<PWindow> WindowManager::CreateWindow(const WindowDesc& desc)
{
	return Result::CreateFailure<PWindow>(""sv);
}

TResult<PWindow> WindowManager::GetWindow(const String& windowID) const
{
	return Result::CreateFailure<PWindow>(""sv);
}

void WindowManager::AccessWindows(const std::function<void(CSpan<PWindow>)>& accessFn) const
{

}