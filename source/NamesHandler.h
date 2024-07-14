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


#include <vector>

#include "BaseHandler.h"
#include "Command.h"
#include "Name.h"


struct Statistics
{
	int Titles = 0;
	int Names = 0;
	int Adjectives = 0;
	int Nouns = 0;
	int Other = 0;
	int Templates = 0;
};


class NamesHandler: public BaseHandler
{
private:

	enum class RowType { Unknown = 0, Comment = 1, SectionOpen = 2, SectionClose = 3, Data = 4, CategoryChange = 5 };
	enum class ProcessingMode { ReadyForSection = 0, SectionFound = 1 };
	enum class Category { Unknown = 0, Titles = 1, Names = 2, Adjectives = 3, Nouns = 4, Other = 5, Templates = 6 };

	Statistics NameStats;

	virtual void Show(Command);

	virtual bool Search(const std::wstring input, bool);
	virtual bool SearchShow();

	void Add(Name name);

	bool PopulateFromFile(const std::wstring);

	RowType GetRowType(const std::wstring);

	int GetNameIndex(std::wstring);

	virtual void Help();
	virtual void List(Command);

public:

	std::vector<Name> Names;

	NamesHandler(const std::wstring);

	virtual bool ProcessCommand(Command);

	std::wstring GetName(const std::wstring, const std::wstring);

	virtual void Stats(Command);
};