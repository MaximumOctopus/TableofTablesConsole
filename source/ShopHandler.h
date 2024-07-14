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

#include "BaseHandler.h"
#include "Command.h"
#include "ShopCategory.h"


class ShopHandler: public BaseHandler
{
	enum class RowType {
		Unknown = 0, Comment = 1, SectionOpen = 2, SectionClose = 3, Armour = 4, Cost = 5, Description = 6, Stealth = 7,
		Interval = 8, Damage = 9, Name = 10, Properties = 11, Strength = 12, Weight = 13, CostTo = 14
	};
	enum class ProcessingMode { ReadyForSection = 0, SectionFound = 1 };

	virtual void Help();
	virtual void List(Command);
	virtual void Show(Command);

	bool ShowItem(int, int, int);

	virtual bool Search(const std::wstring input, bool);
	virtual bool SearchIn(const std::wstring);
	virtual bool SearchShow();

	bool LoadShop(const std::wstring);

	bool PopulateShop(const std::wstring, const std::wstring);
	
	bool AddShop(const std::wstring);

	void AddSubCategory(const std::wstring);

	RowType GetRowType(const std::wstring);

	int GetCategoryID(const std::wstring);
	int GetSubCategoryID(int, const std::wstring);
	int GetItemID(int, int, const std::wstring);

public:

	std::vector<ShopCategory> ShopCategories;

	ShopHandler(const std::wstring folder);

	virtual bool ProcessCommand(Command);

	virtual void Stats(Command);
};