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

#include "Command.h"
#include "Name.h"
#include "NamesHandler.h"
#include "Utility.h"


NamesHandler* GNamesHandler;


NamesHandler::NamesHandler(const std::wstring install_folder)
{
    RootFolder = install_folder;
    FilePrefix = L"names";

    if (GenerateFileList(install_folder + L"Names\\"))
    {
        for (int f = 0; f < FoundFiles.size(); f++)
        {
            PopulateFromFile(install_folder + L"Names\\" + FoundFiles[f]);
        }
    }
}


bool NamesHandler::ProcessCommand(Command c)
{
    if (c.primary == PrimaryCommand::Name)
    {
        std::wstring category = L"";
        std::wstring templat = L"";

        if (!HandleOtherTasks(c))
        {
            if (c.secondary == L"help" || c.secondary == L"?")
            {
                Help();
            } 
            else if (c.secondary == L"")
            {
                category = Last.secondary;
            }
            else
            {
                category = c.secondary;
            }

            if (c.tertiary == L"")
            {
                templat = Last.tertiary;
            }
            else
            {
                templat = c.tertiary;
            }

            std::wcout << GetName(category, templat) << "\n";

            Last = c;
        }

         return true;
    }

    return false;
}


bool NamesHandler::Search(std::wstring input, bool special)
{
    return false;
}


bool NamesHandler::SearchShow()
{
    return false;
}


void NamesHandler::Help()
{
    std::wcout << L"  name help\n";
    std::wcout << L"  name list\n";
    std::wcout << L"  name stats\n\n";
    std::wcout << L"  name category\n";
    std::wcout << "\n";
}


void NamesHandler::List(Command c)
{
    for (int t = 0; t < Names.size(); t++)
    {
        std::wcout << t << " \"" << Names[t].Description << "\"\n";
    }
}


void NamesHandler::Show(Command c)
{

}


void NamesHandler::Stats(Command)
{
    std::wcout << "  Types      : " << Names.size() << "\n";
    std::wcout << "  Titles     : " << NameStats.Titles << "\n";
    std::wcout << "  Names      : " << NameStats.Names << "\n";
    std::wcout << "  Adjectives : " << NameStats.Adjectives << "\n";
    std::wcout << "  Nouns      : " << NameStats.Nouns << "\n";
    std::wcout << "  Other      : " << NameStats.Other << "\n";
    std::wcout << "  Templates  : " << NameStats.Templates << "\n";
    std::wcout << "\n";
}


std::wstring NamesHandler::GetName(const std::wstring description, const std::wstring name_template)
{
    int index = GetNameIndex(description);

    std::wstring name = L"";

    if (index != -1)
    {
        if (name_template == L"")
        {
            int rt = rand() % Names[index].Templates.size();

            name = Names[index].Templates[rt];
        }            
        else
        {
            name = name_template;
        }            
    }
    else
    {
        return L"  Unknown names category \"" + description + L"\" \n";
    }

    // =======================================================================

    if (Names[index].Titles.size() != 0)
    {
        while (name.find(L"$title") != std::wstring::npos)
        {
            int rt = rand() % Names[index].Titles.size();

            Utility::Replace(name, L"$title", Names[index].Titles[rt]);
        }
    }
    else
    {
        Utility::Replace(name, L"$title", L"");
    }

    // =======================================================================

    if (Names[index].Names.size() != 0)
    {
        while (name.find(L"$name") != std::wstring::npos)
        {
            int rt = rand() % Names[index].Names.size();

            Utility::Replace(name, L"$name", Names[index].Names[rt]);
        }
    }
    else
    {
        Utility::Replace(name, L"$name", L"");
    }

    // =======================================================================

    if (Names[index].Adjectives.size() != 0)
    {
        while (name.find(L"$adjective") != std::wstring::npos)
        {
            int rt = rand() % Names[index].Adjectives.size();

            Utility::Replace(name, L"$adjective", Names[index].Adjectives[rt]);
        }
    }
    else
    {
        Utility::Replace(name, L"$adjective", L"");
    }

    // =======================================================================

    if (Names[index].Nouns.size() != 0)
    {
        while (name.find(L"$noun") != std::wstring::npos)
        {
            int rt = rand() % Names[index].Nouns.size();

            Utility::Replace(name, L"$noun", Names[index].Nouns[rt]);
        }
    }
    else
    {
        Utility::Replace(name, L"$noun", L"");
    }

    // =======================================================================

    if (Names[index].Other.size() != 0)
    {
        while (name.find(L"$other") != std::wstring::npos)
        {
            int rt = rand() % Names[index].Other.size();

            Utility::Replace(name, L"$other", Names[index].Other[rt]);
        }
    }
    else
    {
        Utility::Replace(name, L"$other", L"");
    }

    // =======================================================================

    if (name.find(L"$") != std::string::npos)
    {
        std::wcout << L"Unknown entity found in \"" << name_template << "\"\n";
    }

    return name;
}


