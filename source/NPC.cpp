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

#include "NPC.h"


std::wstring NPC::GetDescriptionText(Paramaters parameters, int variant)
{
	std::wstring output = L"";

	if (parameters == Paramaters::All)
	{
		BuildRandom();
	}
	else
	{
		Reroll(parameters);
	}

	output = L"You see a " + LastContent[kNPCPhysique] + L" " + LastContent[kNPCGender] + L" " + LastContent[kNPCRace] +
		L" with a " + LastContent[kNPCMark] + L" wearing the clothes of a " + FromNamedSubItemSpecial(kNPCOccupation, LastContent[kNPCSocialClass], L"royal", LastContent[kNPCGender], L"occupation_" + LastContent[kNPCSocialClass]) + L". This character looks to be in their " + LastContent[kNPCAgeGroup] +
		L", has " + LastContent[kNPCHairColour] + L" hair, and is " + LastContent[kNPCHeight] + L" for a " + LastContent[kNPCGender] + L" " + LastContent[kNPCRace] + L"." +
		L" You can see " + LastContent[kNPCEmotion] + L" in their " + LastContent[kNPCEyeColour] + L", " + LastContent[kNPCEyeShape] + L" eyes.\n\n" +

		L"$Name_Fantasy has a " + LastContent[kNPCLA] + L"-" + LastContent[kNPCMA] + L" alignment and is $" + FromNamedSubItem(L"ha_description_" + LastContent[kNPCHighAbility]) + L"$ yet $" +
		FromNamedSubItem(L"la_description_" + LastContent[kNPCLowAbility]) + L"$" +
		L". Impressively, this character is " + LastContent[kNPCTalent] + L". They often " + LastContent[kNPCMannerism] + L" and has a " +
		LastContent[kNPCTrait] + L" way of speaking. This character is always " + LastContent[kNPCBond] + L" and values " + FromNamedSubItem(L"alignment_" + LastContent[kNPCLA]) + L" more than anything, but is troubled by their " + LastContent[kNPCFlaw] + L". " +
		FromNamedSubItem(L"at_" + LastContent[kNPCLA] + L"_" + LastContent[kNPCMA]) + L".\n\n" +

		L"They have a " + LastContent[kNPCFamily] + L" with their family. This character is " + LastContent[kNPCMaritalStatus] + L" with " + LastContent[kNPCChildren] +
		L". The rest of their family consists of " + LastContent[kNPCSiblings] + L", " + LastContent[kNPCCousins] + L", " + LastContent[kNPCAunts] + L", " + LastContent[kNPCUncles] + L", and " + LastContent[kNPCParents] + L".";

	return output;
}


void NPC::BuildRandom()
{
	GetAttribute(kNPCPhysique);
	GetAttribute(kNPCGender);
	GetAttribute(kNPCRace);
	GetAttribute(kNPCMark);
	GetAttribute(kNPCOccupation);
	GetAttribute(kNPCAgeGroup);
	GetAttribute(kNPCHairColour);
	GetAttribute(kNPCHeight);
	GetAttribute(kNPCSocialClass);

	GetAttribute(kNPCEmotion);
	GetAttribute(kNPCEyeColour);
	GetAttribute(kNPCEyeShape);

	GetAttribute(kNPCLA);
	GetAttribute(kNPCMA);
	GetAttribute(kNPCLowAbility);
	GetAttribute(kNPCTalent);
	GetAttribute(kNPCMannerism);
	GetAttribute(kNPCTrait);
	GetAttribute(kNPCBond);
	GetAttribute(kNPCFlaw);

	GetAttribute(kNPCFamily);
	GetAttribute(kNPCMaritalStatus);
	GetAttribute(kNPCChildren);
	GetAttribute(kNPCSiblings);
	GetAttribute(kNPCAunts);
	GetAttribute(kNPCUncles);
	GetAttribute(kNPCParents);
}


bool NPC::Reroll(Paramaters parameters)
{
	switch (parameters)
	{
	case Paramaters::Physical:

		GetAttribute(kNPCPhysique);
		GetAttribute(kNPCAgeGroup);
		GetAttribute(kNPCHairColour);
		GetAttribute(kNPCHeight);
		GetAttribute(kNPCEyeColour);
		GetAttribute(kNPCEyeShape);

		break;

	default:
		return false;
	}

	return true;
}


std::wstring NPC::GetAttribute(int attribute)
{
	if (Attributes[attribute].size() != 0)
	{
		int ra = rand() % Attributes[attribute].size();

		LastAttributeIndex[attribute] = ra;
		LastContent[attribute] = Attributes[attribute][ra];

		return Attributes[attribute][ra];
	}
	else
	{
		return L"[attr " + std::to_wstring(attribute) + L" == 0]";
	}
}


std::wstring NPC::GetNPCElement(int id)
{
	return L"";
}


int NPC::FindNamedSubItem(const std::wstring name)
{
	for (int t = 0; t < SubItems.size(); t++)
	{
		if (SubItems[t].Name == name)
		{
			return t;
		}
	}

	return -1;
}


std::wstring NPC::FromNamedSubItem(const std::wstring name)
{
	std::wstring named_subitem(name);

	std::transform(named_subitem.begin(), named_subitem.end(), named_subitem.begin(), ::tolower);

	std::wcout << "fnsi " << named_subitem << "\n";

	int subitem = FindNamedSubItem(named_subitem);

	if (subitem != -1)
	{
		int ri = rand() % SubItems[subitem].Items.size();

		return SubItems[subitem].Items[ri];
	}

	return L"";
}


// hack for occupations :)
std::wstring NPC::FromNamedSubItemSpecial(int id, const std::wstring input, const std::wstring test, const std::wstring suffix, const std::wstring name)
{
	std::wstring inputlc(input);
	std::wstring testlc(input);
	std::wstring suffixlc(suffix);
	std::wstring namelc(name);
	std::wstring search = L"";

	std::transform(inputlc.begin(), inputlc.end(), inputlc.begin(), ::tolower);
	std::transform(testlc.begin(), testlc.end(), testlc.begin(), ::tolower);
	std::transform(suffixlc.begin(), suffixlc.end(), suffixlc.begin(), ::tolower);
	std::transform(namelc.begin(), namelc.end(), namelc.begin(), ::tolower);

	if (inputlc == testlc)
	{
		search = namelc + L"_" + suffixlc;
	}
	else
	{
		search = namelc;
	}

	int subitem = FindNamedSubItem(search);

	if (subitem != -1)
	{
		int ri = rand() % SubItems[subitem].Items.size();

		LastContent[id] = SubItems[subitem].Items[ri];

		return LastContent[id];
	}

	return L"";
}