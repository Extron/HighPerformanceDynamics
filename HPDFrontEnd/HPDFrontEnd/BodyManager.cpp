#include "BodyManager.h"


BodyManager::BodyManager(btRigidBody* body, unsigned int id)
{
	this->body = body;
	this->id = id;
}

void BodyManager::WriteStateToFrame(vector<byte>* frame)
{
	HPDMotionState motionState = (HPDMotionState*)body->getMotionState();

	if (motionState != null && motionState->HasStateChanged())
	{
		btTransform transform;

		motionState->getWorldTransform(transform);


	}
}
