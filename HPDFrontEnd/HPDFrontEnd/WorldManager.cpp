#include "WorldManager.h"


WorldManager::WorldManager(char* worldfile)
{
	this->worldfile = worldfile;
}

btDiscreteDynamicsWorld* WorldManager::InitializeWorld()
{
	//TODO: All of these parts of the physics sim can be overridden and we can implement our own.  We will likely need to do that
	//for MPI, at least for a few of them.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	btDiscreteDynamicsWorld* dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	fileLoader = new btBulletWorldImporter(dynamicsWorld);

	if (!fileLoader->loadFile(worldFile))
		cout << "ERROR: Could not load world file " << worldFile << endl;

	return dynamicsWorld;
}