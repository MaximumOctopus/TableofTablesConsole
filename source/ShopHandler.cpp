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
#include <codecvt>
#include <fstream>
#include <iostream>
#include <locale>
#include <string>
#include <vector>
#include <Windows.h>

#include "ShopHandler.h"
#include "ToTUtility.h"
#include "Utility.h"


ShopHandler* GShopHandler;


ShopHandler::ShopHandler(const std::wstring folder)
{
    RootFolder = folder;
    FilePrefix = L"shop";

	LoadShop(folder + L"Shop\\");
}


bool ShopHandler::ProcessCommand(Command c)
{
    if (c.primary == PrimaryCommand::Shop)
    {
        if (HandleOtherTasks(c))
        {
            Last = c;

            return true;
        }

        if (c.secondary == L"search")
        {
            Search(c.tertiary, false);
        }
        else if (c.secondary == L"searchin")
        {
            SearchIn(c.tertiary);
        }
        else if (c.secondary != L"")
        {
            int cat = GetCategoryID(c.secondary);

            if (cat != -1)
            {
                int subcat = GetSubCategoryID(cat, c.tertiary);

                if (subcat != -1)
                {
                    int item = GetItemID(cat, subcat, c.quaternary);

                    if (item != -1)
                    {
                        ShowItem(cat, subcat, item);
                    }
                }
            }
        }

        Last = c;

        return true;
    }

    return false;
}


bool ShopHandler::ShowItem(int cat, int sub_cat, int item)
{
    if (cat < ShopCategories.size())
    {
        if (sub_cat < ShopCategories[cat].SubCategories.size())
        {
            if (item < ShopCategories[cat].SubCategories[sub_cat].Items.size())
            {
                ShopItem shopitem = ShopCategories[cat].SubCategories[sub_cat].Items[item];

                if (shopitem.Name != L"")           std::wcout << L"  Name        : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].Name << "\n";
                if (shopitem.Description != L"")    std::wcout << L"  Description : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].Description << "\n";
                if (shopitem.Armour != L"")         std::wcout << L"  Armour      : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].Armour << "\n";
                if (shopitem.Damage != L"")         std::wcout << L"  Damage      : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].Damage << "\n";
                if (shopitem.Interval != L"")       std::wcout << L"  Interval    : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].Interval << "\n";
                if (shopitem.Properties != L"")     std::wcout << L"  Properties  : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].Properties << "\n";
                if (shopitem.Strength != L"")       std::wcout << L"  Strength    : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].Strength << "\n";
                if (shopitem.Stealth != L"")        std::wcout << L"  Stealth     : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].Stealth << "\n";
                if (shopitem.Weight != L"")         std::wcout << L"  Weight      : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].Weight << "\n";
                if (shopitem.Cost != L"")           std::wcout << L"  Cost        : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].Cost << "\n";
                if (shopitem.CostTo != L"")         std::wcout << L"  Cost To     : " << ShopCategories[cat].SubCategories[sub_cat].Items[item].CostTo << "\n";
                
                std::wcout << "\n";
               // int Currency = 0;
                return true;
            }
        }
    }

    return false;
}


bool ShopHandler::Search(const std::wstring input, bool special)
{
    if (input != L"")
    {
        SearchObject so;

        std::wstring search(input);

        std::transform(search.begin(), search.end(), search.begin(), ::tolower);

        std::wcout << L"  cat sub item detail\n";

        for (int c = 0; c < ShopCategories.size(); c++)
        {
            for (int s = 0; s < ShopCategories[c].SubCategories.size(); s++)
            {
                for (int i = 0; i < ShopCategories[c].SubCategories[s].Items.size(); i++)
                {
                    if (ShopCategories[c].SubCategories[s].Items[i].NameLC.find(search) != std::wstring::npos)
                    {
                        SearchItem si;

                        si.Description = ShopCategories[c].SubCategories[s].Items[i].Name;
                        si.Level1 = c;
                        si.Level2 = s;
                        si.Level3 = i;

                        so.SearchItems.push_back(si);

                        std::wcout << L"  " << Utility::PadRight(c, 3) << L" " << Utility::PadRight(s, 3) << L" " << Utility::PadRight(i, 3) << L"  " << ShopCategories[c].SubCategories[s].Items[i].Name << "\n";
                    }
                }
            }
        }

        if (so.SearchItems.size() != 0)
        {
            SearchResults = so;
        }
        else
        {
            std::wcout << L"  No matching items found.\n";
        }

        std::wcout << "\n";

        return true;
    }

    return false;
}


