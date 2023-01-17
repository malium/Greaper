/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "WindowManager.h"
#include "GreaperDisplayDLL.h"
#define GLFW_DLL
#include <External/GLFW/glfw3.h>

using namespace greaper;
using namespace greaper::disp;

SPtr<WindowManager> gWindowManager = {};
extern SPtr<GreaperDispLibrary> gDispLibrary;

static PMonitor CreateMonitor(GLFWmonitor* monitorInfo, int32 index)
{
	const char* errorMsg = nullptr;
	float hdpi, vdpi;
	int32 workX, workY, workW, workH;
	int32 monX, monY;
	int32 videoModeCount;

	PMonitor monitorPtr{};
	String name{};

	auto nameRet = glfwGetMonitorName(monitorInfo);
	if (nameRet != nullptr)
		name.assign(nameRet);
	else
		gDispLibrary->LogWarning(Format("Couldn't retrive the monitor name, error code: %" PRIi32 " error msg: %s.", glfwGetError(&errorMsg), errorMsg));

	glfwGetMonitorContentScale(monitorInfo, &hdpi, &vdpi);

	glfwGetMonitorWorkarea(monitorInfo, &workX, &workY, &workW, &workH);

	glfwGetMonitorPos(monitorInfo, &monX, &monY);
	
	const auto* videoModeInfos = glfwGetVideoModes(monitorInfo, &videoModeCount);

	Vector<PVideoMode> videoModes{};
	videoModes.resize(videoModeCount, PVideoMode());

	const auto* currentVideoMode = glfwGetVideoMode(monitorInfo);

	monitorPtr.reset(AllocT<Monitor>());
	glfwSetMonitorUserPointer(monitorInfo, monitorPtr.get());

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

	new(monitorPtr.get())Monitor(sizeRect, workRect, index, name, std::move(videoModes), closestVideoModeIndex, (hdpi + vdpi) * 0.5f, hdpi, vdpi);
	return monitorPtr;
}

void greaper::disp::OnMonitorChange(GLFWmonitor* monitor, int32 event)
{
	if (gWindowManager == nullptr || !gWindowManager->IsInitialized() || !gWindowManager->IsActive())
		return;
		
	if(event == GLFW_CONNECTED)
	{
		LOCK(gWindowManager->m_MonitorMutex);
		auto mon = CreateMonitor(monitor, (int32)gWindowManager->m_Monitors.size());
		gDispLibrary->LogVerbose(Format("The monitor with name '%s' has been attached.", mon->GetName().c_str()));
		gWindowManager->m_Monitors.push_back(std::move(mon));
	}
	else if(event == GLFW_DISCONNECTED)
	{
		auto* monitorPtr = glfwGetMonitorUserPointer(monitor);
		if (monitorPtr == nullptr)
		{
			gDispLibrary->LogWarning("A monitor has been disconnected but its UserPointer was nullptr (It was not handled, weird).");
			return;
		}
		LOCK(gWindowManager->m_MonitorMutex);
		auto findIT = std::find_if(gWindowManager->m_Monitors.begin(), gWindowManager->m_Monitors.end(), [monitorPtr](PMonitor mon) { return mon.get() == monitorPtr; });
		if (findIT != gWindowManager->m_Monitors.end())
		{
			gDispLibrary->LogVerbose(Format("The monitor with name '%s' has been disconnected.", findIT->get()->GetName().c_str()));
			gWindowManager->m_Monitors.erase(findIT);
		}
		else
		{
			gDispLibrary->LogWarning("A monitor has been disconnected but the Monitor class was not found (weird).");
		}
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
			LOCK(m_WindowMutex);
			m_Windows.clear();
			prev->AccessWindows([this](CSpan<PWindow> span)
			{
				const auto size = span.GetSizeFn();
				if(m_Windows.capacity() < size)
					m_Windows.reserve(size);
				for(const auto& elem : span)
				{
					m_Windows.push_back(elem);
				}
			});
		}
	}
	else
	{
		QueryMonitors();
	}
	glfwSetMonitorCallback(&OnMonitorChange);
}

void WindowManager::OnDeactivation(const PInterface& newDefault) noexcept
{
	m_Monitors.clear();
	if (newDefault == nullptr) // If there's no new WindowManager disconnect callback
		glfwSetMonitorCallback(nullptr);
}

void WindowManager::InitProperties() noexcept
{

}

void WindowManager::DeinitProperties() noexcept
{

}

void WindowManager::QueryMonitors()
{
	Verify(!m_Library.expired(), "GreaperLibrary attached was expired!");
	auto lib = m_Library.lock();

	int32 monitorCount;
	auto** monitors = glfwGetMonitors(&monitorCount);
	LOCK(m_MonitorMutex);
	m_Monitors.clear();
	m_Monitors.reserve(monitorCount);
	m_MainMonitor = 0;
	
	for (int32 i = 0; i < monitorCount; ++i)
	{
		m_Monitors.push_back(CreateMonitor(monitors[i], i));
	}
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
	SharedLock lock(m_MonitorMutex);
	accessFn(CreateSpan(m_Monitors));
}

TResult<PWindow> WindowManager::CreateWindow(const WindowDesc& desc)
{
	auto scheduler = desc.Scheduler;
	if(scheduler == nullptr)
		scheduler = MPMCTaskScheduler::Create(m_ThreadManager, "GreaperWindow TaskScheduler"sv, 1, false);
	
	

	return Result::CreateFailure<PWindow>("Not implemented"sv);
}

void WindowManager::PollEvents()
{
	if(m_MainThread.expired())
	{
		Verify(!m_Library.expired(), "Trying to PollEvents, but the MainThread handler expired and the GreaperLibrary too!");
		auto lib = m_Library.lock();
		lib->LogError("Trying to WindowManager::PollEvents, but it has no MainThread, something went wrong!");
		return;
	}

	auto mainThread = m_MainThread.lock();
	auto curThreadID = CUR_THID();
	if(mainThread->GetID() != curThreadID)
	{
		Verify(!m_Library.expired(), "Trying to PollEvents, but the GreaperLibrary has expired!");
		auto lib = m_Library.lock();
		lib->LogError(Format("Trying to WindowManager::PollEvents, but it must be called from the MainThread, Calling:%d Main:%d.", curThreadID, mainThread->GetID()));
		return;
	}

	glfwPollEvents();
}

void WindowManager::AccessWindows(const std::function<void(CSpan<PWindow>)>& accessFn) const
{
	SharedLock lock(m_WindowMutex);
	accessFn(CreateSpan(m_Windows));
}

