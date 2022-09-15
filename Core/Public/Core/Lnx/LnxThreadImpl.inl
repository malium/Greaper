/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LNX_THREAD_IMPL_I
#define CORE_LNX_THREAD_IMPL_I 1

//#include "../Base/IThread.h"
//#include <atomic>
#include <utility>

#include "../IThreadManager.h"

namespace greaper
{
	class LnxThreadImpl : IThread
	{
		WThreadManager m_Manager;
		ThreadHandle m_Handle;
		ThreadID_t m_ID;
		std::atomic_int8_t m_State;
        std::function<void()> m_ThreadFn;
        bool m_JoinsAtDestruction;
        String m_Name;

		static inline void* RunFn(void* data)
		{
			//auto* th = (LnxThreadImpl*)data;
			auto* wThis = (WThread*)data;
			auto th = (SPtr<LnxThreadImpl>)wThis->lock();
			if (th == nullptr)
				return nullptr;

			{
				auto mgr = th->m_Manager.lock();
				mgr->GetThreadCreationEvent()->Trigger((PThread)th);
			}

			while (th->GetState() != ThreadState_t::RUNNING)
				THREAD_YIELD();

			if(th->m_ThreadFn != nullptr)
				th->m_ThreadFn();

			th->m_State = ThreadState_t::STOPPED;

			{
				auto mgr = th->m_Manager.lock();
				mgr->GetThreadDestructionEvent()->Trigger((PThread)th);
			}

			//pthread_exit(nullptr);
			return nullptr;
		}

	public:
		LnxThreadImpl(WThreadManager manager, WThread self, const ThreadConfig& config)noexcept
			:m_Manager(std::move(manager))
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

			pthread_attr_t threadAttrib;
			auto ret = pthread_attr_init(&threadAttrib);
			VerifyEqual(ret, 0, "Trying to create a thread, but something went wrong creating its attributes, error:'%d'.", ret);

			if(config.StackSize > 0 && config.StackSize >= PTHREAD_STACK_MIN)
			{
				ret = pthread_attr_setstacksize(&threadAttrib, config.StackSize);
				VerifyEqual(ret, 0, "Trying to create a thread, but something went wrong setting it stack size, error:'%d'.", ret);
			}
			
			ret = pthread_create(&m_Handle, &threadAttrib, &LnxThreadImpl::RunFn, &self);
			VerifyEqual(ret, 0, "Trying to create a thread, but something went wrong, error:'%d'.", ret);

			ret = pthread_setname_np(m_Handle, m_Name.c_str());

			ret = pthread_attr_destroy(&threadAttrib);

			m_ID = m_Handle;
			if(!config.StartSuspended)
				m_State = ThreadState_t::RUNNING;
		}

		~LnxThreadImpl()
		{
			if (m_JoinsAtDestruction && Joinable())
				Join();
		}

		void Detach()override
		{
			auto ret = pthread_detach(m_Handle);
			VerifyEqual(ret, 0, "Trying to detach a thread, but something went wrong, error:'%d'.", ret);
			m_State = ThreadState_t::STOPPED;
		}

		void Join()override
		{
			Verify(Joinable(), "Trying to join a non-joinable thread");
			void* thRet = nullptr;
			auto ret = pthread_join(m_Handle, &thRet);
			VerifyEqual(ret, 0, "Trying to join a thread, but something went wrong, error:'%d'.", ret);
		}

		bool Joinable()const noexcept override
		{
			return m_State == ThreadState_t::RUNNING;
		}

		bool TryJoin()override
		{
			if (!Joinable())
				return false;

			void* thRet = nullptr;
			auto ret = pthread_tryjoin_np(m_Handle, &thRet);
			return ret == 0;
		}

		ThreadHandle GetOSHandle()const noexcept override { return m_Handle; }

		void Terminate()override
		{
			Verify(Joinable(), "Trying to terminate a not Joinable thread.");
			auto ret = pthread_cancel(m_Handle);
			VerifyEqual(ret, 0, "Trying to join a thread, but something went wrong, error:'%d'.", ret);
			m_State = ThreadState_t::STOPPED;
		}

		void Resume()override
		{
			if (GetState() != ThreadState_t::SUSPENDED)
				return;

			Verify(Joinable(), "Trying to resume an invalid thread.");

			m_State = ThreadState_t::RUNNING;
		}

		ThreadID_t GetID()const noexcept override { return m_ID; }

		bool JoinsAtDestruction()const noexcept override { return m_JoinsAtDestruction; }

		const String& GetName()const noexcept override { return m_Name; }

		ThreadState_t GetState()const noexcept override { return (ThreadState_t)m_State.load(); }
	};
	using Thread = LnxThreadImpl;
}

#endif /* CORE_LNX_THREAD_IMPL_I */