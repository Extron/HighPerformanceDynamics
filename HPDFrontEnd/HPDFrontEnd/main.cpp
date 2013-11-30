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
	char* tickStr = FindArg(argc, argv, 't');
    char* iterStr = FindArg(argc, argv, 'n');

    double tick = 0.0166667;
    int iterations = 100;

    if (tickStr)
        tick = atof(tickStr);

    if (iterStr)
        iterations = atoi(iterStr);

	WorldManager* wm = new WorldManager(worldFile, outputFile, tick, iterations);

	cout << "Initializing world from world file " << worldFile << endl;
	wm->InitializeWorld();
	cout << "World initialized." << endl;

    cout << "Beginning simulation.  Running " << iterations << " iterations, time step " << tick << " seconds." << endl;
    while (!wm->IsComplete())
        wm->Tick();

    cout << "Simulation complete.  Rendered simulation stored in " << outputFile << endl;

	return 0;
}
