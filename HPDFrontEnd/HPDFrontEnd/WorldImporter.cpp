#include "WorldImporter.h"


WorldImporter::WorldImporter(btDynamicsWorld* world)
	:btBulletWorldImporter(world)
{
}

btRigidBody*  WorldImporter::createRigidBody(bool isDynamic, btScalar mass, const btTransform& startTransform, btCollisionShape* shape, const char* bodyName)
{
	btVector3 localInertia;
	localInertia.setZero();

	if (mass)
		shape->calculateLocalInertia(mass, localInertia);
	
	HPDMotionState* motionState = new HPDMotionState(startTransform);

	btRigidBody* body = new btRigidBody(mass, motionState, shape, localInertia);	
	body->setWorldTransform(startTransform);

	if (m_dynamicsWorld)
		m_dynamicsWorld->addRigidBody(body);
	
	if (bodyName)
	{
		char* newname = duplicateName(bodyName);
		m_objectNameMap.insert(body,newname);
		m_nameBodyMap.insert(newname,body);
	}

	m_allocatedRigidBodies.push_back(body);
	return body;
}