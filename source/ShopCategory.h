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


#include "ShopSubCategory.h"


class ShopCategory
{
public:

	std::wstring Description = L"";
	std::wstring DescriptionLC = L"";

	std::vector<ShopSubCategory> SubCategories;
};