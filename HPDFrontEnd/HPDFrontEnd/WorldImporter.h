#include <btBulletDynamicsCommon.h>
#include <btBulletCollisionCommon.h>
#include <btBulletWorldImporter.h>

#include "HPDMotionState.h"

class WorldImporter: public btBulletWorldImporter
{
public:
	/**
	 * Creates a new world importer, which is responsible for reading binary .bullet files and initializing the world with them.
	 */
	WorldImporter(btDynamicsWorld* world);

	/**
	 * This is overwritten from btBulletWorldImporter to create objects with a custom motion state class.
	 */
	btRigidBody*  createRigidBody(bool isDynamic, btScalar mass, const btTransform& startTransform,	btCollisionShape* shape,const char* bodyName);
};

