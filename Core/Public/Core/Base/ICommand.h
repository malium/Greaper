/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
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

        static Result<CommandInfo> FromConsole(const String& cmdLine);
    };

    class ICommand
    {
        String m_CommandName;
        String m_HelpMessage;
        bool m_Active;
        bool m_CanBeUndone;

    public:
        ICommand(String commandName, String helpMessage = "", bool canBeUndone = false);

        virtual ~ICommand() = default;

        virtual bool DoCommand(const StringVec& args) = 0;

        virtual bool UndoCommand(const StringVec& args) { return true; }

        const String& GetCommandName()const noexcept { return m_CommandName; }

        const String& GetHelpMessage()const noexcept { return m_HelpMessage; }

        void SetActive(bool active) noexcept { m_Active = active; }

        bool IsActive()const noexcept { return m_Active; }

        bool CanBeUndone()const noexcept { return m_CanBeUndone; }
    };

    Result<CommandInfo> CommandInfo::FromConsole(const String& cmdLine) 
    {
        auto vec = StringUtils::SeparateBySpace(cmdLine);

        if(vec.empty())
            return CreateFailure<CommandInfo>("Couldn't recognize the command name nor its arguments from the given command line."sv);
        CommandInfo info;
        info.CommandName = vec[0];
        vec.erase(vec.begin());
        info.CommandArgs = vec;
        return CreateResult(info);
    }

    ICommand::ICommand(String commandName, String helpMessage, bool canBeUndone) 
        :m_CommandName(commandName)
        ,m_HelpMessage(helpMessage)
        ,m_Active(true)
        ,m_CanBeUndone(canBeUndone)
    {

    }
}

#endif /* CORE_I_COMMAND_H */