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


class Name
{
public:
	std::wstring Description = L"";

	std::vector<std::wstring> Titles;
	std::vector<std::wstring> Names;
	std::vector<std::wstring> Adjectives;
	std::vector<std::wstring> Nouns;
	std::vector<std::wstring> Other;
	std::vector<std::wstring> Templates;

	void Clear();
};