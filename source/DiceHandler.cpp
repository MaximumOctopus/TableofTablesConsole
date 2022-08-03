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

#include <iostream>
#include <string>

#include "Command.h"
#include "DiceHandler.h"


DiceHandler* GDiceHandler;


bool DiceHandler::ProcessCommand(Command c)
{
    if (c.primary == PrimaryCommand::Name)
    {
        if (c.secondary == L"help")
        {
            Help();
        }
        else
        {
            std::wstring sides = L"";
            std::wstring count = L"";

            if (c.secondary == L"")
            {
                sides = Last.secondary;
            }
            else
            {
                sides = c.secondary;
            }

            if (c.tertiary == L"")
            {
                count = Last.tertiary;
            }
            else
            {
                count = c.tertiary;
            }

            int ss = stod(sides);
            int ct = stoi(count);

            std::wcout << Roll(ss, ct) << "\n";
        }

        Last = c;

        return true;
    }

    return false;
}


std::wstring DiceHandler::Roll(int sides, int count)
{
    if (count <= 1)
    {
        int r = 1 + rand() % sides;

        return std::to_wstring(r);
    }
    else
    {
        std::wstring output = L"";
        int total = 0;

        for (int t = 0; t < count; t++)
        {
            int r = 1 + rand() % sides;

            total += r;

            output += std::to_wstring(r) + L" ";
        }

        return output + L"  (total " + std::to_wstring(total) + L")";
    }
}


void DiceHandler::Help()
{
    std::wcout << L"  dice sides count\n\n";
    std::wcout << L"  eg to simulate 2 D4s: dice 4 2\n\n";
    std::wcout << "\n";
}