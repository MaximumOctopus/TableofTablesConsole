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

#include "Name.h"


void Name::Clear()
{
	Description = L"";

	Titles.clear();
	Names.clear();
	Adjectives.clear();
	Nouns.clear();
	Other.clear();
	Templates.clear();
}