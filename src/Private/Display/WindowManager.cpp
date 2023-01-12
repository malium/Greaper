/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "WindowManager.h"
#define GLFW_DLL
#include <External/GLFW/glfw3.h>

using namespace greaper;
using namespace greaper::disp;

SPtr<WindowManager> gWindowManager = {};

void WindowManager::QueryMonitors()
{
	Verify(!m_Library.expired(), "GreaperLibrary attached was expired!");
	auto lib = m_Library.lock();

	int32 monitorCount;
	auto** monitors = glfwGetMonitors(&monitorCount);
	LOCK(m_MonitorMutex);
	m_Monitors.clear();
	m_Monitors.resize(monitorCount, PMonitor());
	m_MainMonitor = 0;
	const char* errorMsg = nullptr;
	float hdpi, vdpi;
	int32 workX, workY, workW, workH;
	int32 monX, monY;
	int32 videoModeCount;
	for (int32 i = 0; i < monitorCount; ++i)
	{
		auto& monitorPtr = m_Monitors[i];
		auto* monitorInfo = monitors[i];
		String name{};

		auto nameRet = glfwGetMonitorName(monitorInfo);
		if (nameRet != nullptr)
			name.assign(nameRet);
		else
			lib->LogWarning(Format("Couldn't retrive the monitor name, error code: %" PRIi32 " error msg: %s.", glfwGetError(&errorMsg), errorMsg));

		glfwGetMonitorContentScale(monitorInfo, &hdpi, &vdpi);

		glfwGetMonitorWorkarea(monitorInfo, &workX, &workY, &workW, &workH);

		glfwGetMonitorPos(monitorInfo, &monX, &monY);
		
		const auto* videoModeInfos = glfwGetVideoModes(monitorInfo, &videoModeCount);

		Vector<PVideoMode> videoModes{};
		videoModes.resize(videoModeCount, PVideoMode());

		const auto* currentVideoMode = glfwGetVideoMode(monitorInfo);

		monitorPtr.reset(AllocT<Monitor>());

		int32 primaryVideoModeIndex = -1;

		int32 workWDiff, workHDiff;
		workWDiff = workHDiff = std::numeric_limits<int32>::max();
		int32 closestVideoModeIndex = -1;
		PVideoMode closestVideoMode{};
		int32 highestBits = 0;
		int32 highestRefresh = 0;

		for (int32 j = 0; j < videoModeCount; ++j)
		{
			auto& videoModePtr = videoModes[j];
			const auto& videoModeInfo = videoModeInfos[j];

			if (primaryVideoModeIndex < 0)
			{
				if (CompareMemory(*currentVideoMode, videoModeInfo))
					primaryVideoModeIndex = j;
			}

			int32 bits = videoModeInfo.redBits + videoModeInfo.greenBits + videoModeInfo.blueBits;

			int32 wDiff = Abs(videoModeInfo.width - workW);
			int32 hDiff = Abs(videoModeInfo.height - workH);

			math::Vector2i resolution{ videoModeInfo.width, videoModeInfo.height };

			videoModePtr.reset(Construct<VideoMode>((resolution), (WMonitor)monitorPtr, (uint16)videoModeInfo.refreshRate, (uint8)bits));

			if (wDiff < workWDiff || hDiff < workHDiff)
			{
				workWDiff = wDiff;
				workHDiff = hDiff;
				highestBits = bits;
				highestRefresh = videoModeInfo.refreshRate;
				closestVideoMode = videoModePtr;
				closestVideoModeIndex = j;
			}
			else if (wDiff == workWDiff && hDiff == workHDiff && bits >= highestBits && videoModeInfo.refreshRate >= highestRefresh)
			{
				workWDiff = wDiff;
				workHDiff = hDiff;
				highestBits = bits;
				highestRefresh = videoModeInfo.refreshRate;
				closestVideoMode = videoModePtr;
				closestVideoModeIndex = j;
			}
		}

		RectI sizeRect{ math::Vector2i(monX, monY), closestVideoMode->GetResolution() };
		RectI workRect{ math::Vector2i(workX, workY), math::Vector2i(workW, workH) };

		new(monitorPtr.get())Monitor(sizeRect, workRect, i, name, std::move(videoModes), closestVideoModeIndex, (hdpi + vdpi) * 0.5f, hdpi, vdpi);
	}
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
	
	if(prev != nullptr)
	{
		// Copy monitors
		{
			LOCK(m_MonitorMutex);
			m_Monitors.clear();
			m_MainMonitor = 0;
			prev->AccessMonitors([this](CSpan<PMonitor> span)
			{
				const auto size = span.GetSizeFn();
				if(m_Monitors.capacity() < size)
					m_Monitors.reserve(size);
				for(const auto& elem : span)
				{
					if(elem->IsPrimary())
						m_MainMonitor = m_Monitors.size();
					m_Monitors.push_back(elem);
				}
			});
		}
		// Copy windows
		{

		}
	}
	else
	{
		QueryMonitors();
	}
}

void WindowManager::OnDeactivation(const PInterface& newDefault) noexcept
{
	m_Monitors.clear();
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
	return PMonitor();
}

void WindowManager::AccessMonitors(const std::function<void(CSpan<PMonitor>)>& accessFn) const
{
	LOCK(m_MonitorMutex);
	accessFn(CreateSpan(m_Monitors));
}

TResult<PWindow> WindowManager::CreateWindow(const WindowDesc& desc)
{
	return Result::CreateFailure<PWindow>(""sv);
}

void WindowManager::AccessWindows(const std::function<void(CSpan<PWindow>)>& accessFn) const
{

}