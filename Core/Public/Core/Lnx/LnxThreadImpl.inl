/***********************************************************************************
*   Copyright 2021 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_LNX_THREAD_IMPL_I
#define CORE_LNX_THREAD_IMPL_I 1

//#include "../Base/IThread.h"
//#include <atomic>
#include "../IThreadManager.h"

namespace greaper
{

	class LnxThreadImpl : IThread
	{
		IThreadManager* m_Manager;
		ThreadConfig m_Config;
		ThreadHandle m_Handle;
		ThreadID_t m_ID;
		std::atomic_int8_t m_State;

		static inline void* RunFn(void* data)
		{
			auto* th = (LnxThreadImpl*)data;
			if (th == nullptr)
				return;

			th->m_Manager->GetThreadEvent()->Trigger(th);

			while (th->GetState() != ThreadState_t::RUNNING)
				THREAD_YIELD();

			th->GetConfiguration().ThreadFN();

			th->m_State = ThreadState_t::STOPPED;

			//pthread_exit(nullptr);
			return nullptr;
		}

	public:
		LnxThreadImpl(IThreadManager* manager, ThreadConfig config)
			:m_Manager(manager)
			,m_Config(std::move(config))
			,m_State(ThreadState_t::SUSPENDED)
		{
			if (manager == nullptr || m_Config.ThreadFN == nullptr)
			{
				m_State = ThreadState_t::STOPPED;
				return;
			}

			pthread_attr_t threadAttrib;
			auto ret = pthread_attr_init(&threadAttrib);
			VerifyEqual(ret, 0, "Trying to create a thread, but something went wrong creating its attributes, error:'%d'.", ret);

			if(m_Config.StackSize > 0 && m_Config.StackSize >= PTHREAD_STACK_MIN)
			{
				ret = pthread_attr_setstacksize(&threadAttrib, m_Config.StackSize);
				VerifyEqual(ret, 0, "Trying to create a thread, but something went wrong setting it stack size, error:'%d'.", ret);
			}
			
			ret = pthread_create(&m_Handle, &threadAttrib, &LnxThreadImpl::RunFn, this);
			VerifyEqual(ret, 0, "Trying to create a thread, but something went wrong, error:'%d'.", ret);

			ret = pthread_setname_np(m_Handle, m_Config.Name.data());

			ret = pthread_attr_destroy(&threadAttrib);

			m_ID = m_Handle;
			if(!m_Config.StartSuspended)
				m_State = ThreadState_t::RUNNING;
		}

		~LnxThreadImpl()
		{
			if (m_Config.JoinAtDestruction && Joinable())
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
			Verify(Joinable(), "Trying to join a not-joinable thread");
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

		const ThreadConfig& GetConfiguration()const noexcept override { return m_Config; }

		ThreadState_t GetState()const noexcept override { return (ThreadState_t)m_State.load(); }
	};
	using Thread = LnxThreadImpl;
}

#endif /* CORE_LNX_THREAD_IMPL_I */