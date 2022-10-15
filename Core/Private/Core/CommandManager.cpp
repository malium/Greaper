/***********************************************************************************
*   Copyright 2022 Marcos Sánchez Torrent.                                         *
*   All Rights Reserved.                                                           *
***********************************************************************************/

#include "CommandManager.h"
#include <Core/IGreaperLibrary.h>
#include <Core/StringUtils.h>

using namespace greaper;
using namespace core;

SPtr<CommandManager> gCommandManager = {};

void CommandManager::OnInitialization() noexcept
{
	VerifyNot(m_Library.expired(), "Trying to initialize LogManager, but its library is expired.");
	auto lib = m_Library.lock();
	auto managers = lib->GetManagers();
	for (const auto& mgr : managers)
	{
		if (mgr.get() == this)
		{
			gCommandManager = mgr;
			break;
		}
	}
}

void CommandManager::OnDeinitialization() noexcept
{
	gCommandManager.reset();
}

void CommandManager::OnActivation(UNUSED const SPtr<IInterface>& oldDefault) noexcept
{

}

void CommandManager::OnDeactivation(UNUSED const SPtr<IInterface>& newDefault) noexcept
{

}

void CommandManager::InitProperties() noexcept
{

}

void CommandManager::DeinitProperties() noexcept
{

}

void CommandManager::InitSerialization() noexcept
{

}

void CommandManager::DeinitSerialization() noexcept
{

}

EmptyResult CommandManager::HandleCommand(const CommandInfo& info) noexcept
{
	auto lck = Lock(m_CommandMutex);
	const auto findIT = m_CommandMap.find(info.CommandName);
	if (findIT == m_CommandMap.end())
		return Result::CreateFailure(Format("Couldn't find a command named '%s'.", info.CommandName.c_str()));
	
	auto cmdIdx = findIT->second;
	if (m_Commands.size() <= cmdIdx)
		return Result::CreateFailure(Format("Something was wrong trying to find the command '%s'.", info.CommandName.c_str()));

	auto& cmd = m_Commands[cmdIdx];
	if (cmd == nullptr)
		return Result::CreateFailure(Format("Trying call the command '%s', but was nullptr.", info.CommandName.c_str()));

	if (!cmd->IsActive())
		return Result::CreateFailure(Format("Trying to call the command '%s', but was inactive.", info.CommandName.c_str()));

	auto lib = m_Library.lock();
	lib->LogVerbose(Format("Handling Command: %s(%s).",
		info.CommandName.c_str(), StringUtils::ComposeString(info.CommandArgs).c_str()));

	const auto result = cmd->DoCommand(info.CommandArgs);

	if (cmd->CanBeUndone() && result.IsOk())
	{
		m_DoneCommands.push_front(info);
	}

	return result;
}

EmptyResult CommandManager::UndoLastCommand() noexcept
{
	auto lck = Lock(m_CommandMutex);
	if (!m_DoneCommands.empty())
	{
		CommandInfo& info = m_DoneCommands.front();
		const auto findIT = m_CommandMap.find(info.CommandName);
		if (findIT == m_CommandMap.end())
			return Result::CreateFailure(Format("Couldn't find a command named '%s'.", info.CommandName.c_str()));
		
		auto cmdIdx = findIT->second;
		if (m_Commands.size() <= cmdIdx)
			return Result::CreateFailure(Format("Something was wrong trying to find the command '%s'.", info.CommandName.c_str()));

		auto& cmd = m_Commands[cmdIdx];
		if (cmd == nullptr)
			return Result::CreateFailure(Format("Trying to undo the command '%s', but was nullptr.", info.CommandName.c_str()));

		if (!cmd->IsActive())
			return Result::CreateFailure(Format("Trying to undo the command '%s', but was inactive.", info.CommandName.c_str()));

		auto lib = m_Library.lock();
		lib->LogVerbose(Format("Undoing Command: %s(%s).",
			info.CommandName.c_str(), StringUtils::ComposeString(info.CommandArgs).c_str()));

		const auto result = cmd->UndoCommand(info.CommandArgs);
		m_DoneCommands.pop_front();
		return result;
	}
	return Result::CreateSuccess();
}

