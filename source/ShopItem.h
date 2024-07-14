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


class ShopItem
{
public:
	std::wstring Name = L"";
	std::wstring Description = L"-";
	std::wstring Armour = L"-";
	std::wstring Damage = L"-";
	std::wstring Interval = L"-";
	std::wstring Properties = L"-";
	std::wstring Strength = L"-";
	std::wstring Stealth = L"-";
	std::wstring Weight = L"-";
	std::wstring Cost = L"-";
	std::wstring CostTo = L"-";
	int Currency = 0;

	std::wstring NameLC = L"-";

	bool SearchHit; // to do, might not be neededz
};