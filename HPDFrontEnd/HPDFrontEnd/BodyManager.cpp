#include "BodyManager.h"


BodyManager::BodyManager(btRigidBody* body, unsigned int id)
{
	this->body = body;
	this->id = id;
}

void BodyManager::WriteStateToFrame(vector<char>* frame)
{
	HPDMotionState* motionState = dynamic_cast<HPDMotionState*>(body->getMotionState());

	if (motionState && motionState->HasStateChanged())
	{
		btTransform transform;

		motionState->getWorldTransform(transform);

		WriteUInt(id, frame);

		WriteVector3(transform.getOrigin(), frame);

		btQuaternion rotation = transform.getRotation();
		WriteAxisAngle(rotation.getAxis(), rotation.getAngle(), frame);

		motionState->ResetStateChanged();
	}
}

btMotionState* BodyManager::GetMotionState()
{
	return body->getMotionState();
}

void BodyManager::WriteInt(int n, vector<char>* frame)
{
	char* byteArray = (char*)n;

	for (int i = 0; i < sizeof(n); i++)
		frame->push_back(byteArray[i]);
}

void BodyManager::WriteUInt(unsigned int n, vector<char>* frame)
{
	char* byteArray = (char*)n;

	for (int i = 0; i < sizeof(n); i++)
		frame->push_back(byteArray[i]);
}

void BodyManager::WriteScalar(btScalar x, vector<char>* frame)
{
	char* byteArray = static_cast<char*>(x);

	for (int i = 0; i < sizeof(x); i++)
		frame->push_back(byteArray[i]);
}

void BodyManager::WriteVector3(btVector3 v, vector<char>* frame)
{
	WriteScalar(v.x(), frame);
	WriteScalar(v.y(), frame);
	WriteScalar(v.z(), frame);
}

void BodyManager::WriteAxisAngle(btVector3 a, btScalar th, vector<char>* frame)
{
	WriteScalar(a.x(), frame);
	WriteScalar(a.y(), frame);
	WriteScalar(a.z(), frame);
	WriteScalar(th, frame);
}