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
#include <string>
#include <vector>
#include <Windows.h>

#include "BaseHandler.h"
#include "Utility.h"


BaseHandler::BaseHandler()
{
    if (LoadSearchResults(RootFolder + L"saves\\" + FilePrefix + L"_search.txt"))
    {
        std::wcout << L"Loaded search results.\n";
    }
    else
    {
        std::wcout << L"Failed to load search results.\n";
    }

    if (LoadFavourites(RootFolder + L"saves\\" + FilePrefix + L"_favs.txt"))
    {
        std::wcout << L"Loaded favourites.\n";
    }
    else
    {
        std::wcout << L"Failed to load favourites.\n";
    }
}


BaseHandler::~BaseHandler()
{
    if (SaveSearchResults(RootFolder + L"saves\\" + FilePrefix + L"_search.txt"))
    {
        std::wcout << L"Saved search results.\n";
    }
    else
    {
        std::wcout << L"Failed to save search results.\n";
    }

    if (SaveFavourites(RootFolder + L"saves\\" + FilePrefix + L"_favs.txt"))
    {
        std::wcout << L"Saved favourites results.\n";
    }
    else
    {
        std::wcout << L"Failed to save favourites.\n";
    }
}


bool BaseHandler::GenerateFileList(const std::wstring folder)
{
    std::wstring filter = folder + L"*.txt";

    FoundFiles.clear();

    WIN32_FIND_DATAW file;

    HANDLE search_handle = FindFirstFileW(filter.c_str(), &file);

    if (search_handle != INVALID_HANDLE_VALUE)
    {
        do
        {
            if (!(file.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY))
            {
                if ((!lstrcmpW(file.cFileName, L".")) || (!lstrcmpW(file.cFileName, L"..")))
                    continue;

                std::wstring names_file(file.cFileName);

                FoundFiles.push_back(names_file);
            }

        } while (FindNextFileW(search_handle, &file));

        FindClose(search_handle);

        return true;
    }

    return false;
}


bool BaseHandler::GenerateFolderList(const std::wstring folder)
{
    std::wstring filter = folder + L"*.*";

    FoundFolders.clear();

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

                FoundFolders.push_back(names_file);
            }

        } while (FindNextFileW(search_handle, &file));

        FindClose(search_handle);

        return true;
    }

    return false;
}


bool BaseHandler::ProcessCommand(Command c)
{
    return false;
}


bool BaseHandler::HandleOtherTasks(Command c)
{
    if (c.secondary == L"list")
    {
        List(c);

        return true;
    }
    else if (c.secondary == L"stats")
    {
        Stats(c);

        return true;
    }
    else if (c.secondary == L"help")
    {
        Help();

        return true;
    }
    else if (c.secondary.find(L"fav") == 0)
    {
        ShowFavourites();

        return true;
    }
    else if (c.secondary == L"add")
    {
        Favourite f;

        f.Description = Utility::DateTime(0);
        f.c = Last;

        Favourites.push_back(f);
    }

    return false;
}


bool BaseHandler::ShowFavourites()
{
    if (Favourites.size() != 0)
    {
        for (int f = 0; f < Favourites.size(); f++)
        {
            std::wcout << L"  " << f << L"  " << Favourites[f].c.GetFullCommand();
        }

        std::wcout << "\n";
    }
    else
    {
        std::wcout << L"No favourites added.\n\n";
    }

    return true;
}


bool BaseHandler::LoadSearchResults(const std::wstring file_name)
{
    std::wifstream file(file_name);

    file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    if (file)
    {
        std::wstring s;

        int mode = 0;

        std::wstring oc = L"";
        std::wstring description = L"";
        int level1 = 0;
        int level2 = 0;
        int level3 = 0;

        while (std::getline(file, s))
        {
            if (s != L"")
            {
                switch (s[0])
                {
                case L'[':
                    if (mode == 0)
                    {

                    }
                    break;
                case L']':
                    if (mode == 1)
                    {
                        SearchResults.OriginalCommand = oc;
                    }
                    break;
                case L'{':
                    if (mode == 0)
                    {
                        description = L"";
                        level1 = 0;
                        level2 = 0;
                        level3 = 0;
                    }
                    break;
                case L'}':
                    if (mode == 2)
                    {
                        SearchItem si;

                        si.Description = description;
                        si.Level1 = level1;
                        si.Level2 = level2;
                        si.Level3 = level3;

                        SearchResults.SearchItems.push_back(si);
                    }
                    break;
                case L'a':
                    if (mode == 2)
                    {
                        level1 = stoi(s.substr(2));
                    }
                    break;
                case L'b':
                    if (mode == 2)
                    {
                        level2 = stoi(s.substr(2));
                    }
                    break;
                case L'c':
                    if (mode == 2)
                    {
                        level3 = stoi(s.substr(2));
                    }
                    break;
                case L'd':
                    if (mode == 1)
                    {
                        description = s.substr(2);
                    }
                    break;
                case L'x':
                    if (mode == 2)
                    {
                        oc = s.substr(2);
                    }
                    break;
                }
            }
        }

        file.close();

        return true;
    }

    return false;
}


bool BaseHandler::SaveSearchResults(const std::wstring file_name)
{
    if (SearchResults.SearchItems.size() != 0)
    {
        std::wofstream file(file_name);

        file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

        if (file)
        {
            file << L"[\n";
            file << L"d:" << SearchResults.OriginalCommand << "\n";
            file << L"]\n";

            for (int t = 0; t < SearchResults.SearchItems.size(); t++)
            {
                file << L"{\n";
                file << L"a:" << SearchResults.SearchItems[t].Level1 << "\n";
                file << L"b:" << SearchResults.SearchItems[t].Level2 << "\n";
                file << L"c:" << SearchResults.SearchItems[t].Level3 << "\n";
                file << L"x:" << SearchResults.SearchItems[t].Description << "\n";
                file << L"}\n";
            }

            file.close();
        }
        else
        {
            std::wcout << L" Couldn't save file \"" << file_name << "\".\n";

            return false;
        }
    }

    return true;
}


bool BaseHandler::LoadFavourites(const std::wstring)
{
    return false;
}


bool BaseHandler::SaveFavourites(const std::wstring)
{
    if (Favourites.size() != 0)
    {

    }

    return false;
}