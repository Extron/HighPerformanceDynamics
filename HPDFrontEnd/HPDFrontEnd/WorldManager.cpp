#include "WorldManager.h"

WorldManager::WorldManager(char* worldFile, char* outputFile, btScalar tickInterval, int iterations)
{
	this->worldFile = worldFile;
	this->outputFile = outputFile;
	this->tickInterval = tickInterval;
    this->iterations = iterations;
    this->currentIteration = 0;

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
		char byteArray[2] = { 0x0, 0x6};

		output.write(byteArray, 2);

        for (int i = 0; i < sizeof(iterations); i++)
        {
            char byte = iterations >> i * 8 & 0xFF;
            output.write(&byte, 1);
        }
    }
    else
    {
        cout << "ERROR: Could not create output file " << outputFile << endl;
        return;
    }

    btCollisionObjectArray objects = world->getCollisionObjectArray();

    for (unsigned int i = 0; i < objects.size(); i++)
    {
        btRigidBody* object = dynamic_cast<btRigidBody*>(objects[i]);
            
        if (object)
            frameManager->AddBody(object, i);
    }
    
    vector<char>* buffer = new vector<char>();

    frameManager->WriteInitialState(buffer);
    
    output.write(&(*buffer)[0], buffer->size());
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
	vector<char>* frame = new vector<char>();
    	
	frameManager->WriteFrame(frameCount, frame);

	frameCount++;

	ofstream output;

    output.open(outputFile, ios::out | ios::app | ios::binary);

	//TODO: This will likely need to be parallized, and we might want to use HDF to do this.
	if (output.is_open())
	{
		//output.seekp(2, ios_base::beg);
        
        //for (int i = 0; i < sizeof(frameCount); i++)
        //{
            //char byte = frameCount >> i * 8 & 0xFF;
            //output.write(&byte, 1);
        //}
        
		output.seekp(0, ios_base::end);
		output.write(&(*frame)[0], frame->size());
		output.close();
	}
}
