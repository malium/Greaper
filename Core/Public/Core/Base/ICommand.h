/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_COMMAND_H
#define CORE_I_COMMAND_H 1

#include "../Memory.h"
#include "../StringUtils.h"
#include "../Result.h"

namespace greaper
{
	struct CommandInfo
	{
		String CommandName;
		StringVec CommandArgs;

		static TResult<CommandInfo> FromConsole(const String& cmdLine)noexcept;
	};

	class ICommand
	{
		String m_CommandName;
		String m_HelpMessage;
		bool m_Active;

	public:
		explicit ICommand(StringView commandName, StringView helpMessage = ""sv)noexcept;

		virtual ~ICommand()noexcept = default;

		virtual EmptyResult DoCommand(const StringVec& args)noexcept = 0;

		virtual EmptyResult UndoCommand(UNUSED const StringVec& args)noexcept { return Result::CreateSuccess(); }

		INLINE const String& GetCommandName()const noexcept { return m_CommandName; }

		INLINE const String& GetHelpMessage()const noexcept { return m_HelpMessage; }

		INLINE void SetActive(bool active) noexcept { m_Active = active; }

		INLINE bool IsActive()const noexcept { return m_Active; }

		virtual bool CanBeUndone()const noexcept = 0;
	};

	INLINE TResult<CommandInfo> CommandInfo::FromConsole(const String& cmdLine)noexcept
	{
		auto vec = StringUtils::SeparateBySpace(cmdLine);

		if(vec.empty())
			return Result::CreateFailure<CommandInfo>("Couldn't recognize the command name nor its arguments from the given command line."sv);
		CommandInfo info;
		info.CommandName = vec[0];
		vec.erase(vec.begin());
		info.CommandArgs = vec;
		return Result::CreateSuccess(info);
	}

	INLINE ICommand::ICommand(StringView commandName, StringView helpMessage) noexcept
		:m_CommandName(commandName)
		,m_HelpMessage(helpMessage)
		,m_Active(true)
	{

	}
}

#endif /* CORE_I_COMMAND_H */