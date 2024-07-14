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

#include "HoardCoin.h"


void HoardCoin::Clear()
{
	Description = L"";

	Weight = 0;
	Density = 0;
	Radius = 0;
	Thickness = 0;
	Volume = 0;
	PackedVolume = 0;
	PackingDensity = 0;

	CoinsPerLitre = 0;
}