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


#include <string>


static const int kCommandCount = 11; 

enum class PrimaryCommand { None = 0, Exit = 1, Help = 2, Process = 3, Stats = 4, Name = 5, Hoard = 6, DiceRoll = 7, NPC = 8, Table = 9, Shop = 10 };

static const std::wstring ValidCommandText[kCommandCount] = { L"", L"exit", L"help", L"@", L"stats", L"name", L"hoard", L"roll", L"npc", L"table", L"shop"};

static const PrimaryCommand ValidCommands[kCommandCount] = { PrimaryCommand::None, PrimaryCommand::Exit, PrimaryCommand::Help, PrimaryCommand::Process, PrimaryCommand::Stats,
	PrimaryCommand::Name, PrimaryCommand::Hoard, PrimaryCommand::DiceRoll, PrimaryCommand::NPC, PrimaryCommand::Table, PrimaryCommand::Shop 
};


struct Command
{
	PrimaryCommand primary = PrimaryCommand::None;

	std::wstring primarystr = L"";
	std::wstring fullcommandstr = L"";

	std::wstring secondary = L"";
	std::wstring tertiary = L"";
	std::wstring quaternary = L"";

	std::wstring GetFullCommand();
};