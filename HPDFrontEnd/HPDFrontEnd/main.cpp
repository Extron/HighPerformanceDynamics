#include <iostream>
#include <string>
#include <cstdlib>
#include <mpi.h>

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

    MPI_Init(&argc, &argv);
	WorldManager* wm = new WorldManager(worldFile, outputFile, tick, iterations);
    
    int size, rank;

    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    
    if (rank == 0)
	    cout << "Initializing world from world file " << worldFile << endl;

	wm->InitializeWorld();

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
    {
	    cout << "World initialized." << endl;
        cout << "Beginning simulation.  Running " << iterations << " iterations, time step " << tick << " seconds." << endl;
    }

    while (!wm->IsComplete())
        wm->Tick();

    MPI_Barrier(MPI_COMM_WORLD);

    if (rank == 0)
        cout << "Simulation complete.  Rendered simulation stored in " << outputFile << endl;

    MPI_Finalize();

	return 0;
}
