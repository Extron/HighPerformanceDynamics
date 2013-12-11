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

void ReadIntCSV(char* string, int* n, int count)
{
    char value[64];

    int j = 0;

    for (int i = 0; i < count; i++)
    {
        while (string[j] != ',')
        {
            value[j] = string[j];
            j++;
        }

        value[j++] = '\0';
        
        n[i] = atoi(value);
    }
}

void ReadDoubleCSV(char* string, double* n, int count)
{
    char value[64];

    int j = 0;

    for (int i = 0; i < count; i++)
    {
        while (string[j] != ',' && string[j] != '\0')
        {
            value[j] = string[j];
            j++;
        }

        value[j++] = '\0';

        n[i] = atof(value);
    }
}

int wrap_printf(void) {return 0;}

/**
 * The main entry point for High Performance Dynamics frontend, which manages parameters and launches HPD with the apprpriate settings.
 */
int main(int argc, char** argv)
{
	char* worldFile = FindArg(argc, argv, 'f');
	char* outputFile = FindArg(argc, argv, 'o');
	char* tickStr = FindArg(argc, argv, 't');
    char* iterStr = FindArg(argc, argv, 'n');
    char* boundsStr = FindArg(argc, argv, 'b');
    char* sizeStr = FindArg(argc, argv, 's');

    double tick = 0.0166667;
    int iterations = 100;
    int worldBounds[3] = { 3, 3, 1 };
    double processBounds[3] = { 10, 10, 30 };

    if (tickStr)
        tick = atof(tickStr);

    if (iterStr)
        iterations = atoi(iterStr);

    if (boundsStr)
        ReadIntCSV(boundsStr, worldBounds, 3);

    if (sizeStr)
        ReadDoubleCSV(sizeStr, processBounds, 3);

    MPI_Init(&argc, &argv);
    
    int size, rank;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
   
    WorldManager* wm = new WorldManager(worldFile, outputFile, tick, iterations);
    
    if (rank == 0)
	    cout << "Initializing world from world file " << worldFile << endl;

	wm->InitializeWorld(rank, size, worldBounds, processBounds);

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