bool ShopHandler::SearchIn(const std::wstring input)
{
    if (SearchResults.SearchItems.size() != 0)
    {
        std::wstring search(input);

        std::transform(search.begin(), search.end(), search.begin(), ::tolower);

        int count = 0;

        int cat = 0;
        int subcat = 0;
        int item = 0;

        for (int t = 0; t < SearchResults.SearchItems.size(); t++)
        {
            cat = SearchResults.SearchItems[t].Level1;
            subcat = SearchResults.SearchItems[t].Level2;
            item = SearchResults.SearchItems[t].Level3;

            if (ShopCategories[cat].SubCategories[subcat].Items[item].NameLC.find(search) != std::wstring::npos)
            {
                std::wcout << L"  " << Utility::PadRight(cat, 3) << L" " << Utility::PadRight(subcat, 3) << L" " << Utility::PadRight(item, 3) << L"  " << ShopCategories[cat].SubCategories[subcat].Items[item].Name << "\n";
            
                count++;
            }
        }

        if (count == 0)
        {
            std::wcout << L"  No matching items found.\n\n";
        }

        return true;
    }
    else
    {
        std::wcout << L"  No search results to filter.\n\n";
    }

    return false;
}


bool ShopHandler::SearchShow()
{
    if (SearchResults.SearchItems.size() != 0)
    {
        std::wcout << L" \"" << SearchResults.OriginalCommand << L"\" \n";

        for (int t = 0; t < SearchResults.SearchItems.size(); t++)
        {
            std::wcout << L"   " << ShopCategories[SearchResults.SearchItems[t].Level1].SubCategories[SearchResults.SearchItems[t].Level2].Items[SearchResults.SearchItems[t].Level3].Name << "\n";
        }

        std::wcout << "\n";

        return true;
    }

    return false;
}


void ShopHandler::Help()
{
    std::wcout << L"  shop help\n";
    std::wcout << L"  shop list\n";
    std::wcout << L"  shop stats\n\n";
    std::wcout << L"  shop search term\n";
    std::wcout << L"  shop searchin term\n";
    std::wcout << L"  shop category subcategory item\n";
    std::wcout << "\n";
}


void ShopHandler::List(Command c)
{
    if (c.tertiary == L"cat")
    {
        for (int t = 0; t < ShopCategories.size(); t++)
        {
            std::wcout << L" " << Utility::PadRight(t, 3) << L" " << ShopCategories[t].Description << "\n";
        }
    }
    else if (c.tertiary == L"subcat")
    {
        try
        {
            int sub = stoi(c.quaternary);

            for (int t = 0; t < ShopCategories[sub].SubCategories.size(); t++)
            {
                std::wcout << L" " << Utility::PadRight(t, 3) << L" " << ShopCategories[sub].SubCategories[t].Description << "\n";
            }
        }
        catch (...)
        {
            std::wcout << L" Invalid subcategory value \"" << c.quaternary << "\". Should be 0 to " << ShopCategories.size() << L".\n";
        }
    }
}

void ShopHandler::Show(Command c)
{

}


void ShopHandler::Stats(Command c)
{
    std::wcout << "  Shop Categories : " << ShopCategories.size() << "\n";
    std::wcout << "\n";
}



int ShopHandler::GetCategoryID(const std::wstring input)
{
    if (std::isalpha(input[0]))
    {
        std::wstring search(input);
        std::transform(search.begin(), search.end(), search.begin(), ::tolower);

        for (int t = 0; t < ShopCategories.size(); t++)
        {
            if (ShopCategories[t].Description.find(search) == 0)
            {
                return t;
            }
        }
    }

    try
    {
        int i = stoi(input);

        if (i < ShopCategories.size())
        {
            return i;
        }

        std::wcout << L"  invalid category value \"" << input << "\".\n";
    }
    catch (...)
    {
    }
    
    return -1;
}


