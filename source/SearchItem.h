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


enum class SearchDomain { None = 0, Hoard = 1, Name = 2, NPC = 3, Shop = 4, Table = 5 };


struct SearchItem
{
	std::wstring Description = L"";

	int Level1 = 0;
	int Level2 = 0;
	int Level3 = 0;
};


struct SearchObject
{
	std::wstring OriginalCommand = L"";

	SearchDomain Domain = SearchDomain::None;

	std::vector<SearchItem> SearchItems;
};