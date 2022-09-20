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
		IInterface::ActivationEvt_t::HandlerType m_OnManagerActivation;
		IApplication::OnInterfaceActivationEvent_t::HandlerType m_OnNewManager;
		SPtr<LnxThreadImpl> m_This;
		Barrier m_Barrier;

		static INLINE void* RunFn(void* data)
		{
			//auto* th = (LnxThreadImpl*)data;
			auto* thread = (SPtr<LnxThreadImpl>*)data;
			SPtr<LnxThreadImpl>& lnxThread = *thread;
			if (lnxThread == nullptr)
				return nullptr;

			if(!lnxThread->m_Manager.expired())
			{
				auto mgr = lnxThread->m_Manager.lock();
				mgr->GetThreadCreationEvent()->Trigger((PThread)lnxThread);
			}

			while (lnxThread->GetState() != ThreadState_t::RUNNING)
				THREAD_YIELD();

			lnxThread->m_Barrier.sync();

			if(lnxThread->m_ThreadFn != nullptr)
				lnxThread->m_ThreadFn();

			lnxThread->m_State = ThreadState_t::STOPPED;

			if (!lnxThread->m_Manager.expired())
			{
				auto mgr = lnxThread->m_Manager.lock();
				mgr->GetThreadDestructionEvent()->Trigger((PThread)lnxThread);
			}

			lnxThread.reset();

			//pthread_exit(nullptr);
			return nullptr;
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
				auto libW = oldManager->GetLibrary();
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
			if (newManager == nullptr)
				return;
			if (newManager->GetInterfaceUUID() != IThreadManager::InterfaceUUID)
				return;

			m_Manager = (WThreadManager)newManager;
			m_OnManagerActivation.Disconnect();
			newManager->GetActivationEvent()->Connect(m_OnManagerActivation, [this](bool active, IInterface* oldManager, const PInterface& newManager) { OnManagerActivation(active, oldManager, newManager); });
			m_OnNewManager.Disconnect();
		}

	public:
		INLINE LnxThreadImpl(WThreadManager manager, PThread self, const ThreadConfig& config)noexcept
			:m_Manager(std::move(manager))
			,m_State(ThreadState_t::SUSPENDED)
            ,m_ThreadFn(config.ThreadFN)
            ,m_JoinsAtDestruction(config.JoinAtDestruction)
            ,m_Name(config.Name)
			,m_This(std::move((SPtr<LnxThreadImpl>)self))
			,m_Barrier(2)
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
			
			ret = pthread_create(&m_Handle, &threadAttrib, &LnxThreadImpl::RunFn, &m_This);
			VerifyEqual(ret, 0, "Trying to create a thread, but something went wrong, error:'%d'.", ret);

			ret = pthread_setname_np(m_Handle, m_Name.c_str());

			ret = pthread_attr_destroy(&threadAttrib);

			m_ID = m_Handle;
			if (!config.StartSuspended)
			{
				m_State = ThreadState_t::RUNNING;
				m_Barrier.sync();
			}
		}

		INLINE LnxThreadImpl(WThreadManager manager, ThreadHandle handle, ThreadID_t id, StringView name)
			:m_Manager(manager)
			, m_Handle(handle)
			, m_ID(id)
			, m_State(ThreadState_t::UNMANAGED)
			, m_ThreadFn(nullptr)
			, m_JoinsAtDestruction(false)
			, m_Name(name)
		{

		}

		INLINE LnxThreadImpl(WThreadManager manager, ThreadHandle handle, ThreadID_t id, StringView name)
			:m_Manager(manager)
			,m_Handle(handle)
			,m_ID(id)
			,m_State(ThreadState_t::UNMANAGED)
			,m_ThreadFn(nullptr)
			,m_JoinsAtDestruction(false)
			,m_Name(name)
		{
			auto mgr = m_Manager.lock();
			mgr->GetActivationEvent()->Connect(m_OnManagerActivation, [this](bool active, IInterface* oldManager, const PInterface& newManager) { OnManagerActivation(active, oldManager, newManager); });
		}

		INLINE ~LnxThreadImpl()
		{
			if (m_JoinsAtDestruction && Joinable())
				Join();
		}

		INLINE void Detach()override
		{
			auto ret = pthread_detach(m_Handle);
			VerifyEqual(ret, 0, "Trying to detach a thread, but something went wrong, error:'%d'.", ret);
			m_State = ThreadState_t::UNMANAGED;
		}

		INLINE void Join()override
		{
			if (m_State == ThreadState_t::STOPPED)
				return;

			Verify(Joinable(), "Trying to join a non-joinable thread");
			void* thRet = nullptr;
			auto ret = pthread_join(m_Handle, &thRet);
			VerifyEqual(ret, 0, "Trying to join a thread, but something went wrong, error:'%d'.", ret);
		}

		INLINE bool Joinable()const noexcept override
		{
			return m_State == ThreadState_t::RUNNING;
		}

		INLINE bool TryJoin()override
		{
			if (m_State == ThreadState_t::STOPPED)
				return true;

			if (!Joinable())
				return false;

			void* thRet = nullptr;
			auto ret = pthread_tryjoin_np(m_Handle, &thRet);
			return ret == 0;
		}

		INLINE ThreadHandle GetOSHandle()const noexcept override { return m_Handle; }

		INLINE void Terminate()override
		{
			Verify(Joinable(), "Trying to terminate a not Joinable thread.");
			auto ret = pthread_cancel(m_Handle);
			VerifyEqual(ret, 0, "Trying to join a thread, but something went wrong, error:'%d'.", ret);
			m_State = ThreadState_t::STOPPED;
		}

		INLINE void Resume()override
		{
			if (GetState() != ThreadState_t::SUSPENDED)
				return;

			m_State = ThreadState_t::RUNNING;
			m_Barrier.sync();
		}

		INLINE ThreadID_t GetID()const noexcept override { return m_ID; }

		INLINE bool JoinsAtDestruction()const noexcept override { return m_JoinsAtDestruction; }

		INLINE const String& GetName()const noexcept override { return m_Name; }

		INLINE ThreadState_t GetState()const noexcept override { return (ThreadState_t)m_State.load(); }
	};
	using Thread = LnxThreadImpl;
}

#endif /* CORE_LNX_THREAD_IMPL_I */