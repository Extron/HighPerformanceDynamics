#include "WorldManager.h"

WorldManager::WorldManager(char* worldFile, char* outputFile, btScalar tickInterval, int iterations)
{
	this->worldFile = worldFile;
	this->outputFile = outputFile;
	this->tickInterval = tickInterval;
    this->iterations = iterations;
    this->currentIteration = 0;
}

void WorldManager::InitializeWorld(int rank, int size, int* worldBounds, double* bounds)
{
    this->rank = rank;
    this->size = size;

    frameManager = new FrameManager(rank, size);
    frameCount = 0;

	//TODO: All of these parts of the physics sim can be overridden and we can implement our own.  We will likely need to do that
	//for MPI, at least for a few of them.
	btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
	btCollisionDispatcher* dispatcher = new	btCollisionDispatcher(collisionConfiguration);
	btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
	btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

	world = new HPDDynamicsWorld(rank, size, worldBounds, btVector3(bounds[0], bounds[1], bounds[2]), dispatcher, overlappingPairCache, solver, collisionConfiguration);

	WorldImporter* fileLoader = new WorldImporter(world);

	if (!fileLoader->loadFile(worldFile))
	{
		cout << "ERROR: Could not load world file " << worldFile << endl;
		return;
	}

	ofstream output(outputFile);

	if (output.is_open())
	{
        if (!rank)
        {
		    char byteArray[2] = { 0x6, 0x0};

		    output.write(byteArray, 2);
            output.write((char*)&iterations, sizeof(iterations));

            /*for (int i = 0; i < sizeof(iterations); i++)
            {
                char byte = iterations >> i * 8 & 0xFF;
                output.write(&byte, 1);
            }*/
        }
        else
        {
            //We have written in rank 0, but no other processes will see the write, so we must advance the 
            //current position in the file.
            output.seekp(2 + sizeof(int));
        }
    }
    else
    {
        cout << "ERROR: Could not create output file " << outputFile << endl;
        return;
    }

    btCollisionObjectArray objects = world->getCollisionObjectArray();

    //TODO: This is where we need to devide the world up into the ranks.
    for (unsigned int i = 0; i < objects.size(); i++)
    {
        btRigidBody* object = dynamic_cast<btRigidBody*>(objects[i]);
            
        if (object)
            frameManager->AddBody(object, i);
    }
    
    frameManager->WriteInitialState(&output); 
    output.close();
}

void WorldManager::Tick()
{
    //cout << "Performing iteration " << currentIteration << ", time " << tickInterval * currentIteration << " seconds." << endl;
    world->stepSimulation(tickInterval, 1, tickInterval);
    currentIteration++;
    //cout << "Beginning write to frame " << frameCount << "." << endl;
    WriteFrame();
}

bool WorldManager::IsComplete()
{
    return currentIteration >= iterations;
}

void WorldManager::WriteFrame()
{
	ofstream fout;
    fout.open(outputFile, ios::out | ios::app | ios::binary);

    if (fout.is_open())
    {
	    frameManager->WriteFrame(frameCount, &fout);
	    frameCount++;
    }

    fout.close();
}
