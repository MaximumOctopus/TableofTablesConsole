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


#include <vector>

#include "BaseHandler.h"
#include "NPC.h"


class NPCHandler: public BaseHandler
{
private:

	enum class RowType { Unknown = 0, Comment = 1, SectionOpen = 2, SectionClose = 3, Data = 4, Description = 5 };
	enum class ProcessingMode { ReadyForSection = 0, SectionFound = 1 };

	virtual void List(Command);
	virtual void Help();

	virtual bool Search(const std::wstring input, bool);
	virtual bool SearchShow();

	bool Load(const std::wstring);

	bool PopulateFromFile(const std::wstring);

	bool AddComponent(std::vector<std::wstring>&, const std::wstring);
	bool AddSubItems(NPC& npc, const std::wstring);

	RowType GetRowType(const std::wstring);

public:

	std::vector<NPC> NPCs;

	NPCHandler(const std::wstring);

	virtual bool ProcessCommand(Command);

	virtual void Stats(Command);
};