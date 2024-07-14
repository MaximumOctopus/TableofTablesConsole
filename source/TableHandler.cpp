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
#include <Windows.h>

#include "Output.h"
#include "TableHandler.h"
#include "ToTUtility.h"
#include "Utility.h"


TableHandler* GTableHandler;


TableHandler::TableHandler(const std::wstring folder)
{
	RootFolder = folder;
	FilePrefix = L"table";

	if (GenerateFolderList(folder + L"Tables\\"))
	{
		for (int f = 0; f < FoundFolders.size(); f++)
		{
			TableCategory tc;
			tc.Title = FoundFolders[f];// Utility::ReplaceAll(FoundFolders[f], L"__", L"//");
			
			Categories.push_back(tc);

			PopulateCategory(folder + L"Tables\\" + FoundFolders[f] + L"\\");
		}
	}
}


bool TableHandler::ProcessCommand(Command c)
{
	if (c.primary == PrimaryCommand::Table)
	{
		bool output_success = false;

		if (HandleOtherTasks(c))
		{
			Last = c;

			return false;
		}

		if (c.tertiary == L"")
		{
			c.tertiary = Last.tertiary;
		}

		if (c.secondary == L"help" || c.secondary == L"?")
		{
			Help();
		}
		else if (c.secondary == L"list")
		{
			List(c);
		}
		else if (c.secondary == L"search")
		{
			if (c.tertiary != L"show")
			{
				Search(c.tertiary, true);
			}
			else
			{
				SearchShow();
			}

			return false;
		}
		else if (c.secondary == L"searchin")
		{
			SearchIn(c.tertiary);
		}
		else if (c.secondary == L"searchq")
		{
			if (c.tertiary != L"show")
			{
				Search(c.tertiary, false);
			}
			else
			{
				SearchShow();
			}

			return false;
		}
		else if (c.secondary == L"show")
		{
			try
			{
				int tableID = stoi(c.tertiary);

				ShowTable(tableID);

				return false;
			}
			catch (...)
			{
				// to do
			}
		}
		else if (c.secondary == L"roll")
		{
			try
			{
				int tableID = stoi(c.tertiary);

				RollTable(tableID);
			}
			catch (...)
			{
				// to do
			}
		}
		else if (c.secondary == L"rollall")
		{
			if (c.tertiary != L"")
			{
				try
				{
					if (std::isalpha(c.tertiary[0]))
					{
						output_success = RollTableAllMembers(c.tertiary);
					}
					else
					{
						int tableID = stoi(c.tertiary);

						output_success = RollTableAllMembers(tableID);
					}
				}
				catch (...)
				{
					// to do
				}
			}
		}

		Last = c;

		return output_success;
	}

	return false;
}


bool TableHandler::Search(const std::wstring input, bool include_items)
{
	std::wstring search(input);

	std::transform(search.begin(), search.end(), search.begin(), ::tolower);

	SearchObject so;

	for (int t = 0; t < Tables.size(); t++)
	{
		if (include_items)
		{
			std::wstring content = L"";

			for (int i = 0; i < Tables[t].Items.size(); i++)
			{
				content = Tables[t].Items[i];

				std::transform(content.begin(), content.end(), content.begin(), ::tolower);

				if (content.find(search) != std::wstring::npos)
				{
					SearchItem si;

					si.Description = Tables[t].Name;
					si.Level1 = t;
					si.Level2 = i;
					si.Level3 = Tables[t].SubID;

					so.SearchItems.push_back(si);

					std::wcout << t << L" " << i << L" " << Tables[t].Title << L" " << content << "\n";
				}
			}
		}
		else
		{
			std::wstring content = Tables[t].Title;

			std::transform(content.begin(), content.end(), content.begin(), ::tolower);

			if (content.find(search) != std::wstring::npos)
			{
				std::wcout << t << L" " << Tables[t].CategoryID << L" " << Tables[t].SubID << L" " << Tables[t].Title << "\n";
			}
		}
	}

	if (so.SearchItems.size() != 0)
	{
		SearchResults = so;
	}

	return true;
}


bool TableHandler::SearchIn(const std::wstring input)
{
	if (SearchResults.SearchItems.size() != 0)
	{
		std::wstring search(input);

		std::transform(search.begin(), search.end(), search.begin(), ::tolower);

		std::wstring content = L"";

		for (int t = 0; t < SearchResults.SearchItems.size(); t++)
		{
			content = Tables[SearchResults.SearchItems[t].Level1].Items[SearchResults.SearchItems[t].Level2];

			std::transform(content.begin(), content.end(), content.begin(), ::tolower);

			if (content.find(search) != std::wstring::npos)
			{
				std::wcout << SearchResults.SearchItems[t].Level1 << L" " << Tables[t].CategoryID << L" " << Tables[t].SubID << L" " << Tables[t].Title << "\n";
			}
		}

		return true;
	}

	return false;
}


