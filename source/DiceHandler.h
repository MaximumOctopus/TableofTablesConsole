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


class DiceHandler
{
	Command Last;

	std::wstring Roll(int, int);
	std::wstring RollSpecial(int, int, std::wstring);

	void Help();

public:

	bool ProcessCommand(Command);

	
};