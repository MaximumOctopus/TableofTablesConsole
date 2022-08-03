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


class HoardCoin
{
public:

	std::wstring Description = L"";

	// loaded from file
	double Weight = 0;
	double Density = 0;
	double Radius = 0;
	double Thickness = 0;
	double Volume =  0;
	double PackedVolume = 0;
	double PackingDensity = 0;

	// calculated
	double CoinsPerLitre = 0;

	void Clear();
};