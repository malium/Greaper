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
		IThreadManager* m_Manager;
		ThreadConfig m_Config;
		ThreadHandle m_Handle;
		ThreadID_t m_ID;
		std::atomic_int8_t m_State;

		static inline unsigned STDCALL RunFn(void* data)
		{
			auto* th = (WinThreadImpl*)data;
			if (th == nullptr)
				return EXIT_FAILURE;

			th->m_Manager->GetThreadEvent()->Trigger(th);

			while (th->GetState() != ThreadState_t::RUNNING)
				THREAD_YIELD();

			th->GetConfiguration().ThreadFN();

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
				mbstowcs(wName, m_Config.Name.data(), MAX_PATH);
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
				ThreadNameInfo threadNameInfo = { 0x1000, m_Config.Name.data(), m_ID, 0 };
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
		WinThreadImpl(IThreadManager* manager, ThreadConfig config)
			:m_Manager(manager)
			,m_Config(std::move(config))
			,m_Handle(InvalidThreadHandle)
			,m_ID(InvalidThreadID)
			,m_State(ThreadState_t::SUSPENDED)
		{
			if (manager == nullptr || m_Config.ThreadFN == nullptr)
			{
				m_State = ThreadState_t::STOPPED;
				return;
			}

			auto hnd = _beginthreadex(nullptr, m_Config.StackSize, &WinThreadImpl::RunFn, this,
				m_Config.StartSuspended ? CREATE_SUSPENDED : 0, &m_ID);

			if (hnd == 0)
			{
				m_State = ThreadState_t::STOPPED;
				m_ID = InvalidThreadID;
				((IInterface*)m_Manager)->GetLibrary()->LogError(
					Format("Trying to create a thread named '%s', but something went wrong '%s'.", m_Config.Name.data(), strerror(errno))
				);
				return;
			}

			SetName();

			m_Handle = reinterpret_cast<HANDLE>(hnd);
			if (!m_Config.StartSuspended)
				m_State = ThreadState_t::RUNNING;
		}

		~WinThreadImpl()
		{
			if (m_Config.JoinAtDestruction && Joinable())
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

		const ThreadConfig& GetConfiguration()const noexcept override { return m_Config; }

		ThreadState_t GetState()const noexcept override { return (ThreadState_t)m_State.load(); }
	};
	using Thread = WinThreadImpl;
}

#endif /* CORE_WIN_THREAD_IMPL_I */