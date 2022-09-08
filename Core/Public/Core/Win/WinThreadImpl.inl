/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_WIN_THREAD_IMPL_I
#define CORE_WIN_THREAD_IMPL_I 1

//#include "../Base/IThread.h"
//#include "../Event.h"
#include "../IThreadManager.h"

namespace greaper
{
	class WinThreadImpl : IThread
	{
		WThreadManager m_Manager;
		ThreadHandle m_Handle;
		ThreadID_t m_ID;
		std::atomic_int8_t m_State;
		std::function<void()> m_ThreadFn;
		bool m_JoinsAtDestruction;
		String m_Name;

		static inline unsigned STDCALL RunFn(void* data)
		{
			//auto* th = (WinThreadImpl*)data;
			auto* wThis = (WThread*)data;
			auto th = (SPtr<WinThreadImpl>)wThis->Lock();
			if (th == nullptr)
				return EXIT_FAILURE;

			{
				auto mgr = th->m_Manager.Lock();
				mgr->GetThreadEvent()->Trigger((PThread)th);
			}

			while (th->GetState() != ThreadState_t::RUNNING)
				THREAD_YIELD();

			if(th->m_ThreadFn != nullptr)
				th->m_ThreadFn();

			th->m_State = ThreadState_t::STOPPED;

			return EXIT_SUCCESS;
		}

		void SetName()
		{
			typedef HRESULT(WINAPI* SetThreadDescription)(HANDLE hThread, PCWSTR lpThreadDescription);
			auto* setThreadDescription = reinterpret_cast<SetThreadDescription>(
				GetProcAddress(GetModuleHandleA("kernel32.dll"), "SetThreadDescription"));

			if(setThreadDescription == nullptr)
				setThreadDescription = reinterpret_cast<SetThreadDescription>(
					GetProcAddress(GetModuleHandleA("KernelBase.dll"), "SetThreadDescription"));
			if (setThreadDescription != nullptr)
			{
				wchar wName[MAX_PATH];
				mbstowcs(wName, m_Name.c_str(), MAX_PATH);
				setThreadDescription(m_Handle, wName);
			}

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

	public:
		WinThreadImpl(WThreadManager manager, WThread self, ThreadConfig config)
			:m_Manager(manager)
			,m_Handle(InvalidThreadHandle)
			,m_ID(InvalidThreadID)
			,m_State(ThreadState_t::SUSPENDED)
			,m_ThreadFn(config.ThreadFN)
			,m_JoinsAtDestruction(config.JoinAtDestruction)
			,m_Name(config.Name)
		{
			if (m_Manager == nullptr || m_ThreadFn == nullptr)
			{
				m_State = ThreadState_t::STOPPED;
				return;
			}

			auto hnd = _beginthreadex(nullptr, config.StackSize, &WinThreadImpl::RunFn, &self,
				config.StartSuspended ? CREATE_SUSPENDED : 0, &m_ID);

			if (hnd == 0)
			{
				m_State = ThreadState_t::STOPPED;
				m_ID = InvalidThreadID;
				VerifyNotNull(m_Manager, "Something went wrong trying to create a WinThread.");
				auto mgr = (PInterface)m_Manager.Lock();
				auto wlib = mgr->GetLibrary();
				VerifyNotNull(wlib, "Something went wrong trying to create a WinThread.");
				auto lib = wlib.Lock();
				lib->LogError(
					Format("Trying to create a thread named '%s', but something went wrong '%s'.", m_Name.c_str(), strerror(errno))
				);
				return;
			}

			SetName();

			m_Handle = reinterpret_cast<HANDLE>(hnd);
			if (!config.StartSuspended)
				m_State = ThreadState_t::RUNNING;
		}

		~WinThreadImpl()
		{
			if (m_JoinsAtDestruction && Joinable())
				Join();
		}

		void Detach()override
		{
			m_Handle = InvalidThreadHandle;
			m_ID = InvalidThreadID;
			m_State = ThreadState_t::STOPPED;
		}

		void Join()override
		{
			Verify(Joinable(), "Trying to join a not-joinable thread");
			WaitForSingleObject(m_Handle, INFINITE);
			m_Handle = InvalidThreadHandle;
			m_ID = InvalidThreadID;
			//m_State = ThreadState_t::STOPPED;
		}

		bool Joinable()const noexcept override
		{
			return m_State == ThreadState_t::RUNNING && m_Handle != InvalidThreadHandle && m_ID != InvalidThreadID;
		}

		bool TryJoin()override
		{
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

		ThreadHandle GetOSHandle()const noexcept override { return m_Handle; }

		void Terminate()override
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

		void Resume()override
		{
			if (GetState() != ThreadState_t::SUSPENDED)
				return;

			Verify(Joinable(), "Trying to resume an invalid thread.");

			ResumeThread(m_Handle);
			m_State = ThreadState_t::RUNNING;
		}

		ThreadID_t GetID()const noexcept override { return m_ID; }

		bool JoinsAtDestruction()const noexcept override { return m_JoinsAtDestruction; }

		const String& GetName()const noexcept override { return m_Name; }

		ThreadState_t GetState()const noexcept override { return (ThreadState_t)m_State.load(); }
	};
	using Thread = WinThreadImpl;
}

#endif /* CORE_WIN_THREAD_IMPL_I */