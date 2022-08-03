//
// Table of Tables Console 2
//
// (c) Paul Alan Freshney 2016-2022
//   paul@freshney.org
//
// Inspired/based on the Table of Tables sub-reddit.
//   https://www.reddit.com/r/BehindTheTables/
//
// Source code and executables:
//   https://github.com/MaximumOctopus/TableofTablesConsole
//

#pragma once


#include <string>
#include <vector>

#include "Command.h"
#include "Favourite.h"
#include "SearchItem.h"


class BaseHandler
{
	
protected:

	std::wstring RootFolder = L"";
	std::wstring FilePrefix = L"";

	std::vector<Favourite> Favourites;

	SearchObject SearchResults;

	Command Last;

	std::vector<std::wstring> FoundFolders;
	std::vector<std::wstring> FoundFiles;

	bool GenerateFileList(const std::wstring);
	bool GenerateFolderList(const std::wstring);

	virtual bool ProcessCommand(Command);
	bool HandleOtherTasks(Command);

	virtual void Help() = 0;
	virtual void List(Command) = 0;
	virtual void Stats(Command) = 0;

	virtual bool Search(const std::wstring, bool) = 0;
	virtual bool SearchShow() = 0;

	bool ShowFavourites();

	bool LoadSearchResults(const std::wstring);
	bool SaveSearchResults(const std::wstring);

	bool LoadFavourites(const std::wstring);
	bool SaveFavourites(const std::wstring);

public:

	BaseHandler();

	~BaseHandler();
};