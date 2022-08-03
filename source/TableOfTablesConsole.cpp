//
// Table of Tables Console 2
//
// (c) Paul Alan Freshney 2016-2022
//   paul@freshney.org
//
// Source code and executables:
//   https://github.com/MaximumOctopus/TableofTablesConsole
// 
// This application is based on, and inspired by, the contents of the popular "Behind the Tables" subreddit:
//   https://www.reddit.com/r/BehindTheTables/wiki/index
// and also contains some content from here:
//   https://github.com/nickolusroy/npc-generator
// and also some from here:
//   https://docs.google.com/spreadsheets/d/1G_PmUM9ONAND7qwiS2ZPHxsT4Le1A8ts9Ig2c18VAHc/edit#gid=796186040
//

#include <iostream>
#include <string>

#include "CommandHandler.h"
#include "DiceHandler.h"
#include "HoardHandler.h"
#include "NamesHandler.h"
#include "GlobalObjects.h"
#include "Welcome.h"


extern CommandHandler* GCommandHandler;


int wmain(int argc, wchar_t* argv[])
{
	Welcome::Header();

    GlobalObjects::CreateGlobalObjects(argc, argv);

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================

	std::wstring input;

	Command c;

	do
	{
		std::wcout << L"> ";

		std::getline(std::wcin, input);

		c = GCommandHandler->ProcessCommand(input);
	}
	while (c.primary != PrimaryCommand::Exit);

    // ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================
	
	GlobalObjects::FreeGlobalObjects();

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================

	#ifdef _DEBUG
	system("pause");
	#endif

	// ==========================================================================================
	// ==========================================================================================
	// ==========================================================================================
	
    return 0;
}