/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#pragma once

#ifndef CORE_I_COMMAND_MANAGER_H
#define CORE_I_COMMAND_MANAGER_H 1

#include "CorePrerequisites.h"
#include "Interface.h"
#include "Base/ICommand.h"
#include "Result.h"
#include "Base/IConsole.h"

namespace greaper
{
	class ICommandManager : public TInterface<ICommandManager>
	{
	public:
		static constexpr Uuid InterfaceUUID = Uuid{ 0x557FF73A, 0x6C4144AF, 0xB02DE998, 0x0D378CCA };
		static constexpr StringView InterfaceName = StringView{ "CommandManager" };

		virtual ~ICommandManager() = default;

		virtual EmptyResult HandleCommand(const CommandInfo& info)noexcept = 0;

		virtual EmptyResult UndoLastCommand()noexcept = 0;

		virtual EmptyResult UndoCommand(const String& cmdName)noexcept = 0;

		virtual EmptyResult AddCommand(PCommand cmd)noexcept = 0;

		virtual EmptyResult RemoveCommand(const String& cmdName)noexcept = 0;

		virtual TResult<PCommand> GetCommand(const String& cmdName)const noexcept = 0;

		virtual bool HasCommand(const String& cmdName)const noexcept = 0;

		virtual void AccessCommandStack(const std::function<void(CSpan<CommandInfo>)>& accessFn)const noexcept = 0;

		virtual PConsole GetConsole()const noexcept = 0;

		virtual EmptyResult CreateConsole()noexcept = 0;

		template<class T, class _Alloc_ = GenericAllocator, class... Args>
		INLINE EmptyResult AddCommandT(Args&&... args)noexcept
		{
			static_assert(std::is_base_of_v<ICommand, T>, "Trying to create a Command which doesn't derive from ICommand");
			auto cmd = PCommand((ICommand*)Construct<T, _Alloc_>(args...), &Impl::DefaultDeleter<ICommand, _Alloc_>);
			return AddCommand(cmd);
		}
	};
}

#endif /* CORE_I_COMMAND_MANAGER_H */