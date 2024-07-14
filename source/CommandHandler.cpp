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

#include <algorithm>
#include <iostream>

#include "CommandHandler.h"
#include "DiceHandler.h"
#include "Help.h"
#include "HoardHandler.h"
#include "NamesHandler.h"
#include "NPCHandler.h"
#include "Output.h"
#include "ShopHandler.h"
#include "TableHandler.h"


CommandHandler* GCommandHandler;

extern DiceHandler* GDiceHandler;
extern HoardHandler* GHoardHandler;
extern NamesHandler* GNamesHandler;
extern NPCHandler* GNPCHandler;
extern ShopHandler* GShopHandler;
extern TableHandler* GTableHandler;


Command CommandHandler::ProcessCommand(std::wstring input)
{
	if (input != L"")
	{
		CreateTokens(input);

		LastCommand = ProcessTokens();

		ExecuteCommand(LastCommand);

		return LastCommand;
	}
	else
	{
		return LastCommand;
	}
}


Command CommandHandler::ProcessTokens()
{
	Command c;

	c.primarystr = CommandTokens[0];

	c.fullcommandstr = L"";

	if (CommandTokens.size() > 1)
	{
		for (int i = 1; i < CommandTokens.size(); i++)
		{
			c.fullcommandstr += CommandTokens[i] + L" ";

		}
	}

	for (int t = 0; t < kCommandCount; t++)
	{
		if (CommandTokens[0] == ValidCommandText[t])
		{
			c.primary = ValidCommands[t];

			if (CommandTokens.size() > 1)
			{
				c.secondary = CommandTokens[1];

				if (CommandTokens.size() > 2)
				{
					c.tertiary = CommandTokens[2];

					if (CommandTokens.size() > 3)
					{
						c.quaternary = CommandTokens[3];
					}
				}
			}
		}
	}

	return c;
}


void CommandHandler::CreateTokens(const std::wstring input)
{
	CommandTokens.clear();

	std::wstring commands(input + L"");

	std::transform(commands.begin(), commands.end(), commands.begin(), ::tolower);

	std::wstring token = L"";
	bool inquotes = false;

	for (int t = 0; t < commands.length(); t++)
	{
		if (commands[t] == L'"')
		{
			inquotes = !inquotes;
		}
		else if (commands[t] == L' ')
		{
			if (!inquotes)
			{
				CommandTokens.push_back(token);

				token = L"";
			}
			else
			{
				token += L" ";
			}
		}
		else
		{
			token += input[t];
		}
	}

	if (token != L"")
	{
		CommandTokens.push_back(token);
	}
}


void CommandHandler::ExecuteCommand(Command c)
{
	bool ReceivedOutput = false;

	Output::Lines.clear();

	switch (c.primary)
	{
	case PrimaryCommand::Help:
		Help::ProcessCommand(c);
		break;

	case PrimaryCommand::Process:

		if (c.secondary == L"help")
		{
			ProcessUserContentHelp();
		}
		else
		{
			std::wcout << L"  " << ProcessUserContent(c.fullcommandstr) << "\n";
		}

		break;

	case PrimaryCommand::Stats:
	{
		GHoardHandler->Stats(c);
		GNamesHandler->Stats(c);
		GNPCHandler->Stats(c);
		GShopHandler->Stats(c);
		GTableHandler->Stats(c);
		break;
	}
	case PrimaryCommand::Name:
		GNamesHandler->ProcessCommand(c);
		break;
	case PrimaryCommand::Hoard:
		GHoardHandler->ProcessCommand(c);
		break;
	case PrimaryCommand::DiceRoll:
		GDiceHandler->ProcessCommand(c);
		break;
	case PrimaryCommand::NPC:
		GNPCHandler->ProcessCommand(c);
		break;
	case PrimaryCommand::Table:
		ReceivedOutput = GTableHandler->ProcessCommand(c);
		break;
	case PrimaryCommand::Shop:
		GShopHandler->ProcessCommand(c);
		break;

	default:
		std::wcout << L"  Unknown command \"" << c.primarystr << "\".\n";
		break;
	}

	if (ReceivedOutput)
	{
		for (int t = 0; t < Output::Lines.size(); t++)
		{
			std::wcout << ProcessUserContentTableRoll(Output::Lines[t]) << "\n";
		}

		std::wcout << "\n";
	}
}

// this function will look for a name token, signified by $category
// (where category is one of the available name categories (eg fantasy))
// then replace the token with a randomly generated name from the specified category

std::wstring CommandHandler::ProcessUserContent(const std::wstring input)
{
	std::wstring output(input);

	auto pos = output.find(L"$");

	while (pos != std::wstring::npos)
	{
		auto end = output.find(L" ", pos);

		if (end != std::wstring::npos)
		{
			std::wstring category = output.substr(pos + 1, end - pos);

			std::wstring name = GNamesHandler->GetName(category, L"");

			output.replace(pos, end, name);
		}
		else
		{
			std::wstring category = output.substr(pos + 1);

			std::wstring name = GNamesHandler->GetName(category, L"");

			output.replace(pos, output.length() - 1, name);
		}

		pos = output.find(L"$");
	}

	output = ProcessUserContentTableRoll(output);

	return output;
}

// this looks for a rollable option in a returned table item; rolls it, and returns the correct selection:
// eg A blade with a carved hilt (d6): 1. ivory; 2. jade; 3. soapstone; 4. ebony; 5. mahogany; 6. oak.
// will see the 6 options, and select one at random. it will then return this:
// A blade with a carved hilt + selected item minus unnecessary characters (1. ; etc.)

std::wstring CommandHandler::ProcessUserContentTableRoll(const std::wstring input)
{
	std::wstring output(input);

	std::transform(output.begin(), output.end(), output.begin(), ::tolower);

	auto pos = output.find(L"(d");

	if (pos != std::wstring::npos)
	{
		auto diceend = output.find(L')');

		if (diceend != std::wstring::npos)
		{
			int dice = stoi(output.substr(pos + 2, diceend - pos - 2));

			int rd = 1 + rand() % dice;

			auto select_start = output.find(std::to_wstring(rd) + L".");

			if (select_start != std::wstring::npos)
			{
				auto select_end = output.find(std::to_wstring(rd + 1) + L".");

				if (select_end != std::wstring::npos)
				{
					return input.substr(0, pos) + input.substr(select_start + 2, select_end - select_start - 2);
				}

				return input.substr(0, pos) + input.substr(select_start + 2);
			}
		}
	}

	return input;
}


void CommandHandler::ProcessUserContentHelp()
{
	std::wcout << L"  @ $name\n";
	std::wcout << L"    where name is one of the loaded name categories (see \"name list\")\n";
	std::wcout << "\n";
}