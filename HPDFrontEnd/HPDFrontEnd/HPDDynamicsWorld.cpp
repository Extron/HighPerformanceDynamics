#include "HPDDynamicsWorld.h"
#include <iostream>

using namespace std;

HPDDynamicsWorld::HPDDynamicsWorld(int rank, int size, int worldBounds[], btVector3 processBounds, 
                                   btDispatcher* dispatcher, btBroadphaseInterface* pairCache, 
                                   btConstraintSolver* constraintSolver, btCollisionConfiguration* collisionConfiguration)
    : btDiscreteDynamicsWorld(dispatcher, pairCache, constraintSolver, collisionConfiguration)
{
    this->rank = rank;
    this->size = size;
    this->processOrigin = processOrigin;
    this->processBounds = processBounds;

    this->worldBounds[0] = worldBounds[0];
    this->worldBounds[1] = worldBounds[1];
    this->worldBounds[2] = worldBounds[2];

    cell[0] = rank % worldBounds[0];
    cell[1] = (rank / worldBounds[0]) % worldBounds[1];
    cell[2] = rank / (worldBounds[0] * worldBounds[1]);

    processOrigin.setX((cell[0] - (worldBounds[0] - 1) / 2) * processBounds.x() * 2);
    processOrigin.setX((cell[1] - (worldBounds[1] - 1) / 2) * processBounds.y() * 2);
    processOrigin.setX((cell[2] - (worldBounds[2] - 1) / 2) * processBounds.z() * 2);
}

void HPDDynamicsWorld::internalSingleStepSimulation(btScalar timeStep)
{
    
    BT_PROFILE("internalSingleStepSimulation");

    if (0 != m_internalPreTickCallback)
        (*m_internalPreTickCallback)(this, timeStep);

    
    predictUnconstraintMotion(timeStep);

    CommunicateGhostBodies();

    btDispatcherInfo& dispatchInfo = getDispatchInfo();
    dispatchInfo.m_timeStep = timeStep;
    dispatchInfo.m_stepCount = 0;
    dispatchInfo.m_debugDraw = getDebugDrawer();


    createPredictiveContacts(timeStep);
    performDiscreteCollisionDetection();
    calculateSimulationIslands();
    
    getSolverInfo().m_timeStep = timeStep;

    solveConstraints(getSolverInfo());
    
    integrateTransforms(timeStep);
    updateActions(timeStep);
    updateActivationState( timeStep );

    RemoveGhostBodies();

    if (0 != m_internalTickCallback)
        (*m_internalTickCallback)(this, timeStep);
}


void HPDDynamicsWorld::addCollisionObject(btCollisionObject* collisionObject, short collisionFilterGroup, short collisionFilterMask)
{
    if (IsCollisionShapeInBounds(collisionObject->getCollisionShape(), collisionObject->getWorldTransform().getOrigin(), processOrigin, processBounds))
        btDiscreteDynamicsWorld::addCollisionObject(collisionObject, collisionFilterGroup, collisionFilterMask);        
}

