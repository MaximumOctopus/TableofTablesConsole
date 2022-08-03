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

#pragma once


#include <string>
#include <vector>

#include "ItemResponse.h"


class Table
{
public:

	enum class Mode { Unknown = 0, OneToOne = 1, Range = 2, All = 3, Title = 4 };

	std::wstring Name = L"";
	std::wstring Title = L"";

	int CategoryID = -1;
	int SubID = -1;
	int TableID = -1;
	Mode Mode = Mode::Unknown;
	int DiceRoll = -1;

	std::vector<std::wstring> Items;

	ItemResponse GetItem(int, int);

	int GetRandomValue();
};