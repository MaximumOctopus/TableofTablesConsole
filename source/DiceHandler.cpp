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

#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

#include "Command.h"
#include "DiceHandler.h"


DiceHandler* GDiceHandler;


bool DiceHandler::ProcessCommand(Command c)
{
    if (c.primary == PrimaryCommand::DiceRoll)
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
                if (Last.secondary.empty())
                {
                    sides = 6;
                }
                else
                {
                    sides = Last.secondary;
                }
            }
            else
            {
                sides = c.secondary;
            }

            if (c.tertiary == L"")
            {
                if (Last.tertiary.empty())
                {
                    count = 1;
                }
                else
                {
                    count = Last.tertiary;
                }
            }
            else
            {
                count = c.tertiary;
            }

            int ss = stoi(sides);
            int ct = stoi(count);

            if (c.quaternary.empty())
            {
                std::wcout << L"       " << Roll(ss, ct) << "\n";
            }
            else
            {
                std::wcout << L"       " << RollSpecial(ss, ct, c.quaternary) << "\n";
            }
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
        int high = 0;
        int low = 999999;

        for (int t = 0; t < count; t++)
        {
            int r = 1 + rand() % sides;

            total += r;

            output += std::to_wstring(r) + L" ";

            if (r > high) high = r;
            if (r < low) low = r;
        }

        return output + L"  (total " + std::to_wstring(total) + L"; h: " + std::to_wstring(high) + L" l: " + std::to_wstring(low) + L")";
    }
}


std::wstring DiceHandler::RollSpecial(int sides, int count, std::wstring special)
{
    if (special.empty()) return L"";

    bool show = 0; // bottom

    if (special[0] == L't') // top
    {
        show = 1;
    }

    int range = stoi(special.substr(1));

    if (count <= 1)
    {
        int r = 1 + rand() % sides;

        return std::to_wstring(r);
    }
    else
    {
        std::vector<int> rolls;
        std::wstring output = L"";

        for (int t = 0; t < count; t++)
        {
            int r = 1 + rand() % sides;

            rolls.push_back(r);
        }

        std::sort(rolls.begin(), rolls.end());

        switch (show)
        {
        case 0: // bottom
        {
            int index = 0;
            int n = 0;

            while (index < rolls.size() && n < range)
            {
                output += std::to_wstring(rolls[index]) + L" ";

                index++;

                n++;
            }

            break;
        }
        case 1: // top
        {
            int index = rolls.size() - 1;
            int n = 0;

            while (index >= 0 && n < range)
            {
                output += std::to_wstring(rolls[index]) + L" ";

                index--;

                n++;
            }

            break;
        }
        }

        return output;
    }
}


void DiceHandler::Help()
{
    std::wcout << L"  roll sides count\n\n";
    std::wcout << L"    eg to simulate 2 D4s: roll 4 2\n\n";
    std::wcout << L"  roll sides count tn\n";
    std::wcout << L"    show the n highest rolls\n";
    std::wcout << L"  roll sides count bn\n";
    std::wcout << L"    show the n lowest rolls\n";
    std::wcout << "\n";
}