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

#include <algorithm>
#include <iostream>
#include <string>
#include <cwctype>

#include "ToTUtility.h"


namespace ToTUtility
{
	int GetDicePrefix(std::wstring input)
	{
		auto pos = input.find_first_of(L" ");

		if (pos != std::wstring::npos)
		{
			return stoi(input.substr(1, pos - 1));
		}

		return 0;
	}


	std::wstring RemoveDicePrefix(std::wstring input)
	{
		auto pos = input.find_first_of(L' ');

		if (pos != std::wstring::npos)
		{
			return input.substr(pos + 1);
		}

		return L""; // to do
	}


	std::wstring GetCostFrom(const std::wstring input)
	{
		std::wstring cost(input);
		std::wstring output = L"";

		std::transform(cost.begin(), cost.end(), cost.begin(), ::tolower);

		if (cost == L"special")
		{
			return cost;
		}
		else
		{
			for (int t = 0; t < cost.length(); t++)
			{
				if (std::iswdigit(cost[t]) || cost[t] == L'.' || cost[t] == L',')
				{
					output += cost[t];
				}
				else
				{
					return output;
				}
			}
		}

		return output;
	}


	int GetCurrency(const std::wstring input)
	{
		std::wstring currency(input);
		std::wstring output = L"";

		std::transform(currency.begin(), currency.end(), currency.begin(), ::tolower);

		if (currency == L"special")
		{
			return 0;
		}
		else
		{
			for (int t = 0; t < currency.length(); t++)
			{
				if (std::iswalpha(currency[t]))
				{
					output += currency[t];
				}
			}
		}

		if (output == L"cp")
		{
			return 1;
		}
		else if (output == L"sp")
		{
			return 2;
		}
		else if (output == L"gp")
		{
			return 3;
		}
		else if (output == L"pp")
		{
			return 4;
		}

		return 0;
	}
}