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


namespace ToTUtility
{
	int GetDicePrefix(std::wstring);

	std::wstring RemoveDicePrefix(std::wstring);

	std::wstring GetCostFrom(const std::wstring);
	int GetCurrency(const std::wstring);
}