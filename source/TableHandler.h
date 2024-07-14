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


#include "BaseHandler.h"
#include "Group.h"
#include "TableCategory.h"
#include "Table.h"


class TableHandler: public BaseHandler
{
private:

	enum class RowType {
		Unknown = 0, Comment = 1, TopLevelOpen = 2, TopLevelClose = 3, SecondaryOpen = 4, SecondaryClose = 5, TertiaryOpen = 6, TertiaryClose = 7,
		Dice = 8, Item = 9, Name = 10
	};

	enum class ProcessingMode { Waiting = 0, TopLevelTable = 0, SecondaryTable = 1, TertiaryTable = 2 };

	int CurrentTableID = 0;

	virtual void List(Command);
	virtual void Show(Command);
	virtual void Help();

	virtual bool Search(const std::wstring, bool);
	bool SearchIn(const std::wstring);
	virtual bool SearchShow();

	bool RollTable(int);
	bool RollTableAllMembers(int);
	bool RollTableAllMembers(std::wstring);

	bool ShowTable(int);

	bool PopulateCategory(const std::wstring);
	bool AddTable(const std::wstring, int);

	TableGroup GetFromSubCategoryName(std::wstring);

	RowType GetRowType(std::wstring);

public:

	std::vector<Table> Tables;
	std::vector<TableCategory> Categories;
	std::vector<TableSubCategory> SubCategories;

	TableHandler(const std::wstring);

	virtual bool ProcessCommand(Command);

	virtual void Stats(Command);

	int FindSubCategoryItem(int, int);
	int FindSubCategoryFrom(const std::wstring, const std::wstring);
};