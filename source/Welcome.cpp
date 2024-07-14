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

#include <iostream>

#include "Constants.h"
#include "Welcome.h"


namespace Welcome
{
	void Header()
	{
		std::wcout << L"==================================================\n";
		std::wcout << L"  " << Constants::ToTCVersion << L" :: " << Constants::ToTCDate << "\n";
		std::wcout << L"==================================================\n\n";
	}
};