void HPDDynamicsWorld::CommunicateGhostBodies()
{
    btBulletWorldImporter importer(this);
    
    unordered_map<int, btDefaultSerializer*> hash;
    char* destinations = new char[size]();
    
    cout << "Sending ghost points." << endl;

    //This sends the all of the correct rigid bodies to the various processes that need them.
    for (int i = 0; i < m_nonStaticRigidBodies.size(); i++)
    {
        btRigidBody* body = m_nonStaticRigidBodies[i];
        btVector3 position = body->getWorldTransform().getOrigin();
        btVector3 v = position - processOrigin;

        if (!IsCollisionShapeInBounds(body->getCollisionShape(), position, processOrigin, processBounds))
        {
            int destCell[3] = {0, 0, 0};

            //Computes the cell coordinates of the process that will be receiving the body.
            destCell[0] = ((int)(v.x() / (2 * processBounds.x())) + cell[0]) % worldBounds[0];
            destCell[1] = ((int)(v.y() / (2 * processBounds.y())) + cell[1]) % worldBounds[1];
            destCell[2] = ((int)(v.z() / (2 * processBounds.z())) + cell[2]) % worldBounds[2];

            int destRank = GetRankFromCellCoords(destCell);

            if (hash.find(destRank) == hash.end())
            {
                btDefaultSerializer* serializer = new btDefaultSerializer();
                serializer->startSerialization();
                //hash.emplace(destRank, serializer);
                //destinations[destRank] = 1;
            }

            body->getCollisionShape()->serializeSingleShape(hash[destRank]);
            body->serializeSingleObject(hash[destRank]);
        }
    }

    char* allDestinations = new char[size * size];
    MPI_Allgather(destinations, size, MPI_CHAR, allDestinations, size * size, MPI_CHAR, MPI_COMM_WORLD);

    delete destinations;

    for (unordered_map<int, btDefaultSerializer*>::iterator i = hash.begin(); i != hash.end(); i++)
    {
        int destRank = i->first;
        btDefaultSerializer* serializer = i->second;
        
        serializer->finishSerialization();
        char* buffer = new char[serializer->getCurrentBufferSize()];

        for (int i = 0; i < serializer->getCurrentBufferSize(); i++)
                buffer[i] = serializer->getBufferPointer()[i];

        cout << "Sending ghost bodies. Source rank: " << rank << " Dest Rank: " << destRank << " size " << serializer->getCurrentBufferSize() << endl;

        MPI_Send(buffer, serializer->getCurrentBufferSize(), MPI_CHAR, destRank, 0, MPI_COMM_WORLD);
        delete buffer;
        delete serializer;
    }

    for (int i = 0; i < size; i++)
    {
        if (allDestinations[rank + i])
        {
            //This means that process i sent data to this process.  We need to receive.
            int message = 0;
            MPI_Status status;
            MPI_Iprobe(MPI_ANY_SOURCE, 0, MPI_COMM_WORLD, &message, &status);
            
            if (message)
            {
                int msgSize = 0;
                MPI_Get_count(&status, MPI_CHAR, &msgSize);
                char* buffer = new char[msgSize];

                cout << "Receiving message from source " << status.MPI_SOURCE << endl;
                MPI_Recv(buffer, msgSize, MPI_CHAR, status.MPI_SOURCE, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                importer.loadFileFromMemory(buffer, msgSize);
                delete buffer;
            }
        }
    }
    delete allDestinations;
}

void HPDDynamicsWorld::RemoveGhostBodies()
{
    //Iterate over all the rigid bodies that this process has and remove those that aren't in the 
    //process bounds.
    for (int i = 0; i < m_nonStaticRigidBodies.size(); i++)
    {
        btRigidBody* body = m_nonStaticRigidBodies[i];

        if (!IsCollisionShapeInBounds(body->getCollisionShape(), body->getWorldTransform().getOrigin(), processOrigin, processBounds))
            btDiscreteDynamicsWorld::removeRigidBody(body);
    }
}

bool HPDDynamicsWorld::IsPointInBounds(btVector3 point, btVector3 pOrigin, btVector3 pBounds)
{
    btVector3 v = point - pOrigin;

    return !(v.x() > pBounds.x() || v.x() < -pBounds.x() ||
             v.y() > pBounds.y() || v.y() < -pBounds.y() ||
             v.z() > pBounds.z() || v.z() < -pBounds.z());
}

bool HPDDynamicsWorld::IsBoxInBounds(btVector3 origin, btVector3 bounds, btVector3 pOrigin, btVector3 pBounds)
{
    btVector3 n = pOrigin - origin;
    n.normalize();

    btVector3 p = btVector3(btFsels(n.x(), bounds.x(), -bounds.x()),
                            btFsels(n.y(), bounds.y(), -bounds.y()),
                            btFsels(n.z(), bounds.z(), -bounds.z()));

    p += origin;

    return IsPointInBounds(p, pOrigin, pBounds);
}

bool HPDDynamicsWorld::IsCollisionShapeInBounds(btCollisionShape* shape, btVector3 origin, btVector3 pOrigin, btVector3 pBounds)
{
    btConvexShape* convexShape = (btConvexShape*)shape;

    if (!convexShape)
        return false;

    btVector3 n = pOrigin - origin;

    n.normalize();

    btVector3 p = convexShape->localGetSupportingVertex(n);
    p += origin;

    return IsPointInBounds(p, pOrigin, pBounds);
}

int HPDDynamicsWorld::GetRankFromCellCoords(int* cellCoords)
{
    return cellCoords[0] + worldBounds[0] * cellCoords[1] + worldBounds[0] * worldBounds[1] * cellCoords[2];
}