int NamesHandler::GetNameIndex(const std::wstring description)
{
    std::wstring category(description);

    std::transform(category.begin(), category.end(), category.begin(), ::tolower);

    for (int t = 0; t < Names.size(); t++)
    {
        if (Names[t].Description[0] == description[0])
        {
            return t;
        }
    }

    return -1;
}



void NamesHandler::Add(Name name)
{
    Names.push_back(name);
}


bool NamesHandler::PopulateFromFile(const std::wstring file_name)
{
    std::wifstream file(file_name);

    file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    if (file)
    {
        ProcessingMode mode = ProcessingMode::ReadyForSection;

        Category category = Category::Unknown;

        std::wstring s;

        int line = 1;

        Name name;

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
                    case RowType::Unknown:
                        break;
                    case RowType::SectionOpen:

                        name.Description = Utility::ExtractFileNameNoExt(file_name);
                                            
                        mode = ProcessingMode::SectionFound;

                        break;
                    case RowType::SectionClose:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            Add(name);

                            NameStats.Titles += name.Titles.size();
                            NameStats.Names += name.Names.size();
                            NameStats.Adjectives += name.Adjectives.size();
                            NameStats.Nouns += name.Nouns.size();
                            NameStats.Other += name.Other.size();
                            NameStats.Templates += name.Templates.size();

                            name.Clear();
                        }
                        else
                        {
                            std::wcout << L"Unpaired { } in " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        mode = ProcessingMode::ReadyForSection;

                        break;
                    }
                    case RowType::Data:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            std::wstring parameter = s.substr(2);

                            switch (category)
                            {
                            case Category::Titles:
                                name.Titles.push_back(parameter);
                                break;
                            case Category::Names:
                                name.Names.push_back(parameter);
                                break;
                            case Category::Adjectives:
                                name.Adjectives.push_back(parameter);
                                break;
                            case Category::Nouns:
                                name.Nouns.push_back(parameter);
                                break;
                            case Category::Other:
                                name.Other.push_back(parameter);
                                break;
                            case Category::Templates:
                                name.Templates.push_back(parameter);
                                break;
                            }
                        }
                        else
                        {
                            std::wcout << L"\"i:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                        }

                        break;
                    }
                    case RowType::CategoryChange:
                    {
                        if (mode == ProcessingMode::SectionFound)
                        {
                            std::wstring parameter = s.substr(2);

                            if (parameter == L"TITLES")
                            {
                                category = Category::Titles;
                            }
                            else if (parameter == L"NAMES")
                            {
                                category = Category::Names;
                            }
                            else if (parameter == L"ADJECTIVES")
                            {
                                category = Category::Adjectives;
                            }
                            else if (parameter == L"NOUNS")
                            {
                                category = Category::Nouns;
                            }
                            else if (parameter == L"OTHER")
                            {
                                category = Category::Other;
                            }
                            else if (parameter == L"TEMPLATES")
                            {
                                category = Category::Templates;
                            }
                            else
                            {
                                category = Category::Unknown;

                                std::wcout << L"Unknown category identifier \"" << parameter << L"\" in \\names\\" << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
                                std::wcout << L"    Valid identifiers: TITLES, NAMES, ADJECTIVES, NOUNS, OTHER, TEMPLATES\n";
                            }
                        }
                        else
                        {
                            std::wcout << L"\"d:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
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
        std::wcerr << "  Couldn't access Names file \"" << file_name << "\".\n";

        return false;
    }

    return true;
}


NamesHandler::RowType NamesHandler::GetRowType(const std::wstring row)
{
    if (row[0] == L'i')
    {
        return RowType::Data;
    }
    else if (row[0] == L'd')
    { 
        return RowType::CategoryChange;
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