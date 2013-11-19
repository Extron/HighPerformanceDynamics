#include <iostream>
#include <string>
#include <cstdlib>

#include "WorldManager.h"

using namespace std;

char* FindArg(int argc, char** argv, char flag)
{
	for (int i = 0; i < argc; i++)
	{
		if (argv[i][0] == '-' && argv[i][1] == flag && i < argc - 1)
			return argv[i + 1];
	}

	return 0;
}

/**
 * The main entry point for High Performance Dynamics frontend, which manages parameters and launches HPD with the apprpriate settings.
 */
int main(int argc, char** argv)
{
	char* worldFile = FindArg(argc, argv, 'f');
	char* outputFile = FindArg(argc, argv, 'o');
	double tick = atof(FindArg(argc, argv, 't'));

	WorldManager* wm = new WorldManager(worldFile, outputFile, tick > 0.0 ? tick : 0.0166667);

	cout << "Initializing world from world file " << worldFile << "..." << endl;
	wm->InitializeWorld();
	cout << "World initialized" << endl;

	return 0;
}