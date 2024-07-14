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

#include "Settings.h"
#include "Utility.h"


Settings* GSettings;


Settings::Settings(std::wstring path)
{
	AppPath = Utility::ExtractFolder(path) + L"\\";
	DataPath = AppPath + L"Data\\";

	std::wcout << " - " << AppPath << "\n";
}