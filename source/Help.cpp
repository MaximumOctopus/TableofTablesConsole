//
// Table of Tables Console 2
//
// (c) Paul Alan Freshney 2016-2022
//   paul@freshney.org
//
// Inspired/based on the Behind the Tables sub-reddit.
//   https://www.reddit.com/r/BehindTheTables/
//
// Source code and executables:
//   https://github.com/MaximumOctopus/TableofTablesConsole
//

#include "Help.h"


#include <iostream>


void Help::ProcessCommand(Command c)
{
	if (c.primary == PrimaryCommand::Help)
	{
		std::wcout << L"  Available commands: roll, hoard, name, npc, shop, table\n";
		std::wcout << L"    all main commands have the options: list, help, and stats\n\n";
		std::wcout << "\n";
		std::wcout << L"    roll sides [count]\n";
		std::wcout << L"    hoard w(eight)|v(olume) type count\n";
		std::wcout << L"    name category\n";
		std::wcout << L"    npc\n";
		std::wcout << L"    shop\n";
		std::wcout << L"    table\n";
		std::wcout << L"\n";
		std::wcout << L"  see the included pdf for more information";
		std::wcout << L"\n";
	}
}