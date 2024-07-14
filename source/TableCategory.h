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


struct TableCategory
{
	// this is a tidied version of the folder name 
	// eg <install>\data\tables\<category>
	std::wstring Title = L"";
};


struct TableSubCategory
{
	int CategoryID = -1;
	int SubCategoryID = -1;

	// this is a tidied version of the source file name of this table
	// eg <install>\data\tables\<category>\<file name>.txt
	std::wstring Title = L"";

	//Favourites: TStringList;
};