EmptyResult CommandManager::UndoCommand(const String& cmdName) noexcept
{
	auto lck = Lock(m_CommandMutex);

	auto cmdInfo = m_DoneCommands.end();
	for (auto it = m_DoneCommands.begin(); it != m_DoneCommands.end(); ++it)
	{
		if (it->CommandName == cmdName)
		{
			cmdInfo = it;
			break;
		}
	}
	if (cmdInfo == m_DoneCommands.end())
		return Result::CreateFailure(Format("The command '%s' was not done.", cmdName.c_str()));

	const auto findIT = m_CommandMap.find(cmdName);
	if (findIT == m_CommandMap.end())
		return Result::CreateFailure(Format("Couldn't find a command named '%s'.", cmdName.c_str()));

	auto cmdIdx = findIT->second;
	if (m_Commands.size() <= cmdIdx)
		return Result::CreateFailure(Format("Something was wrong trying to find the command '%s'.", cmdName.c_str()));

	auto& cmd = m_Commands[cmdIdx];
	if (cmd == nullptr)
		return Result::CreateFailure(Format("Trying to undo the command '%s', but was nullptr.", cmdName.c_str()));

	if (!cmd->IsActive())
		return Result::CreateFailure(Format("Trying to undo the command '%s', but was inactive.", cmdName.c_str()));

	auto lib = m_Library.lock();
	lib->LogVerbose(Format("Undoing Command: %s(%s).",
		cmdName.c_str(), StringUtils::ComposeString(cmdInfo->CommandArgs).c_str()));

	const auto result = cmd->UndoCommand(cmdInfo->CommandArgs);

	m_DoneCommands.erase(cmdInfo);

	return result;
}

EmptyResult CommandManager::AddCommand(PCommand cmd) noexcept
{
	auto lck = Lock(m_CommandMutex);

	const auto findIT = m_CommandMap.find(cmd->GetCommandName());
	if (findIT != m_CommandMap.end())
		return Result::CreateFailure(Format("Trying to add the command '%s', but was already added.", cmd->GetCommandName().c_str()));

	m_CommandMap.insert_or_assign(cmd->GetCommandName(), m_Commands.size());
	m_Commands.push_back(std::move(cmd));
	return Result::CreateSuccess();
}

EmptyResult CommandManager::RemoveCommand(const String& cmdName) noexcept
{
	auto lck = Lock(m_CommandMutex);

	const auto findIT = m_CommandMap.find(cmdName);
	if (findIT == m_CommandMap.end())
		return Result::CreateFailure(Format("Trying to remove the command '%s', but was not found.", cmdName.c_str()));

	if (findIT->second >= m_Commands.size())
		return Result::CreateFailure(Format("Trying to remove the command '%s', but the CommandMap was pointing outside the CommandVec.", cmdName.c_str()));

	auto& cmd = m_Commands[findIT->second];
	cmd.reset();
	m_CommandMap.erase(findIT);
	return Result::CreateSuccess();
}

TResult<PCommand> CommandManager::GetCommand(const String& cmdName) const noexcept
{
	auto lck = SharedLock(m_CommandMutex);
	const auto findIT = m_CommandMap.find(cmdName);
	if (findIT == m_CommandMap.end())
		return Result::CreateFailure<PCommand>(Format("Couldn't find the command '%s'.", cmdName.c_str()));
	
	if (findIT->second >= m_Commands.size())
		return Result::CreateFailure<PCommand>(Format("Something was wrong trying to find the command '%s'.", cmdName.c_str()));

	auto& cmd = m_Commands[findIT->second];
	if (cmd == nullptr)
		return Result::CreateFailure<PCommand>(Format("trying to get the command '%s', but was nullptr.", cmdName.c_str()));
	return Result::CreateSuccess((PCommand)cmd);
}

bool CommandManager::HasCommand(const String& cmdName) const noexcept
{
	auto lck = SharedLock(m_CommandMutex);
	return m_CommandMap.find(cmdName) != m_CommandMap.end();
}

void CommandManager::AccessCommandStack(const std::function<void(CSpan<CommandInfo>)>& accessFn) const noexcept
{
	auto lck = SharedLock(m_CommandMutex);
	accessFn(CreateSpan(m_DoneCommands));
}

PConsole CommandManager::GetConsole() const noexcept { return m_Console; }

EmptyResult CommandManager::CreateConsole() noexcept
{
	return Result::CreateFailure("Not Implemented"sv);
}