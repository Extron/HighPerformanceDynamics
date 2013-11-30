#include <fstream>
#include <iostream>
#include <vector>

#include "btBulletDynamicsCommon.h"

#include "WorldImporter.h"
#include "HPDMotionState.h"
#include "FrameManager.h"

using namespace std;

/**
 * Manages the world that is running in the simulation.  This includes reading in the initial world state, applying appropriate forces and spawning 
 * objects, and maintaining the output file for the world.
 */
class WorldManager
{
public:
	WorldManager(char* worldfile, char* outputFile, btScalar tickInterval = 0.016666667, int iterations = 100);


	/**
	 * Reads in the initial world state, and establishes the Bullet world to use in the simulation.
	 *
	 * @return - Returns the new Bullet world simulator.
	 */
	void InitializeWorld();

	/**
	 * Ticks the simulation one time step.
	 */
	void Tick();

    /**
     * Determines if the simulation is complete.
     */
    bool IsComplete();

	/** 
	 * Writes a frame to the output file.
	 */
	void WriteFrame();

private:
	/**
	 * The Bullet world to use to simulate.
	 */
	btDiscreteDynamicsWorld* world;

	/**
	 * The frame manager, which stores data used to write frames out to the output file.
	 */
	FrameManager* frameManager;

	/**
	 * The number of seconds each tick takes up.
	 */
	btScalar tickInterval;

	/**
	 * The file to load the initial world state from.
	 */
	char* worldFile;

	/**
	 * The file to output the frames to.
	 */
	char* outputFile;

    /**
     * The number of iterations that the simulation will run.
     */
    int iterations;

    /**
     * The current iteration that the simulation is running on.
     */
    int currentIteration;

	/**
	 * Stores the current number of frames that have been generated.
	 */
	unsigned int frameCount;
};

