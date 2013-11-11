#include <iostream>
#include <string>
#include "WorldManager.h"

using namespace std;

string FindArg(int argc, char** argv, string flag)
{
	for (int i = 0; i < argc; i++)
	{
		string arg(argv[i]);

		if (arg == flag && i < argc - 1)
		{
			string val(arg[i + 1]);

			return val;
		}
	}

	return "";
}

/**
 * The main entry point for High Performance Dynamics frontend, which manages parameters and launches HPD with the apprpriate settings.
 */
int main(int argc, char** argv)
{
	string worldfile = FindArg(argc, argv, "-f");

	WorldManager* wm = new WorldManager(worldfile);

	cout << "Initializing world from world file " << worldFile << "..." << endl;
	wm.InitializeWorld();
	cout << "World initialized" << endl;

	return 0;
}