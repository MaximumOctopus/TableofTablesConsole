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
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <Windows.h>

#include "Command.h"
#include "HoardHandler.h"
#include "Utility.h"


HoardHandler* GHoardHandler;


HoardHandler::HoardHandler(const std::wstring install_folder)
{
    RootFolder = install_folder;
    FilePrefix = L"hoard";

    PopulateFromFile(install_folder + L"Misc\\Hoard\\Hoard Calculator.txt");

    Last.primary = PrimaryCommand::Hoard;
    Last.secondary = L"w";
    Last.tertiary = L"0";
    Last.quaternary = L"1000";
}


bool HoardHandler::ProcessCommand(Command c)
{
    if (c.primary == PrimaryCommand::Hoard)
    {
        std::wstring category = L"";
        int coin_type = 0;
        int coin_count = 0;

        if (HandleOtherTasks(c))
        {
            Last = c;

            return true;
        }

        if (c.secondary == L"")
        {
            category = Last.secondary;
        }
        else
        {
            category = c.secondary;
        }

        if (c.tertiary == L"")
        {
            coin_type = stoi(Last.tertiary);
        }
        else
        {
            coin_type = stoi(c.tertiary);
        }

        if (c.quaternary == L"")
        {
            coin_count = stoi(Last.tertiary);
        }
        else
        {
            coin_count = stoi(c.tertiary);
        }

        if (category[0] == L'w')
        {
            std::wcout << GetHoardWeight(coin_type, coin_count, units) << L" " << GetMassUnits(units) << "\n";
        }
        else
        {
            std::wcout << GetHoardVolume(coin_type, coin_count, units) << L" " << GetVolumeUnits(units) << "\n";
        }

        Last = c;

        return true;
    }

    return false;
}


bool HoardHandler::Search(const std::wstring input, bool special)
{
    return false;
}


bool HoardHandler::SearchShow()
{
    return false;
}


void HoardHandler::Help()
{
    std::wcout << L"  hoard help\n";
    std::wcout << L"  hoard list\n";
    std::wcout << L"  hoard stats\n\n";
    std::wcout << L"  hoard w[eight] coin_type count\n";
    std::wcout << L"  hoard v[olume] coin_type count\n";
    std::wcout << L"      where coin_type: 0 = copper, 1 = silver, 2 = gold, 3 = platinum\n";
    std::wcout << "\n";
}


void HoardHandler::List(Command c)
{
    for (int t = 0; t < Hoards.size(); t++)
    {
        std::wcout << "\"" << Hoards[t].Description << "\"\n";
    }
}


void HoardHandler::Stats(Command c)
{
    std::wcout << L"  Hoards :" << Hoards.size() << "\n";
    std::wcout << "\n";
}


double HoardHandler::GetHoardWeight(int coin_type, int coin_count, int unit)
{
    std::wcout << coin_type << " " << coin_count << "\n";

    switch (unit)
    {
    case 0:
        return (double)coin_count * (Hoards[coin_type].Weight / 1000.0f);
        break;
    case 1:
        return (double)coin_count * (Hoards[coin_type].Weight / 1000.0f) * 2.20462262f;
        break;
    }

    return 0;
}


 double HoardHandler::GetHoardVolume(int coin_type, int coin_count, int unit)
 {
     switch (unit)
     {
     case 0:
         return coin_count / (Hoards[coin_type].CoinsPerLitre * 1000);
         break;
     case 1:
         return coin_count / (Hoards[coin_type].CoinsPerLitre * 1000) * 35.3147;
         break;
     }

     return 0;
}


void HoardHandler::Add(HoardCoin hoard)
{
    Hoards.push_back(hoard);
}


bool HoardHandler::PopulateFromFile(const std::wstring file_name)
{
    std::wifstream file(file_name);

    file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    if (file)
    {
        ProcessingMode mode = ProcessingMode::ReadyForSection;

        std::wstring s;

        int line = 1;

        HoardCoin hoard;

        while (std::getline(file, s))
        {
            if (s != L"")
            {
                if (s[0] == L'/' || s[1] == L';')
                {
                    // comment, do nothing
                }
                else
                {
                    RowType current = GetRowType(s);

                    switch (current)
                    {
                    case RowType::SectionOpen:
                        mode = ProcessingMode::SectionFound;
                        break;
                    case RowType::SectionClose:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Add(hoard);

                            mode = ProcessingMode::ReadyForSection;

                            hoard.Clear();
                        }
                        else
                        {
                            std::wcout << L"Unpaired { } in " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }
                        break;
                    }
                    case RowType::Description:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            hoard.Description = s.substr(2);
                        }
                        else
                        {
                            std::wcout << L"\"n:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    }
                    case RowType::Weight:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            hoard.Weight = std::stod(s.substr(2));
                        }
                        else
                        {
                            std::wcout << L"\"w:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    }
                    case RowType::Density:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            hoard.Density = std::stod(s.substr(2));
                        }
                        else
                        {
                            std::wcout << L"\"d:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    }
                    case RowType::Radius:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            hoard.Radius = std::stod(s.substr(2));
                        }
                        else
                        {
                            std::wcout << L"\"r:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    }
                    case RowType::Thickness:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            hoard.Thickness = std::stod(s.substr(2));
                        }
                        else
                        {
                            std::wcout << L"\"t:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    }
                    case RowType::Volume:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            hoard.Volume = std::stod(s.substr(2));
                        }
                        else
                        {
                            std::wcout << L"\"v:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    }
                    case RowType::PackedVolume:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            hoard.PackedVolume = std::stod(s.substr(2));
                        }
                        else
                        {
                            std::wcout << L"\"p:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    }
                    case RowType::PackingDensity:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            hoard.PackingDensity = std::stod(s.substr(2));
                        }
                        else
                        {
                            std::wcout << L"\"q:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    }
                    }
                }
            }

            line++;
        }

        file.close();
    }
    else
    {
        std::wcout << L"  Hoard file not found \"" << file_name << "\".\n";
    
        return false;
    }

    return true;
}


HoardHandler::RowType HoardHandler::GetRowType(const std::wstring row)
{
    if (row[0] == L'n')
    {
        return RowType::Description;
    }
    else if (row[0] == L'w')
    {
        return RowType::Weight;
    }
    else if (row[0] == L'd')
    {
        return RowType::Density;
    }
    else if (row[0] == L'r')
    {
        return RowType::Radius;
    }
    else if (row[0] == L't')
    {
        return RowType::Thickness;
    }
    else if (row[0] == L'v')
    {
        return RowType::Volume;
    }
    else if (row[0] == L'p')
    {
        return RowType::PackedVolume;
    }
    else if (row[0] == L'q')
    {
        return RowType::PackingDensity;
    }
    else if (row[0] == L'{')
    {
        return RowType::SectionOpen;
    }
    else if (row[0] == L'}')
    {
        return RowType::SectionClose;
    }

    return RowType::Unknown;
}


std::wstring HoardHandler::GetMassUnits(int units)
{
    if (units == 0)
    {
        return L"kg";
    }

    return L"lbs";
}


std::wstring HoardHandler::GetVolumeUnits(int units)
{
    if (units == 0)
    {
        return L"m^3";
    }
    
    return L"ft^3";
}