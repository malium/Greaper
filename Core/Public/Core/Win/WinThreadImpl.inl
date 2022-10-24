/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#include "../IGreaperLibrary.h"
#include "WinPlatform.h"

namespace greaper
{
	class WinThreadImpl
	{
		WThreadManager m_Manager;
		ThreadHandle m_Handle;
		ThreadID_t m_ID;
		std::atomic_int8_t m_State;
		std::function<void()> m_ThreadFn;
		bool m_JoinsAtDestruction;
		String m_Name;
		IInterface::ActivationEvt_t::HandlerType m_OnManagerActivation;
		IApplication::OnInterfaceActivationEvent_t::HandlerType m_OnNewManager;
		PThread m_This;
		Barrier m_Barier;

		static INLINE unsigned STDCALL RunFn(void* data)
		{
			OSPlatform::PerThreadSEHInit();

			auto* thread = (PThread*)data;
			PThread& winThread = *thread;
			
			if (winThread == nullptr)
				return EXIT_FAILURE;

			if(!winThread->m_Manager.expired())
			{
				auto mgr = winThread->m_Manager.lock();
				mgr->GetThreadCreationEvent()->Trigger(winThread);
			}

			while (winThread->GetState() != ThreadState_t::RUNNING)
				THREAD_YIELD();

			winThread->m_Barier.sync();

			if(winThread->m_ThreadFn != nullptr)
				winThread->m_ThreadFn();

			winThread->m_State = ThreadState_t::STOPPED;

			if (!winThread->m_Manager.expired())
			{
				auto mgr = winThread->m_Manager.lock();
				mgr->GetThreadDestructionEvent()->Trigger(winThread);
			}
			winThread.reset();

			return EXIT_SUCCESS;
		}

		void SetNameKernel()
		{
			using SetThreadDescription_t = HRESULT(WINAPI*)(HANDLE hThread, PCWSTR lpThreadDescription);
			SetThreadDescription_t SetThreadDescription = nullptr;

			{
				Library kernel32{ L"kernel32.dll"sv };
				auto fnRes = kernel32.GetFunction("SetThreadDescription"sv);
				if (fnRes.IsOk() && fnRes.GetValue() != nullptr)
					SetThreadDescription = (SetThreadDescription_t)fnRes.GetValue();
			}
			if (SetThreadDescription == nullptr)
			{
				Library kernelBase{ L"KernelBase.dll"sv };
				auto fnRes = kernelBase.GetFunction("SetThreadDescription"sv);
				if (fnRes.IsOk() && fnRes.GetValue() != nullptr)
					SetThreadDescription = (SetThreadDescription_t)fnRes.GetValue();
			}

			if (SetThreadDescription != nullptr)
			{
				wchar wName[MAX_PATH];
				mbstowcs(wName, m_Name.c_str(), MAX_PATH);
				SetThreadDescription(m_Handle, wName);
			}
		}
		void SetName()
		{
			SetNameKernel();

			struct ThreadNameInfo
			{
				DWORD dwType;
				LPCSTR lpName;
				DWORD dwThreadId;
				DWORD dwFlags;
			};

#if COMPILER_MSVC
#pragma warning(push)
#pragma warning(disable : 4611)
#endif
			jmp_buf jmpbuf;
			if (!setjmp(jmpbuf))
			{
				ThreadNameInfo threadNameInfo = { 0x1000, m_Name.c_str(), m_ID, 0 };
				__try {
					RaiseException(0x406D1388, 0, sizeof(threadNameInfo) / sizeof(ULONG_PTR), reinterpret_cast<const ULONG_PTR*>(reinterpret_cast<uintptr_t>(&threadNameInfo)));
				}
				__except (GetExceptionCode() == 0x406D1388 ? EXCEPTION_EXECUTE_HANDLER : EXCEPTION_CONTINUE_SEARCH)
				{

				}
				longjmp(jmpbuf, 1);
			}
#if COMPILER_MSVC
#pragma warning(pop)
#endif
		}

		void OnManagerActivation(bool active, IInterface* oldManager, const PInterface& newManager)noexcept
		{
			if (active)
				return;

			if (newManager != nullptr)
			{
				const auto& newThreadMgr = (const PThreadManager&)newManager;
				m_OnManagerActivation.Disconnect();
				newThreadMgr->GetActivationEvent()->Connect(m_OnManagerActivation, [this](bool active, IInterface* oldManager, const PInterface& newManager) { OnManagerActivation(active, oldManager, newManager); });
				m_Manager = (WThreadManager)newThreadMgr;
			}
			else
			{
				m_OnManagerActivation.Disconnect();
				const auto& libW = oldManager->GetLibrary();
				VerifyNot(libW.expired(), "Trying to connect to InterfaceActivationEvent but GreaperLibrary was expired.");
				auto lib = libW.lock();
				auto appW = lib->GetApplication();
				VerifyNot(appW.expired(), "Trying to connect to InterfaceActivationEvent but Application was expired.");
				auto app = appW.lock();
				m_OnNewManager.Disconnect();
				app->GetOnInterfaceActivationEvent()->Connect(m_OnNewManager, [this](const PInterface& newManager) { OnNewManager(newManager); });
			}
		}

		void OnNewManager(const PInterface& newManager)noexcept
		{
			if (newManager == nullptr || newManager->GetInterfaceUUID() != IThreadManager::InterfaceUUID)
				return;

			m_Manager = (WThreadManager)newManager;
			m_OnManagerActivation.Disconnect();
			newManager->GetActivationEvent()->Connect(m_OnManagerActivation, [this](bool active, IInterface* oldManager, const PInterface& newManager) { OnManagerActivation(active, oldManager, newManager); });
			m_OnNewManager.Disconnect();
		}

