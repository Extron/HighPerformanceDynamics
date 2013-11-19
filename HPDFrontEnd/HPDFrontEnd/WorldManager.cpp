#include "WorldManager.h"

WorldManager::WorldManager(char* worldFile, char* outputFile, btScalar tickInterval)
{
	this->worldFile = worldFile;
	this->outputFile = outputFile;
	this->tickInterval = tickInterval;

	frameManager = new FrameManager();
	frameCount = 0;
}

void WorldManager::InitializeWorld()
{
	//TODO: All of these parts of the physics sim can be overridden and we can implement our own.  We will likely need to do that
	//for MPI, at least for a few of them.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

	WorldImporter* fileLoader = new WorldImporter(world);

	if (!fileLoader->loadFile(worldFile))
	{
		cout << "ERROR: Could not load world file " << worldFile << endl;
		return;
	}

	ofstream output(outputFile);

	if (output.is_open())
	{
		char byteArray[6] = { 0x0, 0x6, 0x0, 0x0, 0x0, 0x0 };

		output.write(byteArray, 6);
		output.close();
	}
	else
	{
		cout << "ERROR: Could not create output file " << outputFile << endl;
		return;
	}

	btCollisionObjectArray objects = world->getCollisionObjectArray();

	//TODO: This can be parallelized as well to speed up initialization.
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		btRigidBody* object = dynamic_cast<btRigidBody*>(objects[i]);

		if (object)
			frameManager->AddBody(object, i);
	}
}

void WorldManager::WriteFrame()
{
	vector<char>* frame = new vector<char>();
	
	frameManager->WriteFrame(frameCount, frame);

	frameCount++;

	ofstream output(outputFile);

	//TODO: This will likely need to be parallized, and we might want to use HDF to do this.
	if (output.is_open())
	{
		output.seekp(2, ios_base::beg);
		char* byteArray = (char*)frameCount;
		output.write(byteArray, 4);

		output.seekp(0, ios_base::end);
		output.write((*frame)[0], frame->size());
		output.close();
	}
}