int ShopHandler::GetSubCategoryID(int cat, const std::wstring input)
{
    if (std::isalpha(input[0]))
    {
        std::wstring search(input);
        std::transform(search.begin(), search.end(), search.begin(), ::tolower);

        for (int t = 0; t < ShopCategories[cat].SubCategories.size(); t++)
        {
            if (ShopCategories[cat].SubCategories[t].Description.find(search) == 0)
            {
                return t;
            }
        }
    }

    try
    {
        int i = stoi(input);

        if (i < ShopCategories[cat].SubCategories.size())
        {
            return i;
        }

        std::wcout << L"  invalid subcategory value \"" << input << "\".\n";
    }
    catch (...)
    {
    }

    return -1;
}


int ShopHandler::GetItemID(int cat, int sub_cat, const std::wstring input)
{
    if (std::isalpha(input[0]))
    {
        std::wstring search(input);
        std::transform(search.begin(), search.end(), search.begin(), ::tolower);

        for (int t = 0; t < ShopCategories[cat].SubCategories[sub_cat].Items.size(); t++)
        {
            if (ShopCategories[cat].SubCategories[sub_cat].Items[t].NameLC.find(search) == 0)
            {
                return t;
            }
        }
    }

    try
    {
        int i = stoi(input);

        if (i < ShopCategories[cat].SubCategories[sub_cat].Items.size())
        {
            return i;
        }

        std::wcout << L"  invalid item value \"" << input << "\".\n";
    }
    catch (...)
    {
    }

    return -1;
}


bool ShopHandler::LoadShop(const std::wstring folder)
{
    std::wstring filter = folder + L"\\*.*";

    WIN32_FIND_DATAW file;

    HANDLE search_handle = FindFirstFileW(filter.c_str(), &file);

    if (search_handle != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
            {
                if ((!lstrcmpW(file.cFileName, L".")) || (!lstrcmpW(file.cFileName, L"..")))
                    continue;

                std::wstring names_file(file.cFileName);

                PopulateShop(folder + names_file + L"\\", L"to do");
            }

        } while (FindNextFileW(search_handle, &file));

        FindClose(search_handle);

        return true;
    }

    return false;
}


bool ShopHandler::PopulateShop(const std::wstring folder, const std::wstring name)
{
    ShopCategory sc;
    sc.Description = name;

    std::wstring lc(name);
    std::transform(lc.begin(), lc.end(), lc.begin(), ::tolower);

    sc.DescriptionLC = lc;

    ShopCategories.push_back(sc);

    if (GenerateFileList(folder))
    {
        for (int t = 0; t < FoundFiles.size(); t++)
        {
            AddShop(folder + FoundFiles[t]);
        }
    }

    return false;
}


