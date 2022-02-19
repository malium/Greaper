/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_EVENT_H
#define CORE_EVENT_H 1

#include "Memory.h"
#include "Concurrency.h"
#include <functional>

namespace greaper
{
	template<class... Args>
	class Event;

	/*** The class that listeners will handle
	*	Allows on demand event desconnection and desconnection at end-of-life
	*/
	template<class... Args>
	class EventHandler
	{
		Event<Args...>* m_Event = nullptr;
		uint32 m_ID = 0;
		
	public:
		~EventHandler();

		void Disconnect();

		friend class Event<Args...>;
	};

	template<class... Args>
	struct EventHandlerID
	{
		using HandlerFunction = std::function<void(Args...)>;
		HandlerFunction Function;
		uint32 ID;
	};

	template<>
	struct EventHandlerID<void>
	{
		using HandlerFunction = std::function<void()>;
		HandlerFunction Function;
		uint32 ID = 0;
	};

	/*** Handles event creation, trigger and dispatching
	*	Event provides an easy and multithreaded way of handling event triggering, dispatching and listening
	*	Also this way of handling events, provide an easy way of sending arguments via template parameters,
	*	if no parameter is wanted to be send, use Event<void>.
	*	Events can have a name if wanted, otherwise unnamed will be their name.
	*	
	*	Use example:
	*	Event<uint32, String> testEvent = Event<uint32, String>("Test");
	*	EventHandler<uint32, String> listener;
	*	testEvent.Connect(listener, [](uint32 i, String s) { std::cout << i << " " << s << std::endl; });
	*	
	*	testEvent.Trigger(0, "hi");
	*/
	template<class... Args>
	class Event
	{
		Mutex m_Mutex;
		Vector<EventHandlerID<Args...>> m_Handlers;
		String m_Name;
		uint32 m_LastID;

	public:
		using HandlerType = EventHandler<Args...>;
		using HandlerFunction = typename EventHandlerID<Args...>::HandlerFunction;
		
		Event(const StringView& eventName = "unnamed"sv) noexcept;
		~Event() = default;
		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;

		const String& GetName()const noexcept { return m_Name; }

		void Connect(HandlerType& handler, HandlerFunction function) noexcept;

		void Disconnect(HandlerType& handler) noexcept;

		void Trigger(Args&&... args) noexcept;
	};

	template<class... Args>
	EventHandler<Args...>::~EventHandler()
	{
		Disconnect();
	}

	template<class... Args>
	void EventHandler<Args...>::Disconnect()
	{
		if(m_Event != nullptr)
		{
			m_Event->Disconnect(*this);
		}
		m_Event = nullptr;
	}

	template<class... Args>
	Event<Args...>::Event(const StringView& eventName) noexcept
		:m_Name(eventName)
		,m_LastID(0)
	{

	}

	template<class... Args>
	void Event<Args...>::Connect(HandlerType& handler, HandlerFunction function) noexcept
	{
		EventHandlerID<Args...> hnd;
		hnd.Function = std::move(function);
		hnd.ID = m_LastID++;
		handler.m_Event = this;
		handler.m_ID = hnd.ID;
		auto lck = Lock(m_Mutex);
		m_Handlers.push_back(std::move(hnd));
	}

	template<class... Args>
	void Event<Args...>::Disconnect(HandlerType& handler) noexcept
	{
		auto lck = Lock(m_Mutex);
		for (auto it = m_Handlers.begin(); it != m_Handlers.end(); ++it)
		{
			EventHandlerID<Args...>& hnd = *it;
			if (hnd.ID == handler.m_ID)
			{
				handler.m_Event = nullptr;
				m_Handlers.erase(it);
				break;
			}
		}
	}

	template<class... Args>
	void Event<Args...>::Trigger(Args&&... args) noexcept
	{
		auto lck = Lock(m_Mutex);
		for (EventHandlerID<Args...>& hnd : m_Handlers)
		{
			hnd.Function(std::forward<Args>(args)...);
		}
	}

	template<>
	class Event<void>
	{
		Mutex m_Mutex;
		Vector<EventHandlerID<void>> m_Handlers;
		String m_Name;
		uint32 m_LastID;

	public:
		using HandlerType = EventHandler<void>;
		using HandlerFunction = typename EventHandlerID<void>::HandlerFunction;

		Event(const StringView& eventName = "unnamed"sv) noexcept
			:m_Name(eventName)
			, m_LastID(0)
		{

		}
		~Event() = default;
		Event(const Event&) = delete;
		Event& operator=(const Event&) = delete;

		const String& GetName()const noexcept { return m_Name; }

		void Connect(HandlerType& handler, HandlerFunction function) noexcept
		{
			EventHandlerID<void> hnd;
			hnd.Function = std::move(function);
			hnd.ID = m_LastID++;
			handler.m_Event = this;
			handler.m_ID = hnd.ID;
			auto lck = Lock(m_Mutex);
			m_Handlers.push_back(std::move(hnd));
		}

		void Disconnect(HandlerType& handler) noexcept
		{
			auto lck = Lock(m_Mutex);
			for (auto it = m_Handlers.begin(); it != m_Handlers.end(); ++it)
			{
				EventHandlerID<void>& hnd = *it;
				if (hnd.ID == handler.m_ID)
				{
					handler.m_Event = nullptr;
					m_Handlers.erase(it);
					break;
				}
			}
		}

		void Trigger(void) noexcept
		{
			auto lck = Lock(m_Mutex);
			for (EventHandlerID<void>& hnd : m_Handlers)
			{
				hnd.Function();
			}
		}
	};
}

#endif /* CORE_EVENT_H */
