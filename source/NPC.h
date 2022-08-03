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
#include <vector>

#include "NPCSub.h"


static const int kNPCGender = 0;
static const int kNPCRace = 1;
static const int kNPCLA = 2;
static const int kNPCMA = 3;
static const int kNPCTalent = 4;
static const int kNPCMannerism = 5;
static const int kNPCTrait = 6;
static const int kNPCBond = 7;
static const int kNPCFlaw = 8;
static const int kNPCEmotion = 9;
static const int kNPCSocialClass = 10;
static const int kNPCMark = 11;
static const int kNPCHighAbility = 12;
static const int kNPCLowAbility = 13;
static const int kNPCIdeal = 14;
static const int kNPCOccupation = 15;
static const int kNPCEyeColour = 16;
static const int kNPCEyeShape = 17;
static const int kNPCHairColour = 18;
static const int kNPCAgeGroup = 19;
static const int kNPCPhysique = 20;
static const int kNPCHeight = 21;
static const int kNPCParents = 22;
static const int kNPCSiblings = 23;
static const int kNPCCousins = 24;
static const int kNPCAunts = 25;
static const int kNPCUncles = 26;
static const int kNPCChildren = 27;
static const int kNPCFamily = 28;
static const int kNPCMaritalStatus = 29;


enum class Paramaters { All = 0, Physical = 1 };


class NPC
{
    void BuildRandom();

    std::wstring GetAttribute(int attribute);

    int FindNamedSubItem(const std::wstring);
    std::wstring FromNamedSubItemSpecial(int, const std::wstring, const std::wstring, const std::wstring, const std::wstring);

    int LastAttributeIndex[30] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0 ,
            0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

    std::wstring LastContent[30] = { L"" };

public:

    std::vector<std::wstring> Attributes[30];

    std::vector<NPCSub> SubItems;

    std::wstring Description = L"";

    bool Reroll(Paramaters);

    std::wstring GetDescriptionText(Paramaters, int);

    std::wstring GetNPCElement(int);

    std::wstring FromNamedSubItem(const std::wstring);
};