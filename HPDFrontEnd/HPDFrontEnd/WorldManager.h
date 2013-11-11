#include "btBulletDynamicsCommon.h"
#include "btBulletWorldImporter.h"

#include <regex>
#include <fstream>

/**
 * Manages the world that is running in the simulation.  This includes reading in the initial world state, applying appropriate forces and spawning 
 * objects, and maintaining the output file for the world.
 */
class WorldManager
{
public:
	WorldManager(char* worldfile);


	/**
	 * Reads in the initial world state, and establishes the Bullet world to use in the simulation.
	 *
	 * @return - Returns the new Bullet world simulator.
	 */
	btDiscreteDynamicsWorld* InitializeWorld();

private:
	/**
	 * The file to load the initial world state from.
	 */
	char* worldfile;
};