bool TableHandler::SearchShow()
{
	if (SearchResults.SearchItems.size() != 0)
	{
		std::wcout << L" \"" << SearchResults.OriginalCommand << L"\" \n";

		for (int t = 0; t < SearchResults.SearchItems.size(); t++)
		{
			std::wcout << L"   " << Tables[SearchResults.SearchItems[t].Level1].Items[SearchResults.SearchItems[t].Level2] << "\n";
		}

		std::wcout << "\n";

		return true;
	}

	return false;
}


bool TableHandler::ShowTable(int table)
{
	if (table < Tables.size())
	{
		std::wcout << L"  D" << Tables[table].Items.size() << L"  " << Tables[table].Title << "\n";

		for (int i = 0; i < Tables[table].Items.size(); i++)
		{
			std::wcout << L"    " << Tables[table].Items[i] << "\n";
		}

		std::wcout << "\n";

		return true;
	}

	return false;
}


// rolls a single table
bool TableHandler::RollTable(int table)
{
	if (table < Tables.size())
	{
		int ri = rand() % Tables[table].Items.size();

		std::wcout << L"  D" << Tables[table].Items.size() << L"  " << Tables[table].Title << "\n";
		std::wcout << L"    " << Tables[table].Items[ri] << "\n";

		return true;
	}

	return false;
}


// rolls all tables in the same category/subcategory
// eg the Assassin set is a selection of tables that can be used to generate a quick assassin NPC
bool TableHandler::RollTableAllMembers(int table)
{
	if (table < Tables.size())
	{
		int cat = Tables[table].CategoryID;
		int subcat = Tables[table].SubID;

		for (int t = 0; t < Tables.size(); t++)
		{
			if (Tables[t].CategoryID == cat && Tables[t].SubID == subcat)
			{
				int ri = rand() % Tables[t].Items.size();

				Output::Lines.push_back(L" " + Tables[t].Title + L" " + Tables[t].Items[ri]);
			}
		}

		return true;
	}

	return false;
}


// rolls all tables in the same category/subcategory
// eg the Assassin set is a selection of tables that can be used to generate a quick assassin NPC
bool TableHandler::RollTableAllMembers(std::wstring table_name)
{
	if (table_name != L"")
	{
		TableGroup tg = GetFromSubCategoryName(table_name);

		std::wcout << L"  " << tg.Category << L"  " << tg.SubCategory << "\n";

		if (tg.valid)
		{
			for (int t = 0; t < Tables.size(); t++)
			{
				if (Tables[t].CategoryID == tg.Category && Tables[t].SubID == tg.SubCategory)
				{
					int ri = rand() % Tables[t].Items.size();

					Output::Lines.push_back(L" " + Tables[t].Title + L" " + Tables[t].Items[ri]);
				}
			}

			return true;
		}
	}

	return false;
}


void TableHandler::List(Command c)
{
	if (c.tertiary == L"")
	{
		if (Last.tertiary.empty())
		{
			c.tertiary = L"cat";
		}
		else
		{
			c.tertiary = Last.tertiary;
		}
	}

	if (c.tertiary == L"cat")
	{
		for (int t = 0; t < Categories.size(); t++)
		{
			std::wcout << L" " << Utility::PadRight(t, 3) << L" \"" << Categories[t].Title << L"\".\n";
		}
	}
	else if (c.tertiary == L"subcat")
	{
		for (int t = 0; t < SubCategories.size(); t++)
		{
			std::wcout << L" " << Utility::PadRight(t, 3) << L" \"" << SubCategories[t].Title << L"\".\n";
		}
	}
}


void TableHandler::Show(Command c)
{
	if (c.tertiary == L"")
	{
		if (Last.tertiary.empty())
		{
			c.tertiary = L"cat";
		}
		else
		{
			c.tertiary = Last.tertiary;
		}
	}

	if (c.quaternary == L"")
	{
		return;
	}

	int meow = stoi(c.quaternary);

	if (c.tertiary == L"cat")
	{
		for (int t = 0; t < Tables.size(); t++)
		{
			if (Tables[t].CategoryID == meow)
			{
				std::wcout << L" " << Utility::PadRight(t, 3) << L" \"" << Tables[t].Title << L"\".\n";
			}
		}
	}
	else if (c.tertiary == L"subcat")
	{
		for (int t = 0; t < Tables.size(); t++)
		{
			if (Tables[t].SubID == meow)
			{
				std::wcout << L" " << Utility::PadRight(t, 3) << L" \"" << Tables[t].Title << L"\".\n";
			}
		}
	}
}


