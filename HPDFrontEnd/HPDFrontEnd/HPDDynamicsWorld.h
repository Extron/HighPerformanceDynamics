#pragma once


#include "btCollisionCommon.h"

class HPDDynamicsWorld : public btDiscreteDynamicsWorld
{
public:
    /**
     * Creates an MPI-friendly dynamic world in Bullet.
     */
    HPDDynamicsWorld(int rank, int size, btVector3 processOrigin, btVector3 processBounds, 
                     btVcebtDispatcher* dispatcher, btBroadphaseInterface* pairCache, 
                     btConstraintSolver* constraintSolver, btCollisionConfiguration* collisionConfiguration);

    
    /**
     * This function is overridden from the parent btCollisionWorld, and allows individual processes to manage their 
     * own lists of bodies.
     * */
    void addCollisionObject(btCollisionObject* collisionObject, short collisionFilterGroup, short collisionFilterMask);


private:
    /**
     * To help with MPI communication when sending objects to other processes, each process maintains a
     * list of all of the objects in the full world.  This means that when sending an object to another 
     * process, all it needs to send is the index into this array.
     */
    btAlignedObjectArray<btCollisionObject*> worldObjects;
    
    /**
     * The location in the world of the bounding box of this process.
     */
    btVector3 processOrigin;
    
    /**
     * The size of the bounding box of this process.
     */
    btVector3 processBounds;
        
    /**
     * The rank of the local process that owns this world.
     */
    int rank;


    /**
     * The number of processes running the program.
     * */
    int size;



    /**
     * Gets whether a specified point is in the bounds defined for the process running this local world.
     */
    bool IsPointInBounds(btVector3 point);

    /**
     * Gets whether a specified aabb is in the bounds defined for the process running this local world.
     */
    bool IsBoxInBounds(btVector3 origin, btVector3 bounds);

    /**
     * Gets whether a specified collision shape is in the bounds defined for the process running this local world.
     */
    bool IsCollisionShapeInBounds(btCollisionShape* shape, btVector3 origin);
}
