#include "HPDDynamicsWorld.h"

HPDDynamicsWorld::HPDDynamicsWorld(int rank, int size, btVector3 processOrigin, btVector3 processBounds, 
                                   btDispatcher* dispatcher, btBroadphaseInterface* pairCache, 
                                   btConstraintSolver* constraintSolver, btCollisionConfiguration* collisionConfiguration)
    : btDiscreteDynamicsWorld(dispatcher, pairCache, constraintSolver, collisionConfiguration)
{
    this->rank = rank;
    this->size = size;
    this->processOrigin = processOrigin;
    this->processBounds = processBounds;
}

void HPDDynamicsWorld::addCollisionObject(btCollisionObject* collisionObject, short collisionFilterGroup,short collisionFilterMask)
{
    
}

bool HPDDynamicsWorld::IsPointInBounds(btVector3 point)
{
    btVector3 v = point - processOrigin;

    return !(v.x() > processBounds.x() || v.x() < -processBounds.x() ||
             v.y() > processBounds.y() || v.y() < -processBounds.y() ||
             v.z() > processBounds.z() || v.z() < -processBounds.z())
}

bool HPDDynamicsWorld::IsBoxInBounds(btVector3 origin, btVector3 bounds)
{
    btVector3 n = processorOrigin - origin;
    n.normalize();

    btVector3 p = btVector3(btFsels(n.x(), bounds.x(), -bounds.x()),
                            btFsels(n.y(), bounds.y(), -bounds.y()),
                            btFsels(n.z(), bounds.z(), -bounds.z()));

    p += origin;

    return IsPointInBounds(p);
}

bool HPDDynamicsWorld::IsCollisionShapeInBounds(btCollisionShape* shape, btVector3 origin)
{
    btVector3 n = processOrigin - origin;

    
}