void TableHandler::Stats(Command)
{
	std::wcout << "  Tabls               : " << Tables.size() << "\n";
	std::wcout << "  Table Categories    : " << Categories.size() << "\n";
	std::wcout << "  Table Subcategories : " << SubCategories.size() << "\n";
	std::wcout << "\n";
}


bool TableHandler::PopulateCategory(const std::wstring folder)
{
	if (GenerateFileList(folder))
	{
		for (int f = 0; f < FoundFiles.size(); f++)
		{
			AddTable(folder + FoundFiles[f], f);
		}
	}

	return true;
}


bool TableHandler::AddTable(const std::wstring file_name, int sub_id)
{
	if (sub_id != -1)
	{
		TableSubCategory subcategory;

		subcategory.CategoryID = Categories.size() - 1;
		subcategory.SubCategoryID = sub_id;
		subcategory.Title = Utility::ExtractFileNameNoExt(file_name);

		SubCategories.push_back(subcategory);
	}

	std::wifstream file(file_name);

	file.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	if (file)
	{
		ProcessingMode mode = ProcessingMode::Waiting;

		std::wstring s;

		//

		std::vector<std::wstring> Items;

		std::wstring Name = L"";
		std::wstring Title = L"";
		int CategoryID = -1;
		int SubID = sub_id;
		int TableID = -1;
		int DiceRoll = -1;

		//

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
					case RowType::TopLevelOpen:

						mode = ProcessingMode::TopLevelTable;

						if (sub_id != -1)
						{
							CategoryID = Categories.size() - 1;
							SubID = sub_id;
						}
						else
						{
							CategoryID = -1;
							SubID = -1;
						}

						TableID = CurrentTableID;

						Items.clear();

						break;
					case RowType::TopLevelClose:
						if (mode == ProcessingMode::TopLevelTable)
						{
							Table table;

							table.CategoryID = CategoryID;
							table.SubID = SubID;
							table.TableID = TableID;
							table.Title = Title;
							table.DiceRoll = DiceRoll;
							table.Name = Name;

							if (DiceRoll == 0)
							{
								table.Mode = Table::Mode::All;
							}
							else if (DiceRoll > 0)
							{
								if (Items.size() == DiceRoll)
								{
									table.Mode = Table::Mode::OneToOne;
								}
								else
								{
									table.Mode = Table::Mode::Range;
								}
							}
							else
							{
								table.Mode = Table::Mode::Unknown;
							}

							for (int i = 0; i < Items.size(); i++)
							{
								table.Items.push_back(Items[i]);
							}

							CurrentTableID++;

							Tables.push_back(table);
						}
						else
						{
							std::wcout << L"Unpaired { } in " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
						}

						mode = ProcessingMode::Waiting;

						break;
					case RowType::SecondaryOpen:

						mode = ProcessingMode::SecondaryTable;

						break;
					case RowType::SecondaryClose:

						if (mode == ProcessingMode::SecondaryTable)
						{
							Table table;

							table.CategoryID = CategoryID;
							table.SubID = SubID;
							table.TableID = TableID;
							table.Title = Title;
							table.DiceRoll = DiceRoll;
							table.Name = Name;
							table.Mode = Table::Mode::Title;

							for (int i = 0; i < Items.size(); i++)
							{
								table.Items.push_back(Items[i]);
							}

							CurrentTableID++;

							Tables.push_back(table);
						}
						else
						{
							std::wcout << L"Unpaired [ ] in " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
						}

						mode = ProcessingMode::Waiting;

						break;
					case RowType::TertiaryOpen:
						mode = ProcessingMode::TertiaryTable;

						break;
					case RowType::TertiaryClose:
						mode = ProcessingMode::Waiting;

						break;
					case RowType::Dice:
						switch (mode)
						{
						case ProcessingMode::TopLevelTable:
						{
							int diceprefix = ToTUtility::GetDicePrefix(s.substr(2));
							Title = ToTUtility::RemoveDicePrefix(s.substr(2));

							if (diceprefix > 0)
							{
								DiceRoll = diceprefix;
							}
							else
							{
								DiceRoll = -1;
							}

							break;
						}
						case ProcessingMode::SecondaryTable:

							DiceRoll = -1;
							Title = ToTUtility::RemoveDicePrefix(s.substr(2));

							break;
						case ProcessingMode::TertiaryTable:
							break;

						default:
							std::wcout << L"\"d:\" identifier found in invalid location: " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
						}

						break;
					case RowType::Item:
					{
						switch (mode)
						{
						case ProcessingMode::TopLevelTable:
						{
							std::wstring content = s.substr(2);

							Items.push_back(content);

							/*lPos: = Pos('$', Copy(lFile[t], 3, length(lFile[t]) - 2)); TO DO

							if lPos > 0 then begin
								lName : = ToTUtilityContent.GetAlphaNumWordFrom(lPos + 1, Copy(lFile[t], 3, length(lFile[t]) - 2));

							if GetNameIndex(lName) = -1 then
								ShowMessage('Unknown $name reference "' + lName + '" in: ' + #13#13 + aFileName + #13#13 +
									'@ line ' + IntToStr(t + 1));
							end;*/

							break;
						}
						default:
							std::wcout << L"\"i:\" identifier found in invalid location: " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
						}

						break;
					}
					case RowType::Name:
						switch (mode)
						{
						case ProcessingMode::TopLevelTable:
							Name = s.substr(2);

							std::transform(Name.begin(), Name.end(), Name.begin(), ::tolower);

							break;
						case ProcessingMode::SecondaryTable:
							break;

						default:
							std::wcout << L"\"n:\" identifier found in invalid location: " << Utility::ExtractFileName(file_name) << L" @ line " << line << "\n";
						}

						break;
					}
				}
			}

			line++;
		}

		file.close();
	}
    else
    {
        std::wcerr << "  Couldn't access Table file \"" << file_name << "\".\n";

		return false;
    }

	return false;
}


