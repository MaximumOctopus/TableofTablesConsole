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

#include <time.h>

#include "CommandHandler.h"
#include "DiceHandler.h"
#include "GlobalObjects.h"
#include "HoardHandler.h"
#include "NamesHandler.h"
#include "NPCHandler.h"
#include "TableHandler.h"
#include "Settings.h"
#include "ShopHandler.h"


extern CommandHandler* GCommandHandler; 
extern DiceHandler* GDiceHandler;
extern HoardHandler* GHoardHandler;
extern NamesHandler* GNamesHandler;
extern NPCHandler* GNPCHandler;
extern Settings* GSettings;
extern ShopHandler* GShopHandler;
extern TableHandler* GTableHandler;


namespace GlobalObjects
{
	void CreateGlobalObjects(int argc, wchar_t* argv[])
	{
		srand((unsigned)time(NULL));

		GSettings = new Settings(argv[0]);

		GDiceHandler = new DiceHandler();

		GTableHandler = new TableHandler(GSettings->DataPath);
		GShopHandler = new ShopHandler(GSettings->DataPath);

		GNPCHandler = new NPCHandler(GSettings->DataPath);
		GNamesHandler = new NamesHandler(GSettings->DataPath);
		GHoardHandler = new HoardHandler(GSettings->DataPath);

		GCommandHandler = new CommandHandler();
	}

	void FreeGlobalObjects()
	{
		delete GSettings;

		delete GNamesHandler;

		delete GHoardHandler;

		delete GCommandHandler;

		delete GNPCHandler;
	}
}