	public:
		INLINE WinThreadImpl(WThreadManager manager, PThread self, const ThreadConfig& config)noexcept
			:m_Manager(std::move(manager))
			,m_Handle(InvalidThreadHandle)
			,m_ID(InvalidThreadID)
			,m_State(ThreadState_t::SUSPENDED)
			,m_ThreadFn(config.ThreadFN)
			,m_JoinsAtDestruction(config.JoinAtDestruction)
			,m_Name(config.Name)
			,m_This(std::move(self))
			,m_Barier(2)
		{
			if (m_Manager.expired() || m_ThreadFn == nullptr)
			{
				m_State = ThreadState_t::STOPPED;
				return;
			}

			auto hnd = _beginthreadex(nullptr, config.StackSize, &WinThreadImpl::RunFn, &m_This,
				config.StartSuspended ? CREATE_SUSPENDED : 0, &m_ID);

			if (hnd == 0)
			{
				m_State = ThreadState_t::STOPPED;
				m_ID = InvalidThreadID;
				auto mgr = m_Manager.lock();
				const auto& wlib = mgr->GetLibrary();
				VerifyNot(wlib.expired(), "Something went wrong trying to create a WinThread.");
				auto lib = wlib.lock();
				lib->LogError(
					Format("Trying to create a thread named '%s', but something went wrong '%s'.", m_Name.c_str(), strerror(errno))
				);
				return;
			}

			SetName();

			m_Handle = reinterpret_cast<HANDLE>(hnd);
			if (!config.StartSuspended)
				m_State = ThreadState_t::RUNNING;

			auto mgr = m_Manager.lock();
			mgr->GetActivationEvent()->Connect(m_OnManagerActivation, [this](bool active, IInterface* oldManager, const PInterface& newManager) { OnManagerActivation(active, oldManager, newManager); });
			m_Barier.sync();
		}

		INLINE WinThreadImpl(WThreadManager manager, ThreadHandle handle, ThreadID_t id, StringView name, bool setName = false)
			:m_Manager(std::move(manager))
			,m_Handle(handle)
			,m_ID(id)
			,m_State(ThreadState_t::UNMANAGED)
			,m_ThreadFn(nullptr)
			,m_JoinsAtDestruction(false)
			,m_Name(name)
		{
			if (setName)
				SetName();
			auto mgr = m_Manager.lock();
			mgr->GetActivationEvent()->Connect(m_OnManagerActivation, [this](bool active, IInterface* oldManager, const PInterface& newManager) { OnManagerActivation(active, oldManager, newManager); });
		}

		INLINE ~WinThreadImpl()
		{
			if (m_JoinsAtDestruction && Joinable())
				Join();
		}

		INLINE void Detach()
		{
			m_Handle = InvalidThreadHandle;
			m_ID = InvalidThreadID;
			m_JoinsAtDestruction = false;
			m_State = ThreadState_t::UNMANAGED;
		}

		INLINE void Join()
		{
			if (m_State == ThreadState_t::STOPPED)
				return;

			Verify(Joinable(), "Trying to join a not-joinable thread");
			WaitForSingleObject(m_Handle, INFINITE);
			m_Handle = InvalidThreadHandle;
			m_ID = InvalidThreadID;
			//m_State = ThreadState_t::STOPPED;
		}

		INLINE bool Joinable()const noexcept 
		{
			return m_State == ThreadState_t::RUNNING
				&& m_Handle != InvalidThreadHandle && m_ID != InvalidThreadID;
		}

		INLINE bool TryJoin()
		{
			if (m_State == ThreadState_t::STOPPED)
				return true;
			
			if (!Joinable())
				return false;

			const auto ret = WaitForSingleObject(m_Handle, 0);
#if GREAPER_DEBUG
			VerifyInequal(ret, WAIT_FAILED, "Something went wrong trying to join a thread, error '%d'.", GetLastError());
#endif
			if (ret == WAIT_OBJECT_0)
			{
				m_Handle = InvalidThreadHandle;
				m_ID = InvalidThreadID;
				//m_State = ThreadState_t::STOPPED;
				return true;
			}
			return false;
		}

		INLINE ThreadHandle GetOSHandle()const noexcept  { return m_Handle; }

		INLINE void Terminate()
		{
			Verify(Joinable(), "Trying to terminate a not Joinable thread.");
			if (!TerminateThread(m_Handle, EXIT_FAILURE))
			{
				Break("Something went wrong while terminating a thread, error:'%d'.", GetLastError());
			}
			m_Handle = InvalidThreadHandle;
			m_ID = InvalidThreadID;
			m_State = ThreadState_t::STOPPED;
		}

		INLINE void Resume()
		{
			if (GetState() != ThreadState_t::SUSPENDED)
				return;

			ResumeThread(m_Handle);
			m_State = ThreadState_t::RUNNING;
			m_Barier.sync();
		}

		INLINE ThreadID_t GetID()const noexcept  { return m_ID; }

		INLINE bool JoinsAtDestruction()const noexcept  { return m_JoinsAtDestruction; }

		INLINE const String& GetName()const noexcept  { return m_Name; }

		INLINE ThreadState_t GetState()const noexcept  { return (ThreadState_t)m_State.load(); }
	};
}