TableGroup TableHandler::GetFromSubCategoryName(std::wstring name)
{
	TableGroup tg;

	std::wstring namelc(name);

	std::transform(namelc.begin(), namelc.end(), namelc.begin(), ::tolower);

	for (int t = 0; t < SubCategories.size(); t++)
	{
		std::wstring title(SubCategories[t].Title);

		std::transform(title.begin(), title.end(), title.begin(), ::tolower);

		if (title.find(namelc) != std::wstring::npos)
		{
			tg.Category = SubCategories[t].CategoryID;
			tg.SubCategory = SubCategories[t].SubCategoryID;
			tg.valid = true;

			return tg;
		}
	}

	return tg;
}


TableHandler::RowType TableHandler::GetRowType(const std::wstring row)
{
	switch (row[0])
	{
	case L'{':
		return RowType::TopLevelOpen;
	case L'}':
		return RowType::TopLevelClose;

	case L'[':
		return RowType::SecondaryOpen;
	case L']':
		return RowType::SecondaryClose;

	case L'(':
		return RowType::TertiaryOpen;
	case L')':
		return RowType::TertiaryClose;

	case L'd':
		return RowType::Dice;
	case L'i':
		return RowType::Item;
	case L'n':
		return RowType::Name;
	}

	return RowType::Unknown;
}


int TableHandler::FindSubCategoryItem(int cat, int subcat)
{
	for (int t = 0; t < SubCategories.size(); t++)
	{
		if (SubCategories[t].CategoryID == cat && SubCategories[t].SubCategoryID == subcat)
		{
			return t;
		}
	}

	return -1;
}


int TableHandler::FindSubCategoryFrom(const std::wstring cat, const std::wstring sub_cat)
{
	std::wstring from_category(cat);
	std::wstring from_sub_category(sub_cat);

	std::transform(from_category.begin(), from_category.end(), from_category.begin(), ::tolower);
	std::transform(from_sub_category.begin(), from_sub_category.end(), from_sub_category.begin(), ::tolower);

	int found_cat = -1;

	for (int t = 0; t < Categories.size(); t++)
	{
		std::wstring title(Categories[t].Title);

		std::transform(title.begin(), title.end(), title.begin(), ::tolower);

		if (title == from_category)
		{
			found_cat = t;
		}
	}

	if (found_cat != -1)
	{
		for (int t = 0; t < SubCategories.size(); t++)
		{
			std::wstring title(SubCategories[t].Title);

			std::transform(title.begin(), title.end(), title.begin(), ::tolower);

			if (SubCategories[t].CategoryID == found_cat && title == from_sub_category)
			{
				return t;
			}
		}
	}

	return -1;
}


void TableHandler::Help()
{
	std::wcout << L"\nAvailable \"table\" commands:\n\n";
	std::wcout << L"  table help\n";
	std::wcout << L"  table list cat\n";
	std::wcout << L"  table list subcat\n";
	std::wcout << L"  table show cat n\n";
	std::wcout << L"  table show subcat n\n";
	std::wcout << L"  table stats\n\n";
	std::wcout << L"  table roll table_id\n";
	std::wcout << L"  table rollall table_id\n";
	std::wcout << L"  table search term\n";
	std::wcout << L"  table searchq term\n";
	std::wcout << L"  table show table_id\n";
	std::wcout << "\n";
}