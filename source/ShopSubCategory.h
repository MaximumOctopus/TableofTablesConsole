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

#include "ShopItem.h"


class ShopSubCategory
{
public:

	std::vector<ShopItem> Items;

	std::wstring Description = L"";
	std::wstring DescriptionLC = L"";
};