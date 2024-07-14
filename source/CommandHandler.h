//
// Table of Tables Console 2
//
// (c) Paul Alan Freshney 2016-2024
//   paul@freshney.org
//
// Inspired/based on the Behind the Tables sub-reddit.
//   https://www.reddit.com/r/BehindTheTables/
//
// Source code and executables:
//   https://github.com/MaximumOctopus/TableofTablesConsole
//

#pragma once


#include <string>
#include <vector>

#include "Command.h"


class CommandHandler
{
	std::vector<std::wstring> CommandTokens;

	Command LastCommand;

	Command ProcessTokens();

	void CreateTokens(const std::wstring);

	void ExecuteCommand(Command);

	std::wstring ProcessUserContent(const std::wstring);

	std::wstring ProcessUserContentTableRoll(const std::wstring);

	void ProcessUserContentHelp();

public:

	Command ProcessCommand(std::wstring);
};