bool ShopHandler::AddShop(const std::wstring file_name)
{
    std::wifstream file(file_name);

    file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    if (file)
    {
        AddSubCategory(file_name);

        ProcessingMode mode = ProcessingMode::ReadyForSection;

        std::wstring s;

        int line = 1;

        std::wstring Armour = L"";
        std::wstring Cost = L"";
        int Currency = 1;
        std::wstring Description = L"";
        std::wstring Stealth = L"";
        std::wstring Interval = L"";
        std::wstring Damage = L"";
        std::wstring Name = L"";
        std::wstring Properties = L"";
        std::wstring Strength = L"";
        std::wstring Weight = L"";
        std::wstring CostTo = L"";


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
                        std::wstring lc(Name);
                        std::transform(lc.begin(), lc.end(), lc.begin(), ::tolower);

                        ShopItem shopitem;

                        shopitem.Armour = Armour;
                        shopitem.Cost = Cost;
                        shopitem.Currency = Currency;
                        shopitem.Description = Description;
                        shopitem.Stealth = Stealth;
                        shopitem.Interval = Interval;
                        shopitem.Damage = Damage;
                        shopitem.Name = Name;
                        shopitem.Properties = Properties;
                        shopitem.Strength = Strength;
                        shopitem.Weight = Weight;
                        shopitem.CostTo = CostTo;

                        shopitem.NameLC = lc; // cached lowercase text to make searching a bit faster/simpler ;)


                        ShopCategories.back().SubCategories.back().Items.push_back(shopitem);

                        mode = ProcessingMode::ReadyForSection;

                        break;
                    }
                    case RowType::Armour:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Armour = s.substr(2);
                        }
                        else
                        {
                            std::wcout << L"\"a:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    case RowType::Cost:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Cost = ToTUtility::GetCostFrom(s.substr(2));
                            Currency = ToTUtility::GetCurrency(s.substr(2));
                        }
                        else
                        {
                            std::wcout << L"\"c:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    case RowType::Description:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Description = s.substr(2);
                        }
                        else
                        {
                            std::wcout << L"\"d:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    case RowType::Stealth:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Stealth = s.substr(2);
                        }
                        else
                        {
                            std::wcout << L"\"h:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }
                        
                        break;
                    case RowType::Interval:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Interval = s.substr(2);
                        }
                        else
                        {
                            std::wcout << L"\"i:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    case RowType::Damage:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Damage = s.substr(2);
                        }
                        else
                        {
                            std::wcout << L"\"m:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    case RowType::Name:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Name = s.substr(2);
                        }
                        else
                        {
                            std::wcout << L"\"n:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    case RowType::Properties:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Properties = s.substr(2);
                        }
                        else
                        {
                            std::wcout << L"\"p:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    case RowType::Strength:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Strength = s.substr(2);
                        }
                        else
                        {
                            std::wcout << L"\"s:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }
                        
                        break;
                    case RowType::Weight:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Weight = s.substr(2);
                        }
                        else
                        {
                            std::wcout << L"\"w:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }
                        
                        break;
                    case RowType::CostTo:
                        if (mode == ProcessingMode::SectionFound)
                        {
                            CostTo = ToTUtility::GetCostFrom(s.substr(2));
                        }
                        else
                        {
                            std::wcout << L"\"z:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    }
                }
            }

            line++;
        }

        file.close();

        return true;
    }
    else
    {
        std::wcerr << "  Couldn't access Shop file \"" << file_name << "\".\n";

        return false;
    }

    return false;
}


ShopHandler::RowType ShopHandler::GetRowType(const std::wstring row)
{
    if (row[0] == L'{')
    {
        return RowType::SectionOpen;
    }
    else if (row[0] == L'}')
    {
        return RowType::SectionClose;
    }
    else if (row[0] == L'a')
    {
        return RowType::Armour;
    }
    else if (row[0] == L'c')
    {
        return RowType::Cost;
    }
    else if (row[0] == L'd')
    {
        return RowType::Description;
    }
    else if (row[0] == L'h')
    {
        return RowType::Stealth;
    }
    else if (row[0] == L'i')
    {
        return RowType::Interval;
    }
    else if (row[0] == L'm')
    {
        return RowType::Damage;
    }
    else if (row[0] == L'n')
    {
        return RowType::Name;
    }
    else if (row[0] == L'p')
    {
        return RowType::Properties;
    }
    else if (row[0] == L's')
    {
        return RowType::Strength;
    }
    else if (row[0] == L'w')
    {
        return RowType::Weight;
    }
    else if (row[0] == L'z')
    {
        return RowType::CostTo;
    }

    return RowType::Unknown;
}


void ShopHandler::AddSubCategory(const std::wstring file_name)
{
    ShopSubCategory shopsubcategory;

    shopsubcategory.Description = Utility::ExtractFileNameNoExt(file_name);

    std::wstring lc(shopsubcategory.Description);
    std::transform(lc.begin(), lc.end(), lc.begin(), ::tolower);

    shopsubcategory.DescriptionLC = lc;

    ShopCategories.back().SubCategories.push_back(shopsubcategory);
}