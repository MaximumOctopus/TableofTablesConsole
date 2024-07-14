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
#include "Utility.h"
#include <Windows.h>

#include "Command.h"
#include "NPCHandler.h"


NPCHandler* GNPCHandler;


NPCHandler::NPCHandler(const std::wstring folder)
{
    RootFolder = folder;
    FilePrefix = L"npc";

    if (GenerateFolderList(folder + L"NPC\\"))
    {
        for (int f = 0; f < FoundFolders.size(); f++)
        {
            PopulateFromFile(folder + L"NPC\\" + FoundFolders[f] + L"\\");
        }
    }
}


bool NPCHandler::ProcessCommand(Command c)
{
    if (c.primary == PrimaryCommand::NPC)
    {
        Last = c;

        if (c.secondary == L"help" || c.secondary == L"?")
        {
            Help();

            return true;
        }

        if (HandleOtherTasks(c))
        {
            return true;
        }
        
        std::wcout << NPCs[0].GetDescriptionText(Paramaters::All, 0) << "\n";
    }

    return true;
}


bool NPCHandler::Search(const std::wstring input, bool special)
{
    return false;
}


bool NPCHandler::SearchShow()
{
    return false;
}


void NPCHandler::Help()
{
    std::wcout << L"  npc help\n";
    std::wcout << L"  npc list\n";
    std::wcout << L"  npc stats\n\n";
    std::wcout << L"  npc\n";
    std::wcout << "\n";
}


void NPCHandler::List(Command c)
{
    for (int t = 0; t < NPCs.size(); t++)
    {
        std::wcout << L" " << t << L"  " << NPCs[t].Description << "\n";
    }
}


void NPCHandler::Show(Command c)
{

}


void NPCHandler::Stats(Command)
{
    std::wcout << L"  NPCs : " << NPCs.size() << "\n";
    std::wcout << "\n";
}


bool NPCHandler::PopulateFromFile(const std::wstring folder)
{
    NPC npc;

    npc.Description = L"default";

    AddComponent(npc.Attributes[kNPCGender], folder + L"gender.txt");
    AddComponent(npc.Attributes[kNPCRace], folder + L"race.txt");
    AddComponent(npc.Attributes[kNPCLA], folder + L"lawfulalignment.txt");
    AddComponent(npc.Attributes[kNPCMA], folder + L"moralalignment.txt");
    AddComponent(npc.Attributes[kNPCMark], folder + L"mark.txt");
    AddComponent(npc.Attributes[kNPCTalent], folder + L"talent.txt");
    AddComponent(npc.Attributes[kNPCMannerism], folder + L"mannerism.txt");
    AddComponent(npc.Attributes[kNPCTrait], folder + L"trait.txt");
    AddComponent(npc.Attributes[kNPCBond], folder + L"bond.txt");
    AddComponent(npc.Attributes[kNPCFlaw], folder + L"flaw.txt");
    AddComponent(npc.Attributes[kNPCEmotion], folder + L"emotion.txt");
    AddComponent(npc.Attributes[kNPCSocialClass], folder + L"socialclass.txt");

    AddSubItems(npc, folder + L"lowability.txt");
    AddSubItems(npc, folder + L"highability.txt");
    AddSubItems(npc, folder + L"ability_descriptions.txt");
    AddSubItems(npc, folder + L"ideal.txt");
    AddSubItems(npc, folder + L"occupation.txt");
    AddSubItems(npc, folder + L"alignment_traits.txt");

    AddComponent(npc.Attributes[kNPCEyeColour], folder + L"eyecolour.txt");
    AddComponent(npc.Attributes[kNPCEyeShape], folder + L"eyeshape.txt");
    AddComponent(npc.Attributes[kNPCHairColour], folder + L"haircolour.txt");
    AddComponent(npc.Attributes[kNPCAgeGroup], folder + L"agegroup.txt");
    AddComponent(npc.Attributes[kNPCPhysique], folder + L"physique.txt");
    AddComponent(npc.Attributes[kNPCHeight], folder + L"height.txt");

    AddComponent(npc.Attributes[kNPCParents], folder + L"parents.txt");
    AddComponent(npc.Attributes[kNPCSiblings], folder + L"siblings.txt");
    AddComponent(npc.Attributes[kNPCCousins], folder + L"cousins.txt");
    AddComponent(npc.Attributes[kNPCAunts], folder + L"aunts.txt");
    AddComponent(npc.Attributes[kNPCUncles], folder + L"uncles.txt");
    AddComponent(npc.Attributes[kNPCChildren], folder + L"children.txt");
    AddComponent(npc.Attributes[kNPCFamily], folder + L"family.txt");
    AddComponent(npc.Attributes[kNPCMaritalStatus], folder + L"maritalstatus.txt");

    NPCs.push_back(npc);

    return true;
}


bool NPCHandler::AddComponent(std::vector<std::wstring>& list, const std::wstring file_name)
{
    std::wifstream file(file_name);

    file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    if (file)
    {
        ProcessingMode mode = ProcessingMode::ReadyForSection;

        std::wstring s;

        int line = 1;

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
                        mode = ProcessingMode::ReadyForSection;
                        break;

                    case RowType::Data:

                        if (mode == ProcessingMode::SectionFound)
                        {
                            list.push_back(s.substr(2));
                        }
                        else
                        {
                            std::wcout << L"\"i:\" identifier found in invalid location. " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
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
        std::wcout << L"  NPC Component file not found \"" << file_name << "\".\n";
    }

    return false;
}


bool NPCHandler::AddSubItems(NPC& npc, const std::wstring file_name)
{
    std::wifstream file(file_name);

    file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

    if (file)
    {
        ProcessingMode mode = ProcessingMode::ReadyForSection;

        std::wstring s;

        int line = 1;

        std::vector<std::wstring> items;
        std::wstring name = L"";

        while (std::getline(file, s))
        {
            if (s != L"")
            {
                RowType current = GetRowType(s);

                switch (current)
                {
                case RowType::SectionOpen:
                {
                    name = L"";
                    items.clear();

                    break;
                }
                case RowType::SectionClose:
                {
                    NPCSub sub;

                    sub.Name = name;

                    for (int t = 0; t < items.size(); t++)
                    {
                        sub.Items.push_back(items[t]);
                    }

                    npc.SubItems.push_back(sub);

                    break;
                }
                case RowType::Data:
                    items.push_back(s.substr(2));

                    break;
                case RowType::Description:
                    name = s.substr(2);

                    break;
                }
            }
        }

        return true;
    }
    else
    {
        std::wcout << L"  NPC SubItem file not found \"" << file_name << "\".\n";
    }

    return false;
}


NPCHandler::RowType NPCHandler::GetRowType(const std::wstring row)
{
    if (row[0] == L'i')
    {
        return RowType::Data;
    }
    else if (row[0] == L'd')
    {
        return RowType::Description;
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