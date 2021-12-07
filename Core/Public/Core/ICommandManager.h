/***********************************************************************************
*   Copyright 2021 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_COMMAND_MANAGER_H
#define CORE_I_COMMAND_MANAGER_H 1

#include "Interface.h"
#include "Base/ICommand.h"
#include "Result.h"

namespace greaper
{
    class ICommandManager : public TInterface<ICommandManager>
    {
    public:
        static constexpr Uuid InterfaceUUID = Uuid{ 0x557FF73A, 0x6C4144AF, 0xB02DE998, 0x0D378CCA };
		static constexpr StringView InterfaceName = StringView{ "CommandManager" };

        virtual ~ICommandManager() = default;

        virtual void HandleCommand(const CommandInfo& info) = 0;

        virtual void UndoLastCommand() = 0;

        virtual void UndoCommand(const String& cmdName) = 0;

        virtual EmptyResult AddCommand(ICommand* cmd) = 0;

        virtual EmptyResult RemoveCommand(const String& cmdName) = 0;

        virtual Result<ICommand*> GetCommand(const String& cmdName) = 0;

        virtual bool HasCommand(const String& cmdName)const = 0;

        virtual sizet GetStackedCommandCount()const = 0;
    };

    template<class T, class _Alloc_ = GenericAllocator, class... Args>
    EmptyResult AddCommand(ICommandManager* mgr, Args&&... args)
    {
        static_assert(std::is_base_of_v<ICommand, T>, "Trying to create a Command which doesn't derive from ICommand");
        auto cmd = (ICommand*)Construct<T, _Alloc_>(args);
        auto rtn = mgr->AddCommand(cmd);
        if(rtn.HasFailed())
            Destroy<T, _Alloc_>((T*)cmd);
        return mgr->AddCommand(cmd);
    }
}

#endif /* CORE_I_COMMAND_MANAGER_H */