/***********************************************************************************
*   Copyright 2022 Marcos S�nchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_COMMAND_MANAGER_H
#define CORE_COMMAND_MANAGER_H 1

#include "ImplPrerequisites.h"
#include <Core/ICommandManager.h>

namespace greaper::core
{
	class CommandManager final : public ICommandManager
	{
		Vector<PCommand> m_Commands;
		UnorderedMap<String, std::size_t> m_CommandMap;
		Deque<CommandInfo> m_DoneCommands;
		mutable RWMutex m_CommandMutex;

		PConsole m_Console;

	public:
		CommandManager()noexcept = default;
		~CommandManager()noexcept = default;

		void OnInitialization()noexcept override;

		void OnDeinitialization()noexcept override;

		void OnActivation(UNUSED const PInterface& oldDefault)noexcept override;

		void OnDeactivation(UNUSED const PInterface& newDefault)noexcept override;

		void InitProperties()noexcept override;

		void DeinitProperties()noexcept override;

		EmptyResult HandleCommand(const CommandInfo& info)noexcept override;

		EmptyResult UndoLastCommand()noexcept override;

		EmptyResult UndoCommand(const String& cmdName)noexcept override;

		EmptyResult AddCommand(PCommand cmd)noexcept override;

		EmptyResult RemoveCommand(const String& cmdName)noexcept override;

		TResult<PCommand> GetCommand(const String& cmdName)const noexcept override;

		bool HasCommand(const String& cmdName)const noexcept override;

		void AccessCommandStack(const std::function<void(CSpan<CommandInfo>)>& accessFn)const noexcept override;

		PConsole GetConsole()const noexcept override;

		EmptyResult CreateConsole()noexcept override;
	};
}

#endif /* CORE_COMMAND_MANAGER_H */