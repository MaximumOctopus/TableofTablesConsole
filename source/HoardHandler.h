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
#include "HoardCoin.h"


class HoardHandler: public BaseHandler
{
    enum class RowType {
        Unknown = 0, Comment = 1, SectionOpen = 2, SectionClose = 3, Description = 4, Weight = 5,
        Density = 6, Radius = 7, Thickness = 8, Volume = 9, PackedVolume = 10, PackingDensity = 11
    };

	enum class Units { Metric = 0, Imperial = 1};

	enum class ProcessingMode { ReadyForSection = 0, SectionFound = 1 };

	int units = 0;

	double GetHoardWeight(int, int, int);
	double GetHoardVolume(int, int, int);

	void Add(HoardCoin hoard);

	bool PopulateFromFile(const std::wstring);

	std::wstring GetMassUnits(int);
	std::wstring GetVolumeUnits(int);

	virtual void List(Command);
	virtual void Help();

	virtual bool Search(const std::wstring, bool);
	virtual bool SearchShow();

public:

	std::vector<HoardCoin> Hoards;

	HoardHandler(const std::wstring);

	bool ProcessCommand(Command);

	HoardHandler::RowType GetRowType(const std::wstring);

	virtual void Stats(Command);
};