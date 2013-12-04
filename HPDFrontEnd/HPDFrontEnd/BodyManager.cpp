#include "BodyManager.h"


BodyManager::BodyManager(btRigidBody* body, unsigned int id)
{
	this->body = body;
	this->id = id;
}

int BodyManager::GetInitialStateSize()
{
    btCollisionShape* shape = body->getCollisionShape();

    btBoxShape* box = dynamic_cast<btBoxShape*>(shape);

    if (box)
        return 26;

    btSphereShape* sphere = dynamic_cast<btSphereShape*>(shape);

    if (sphere)
        return 10;

    btCylinderShape* cylinder = dynamic_cast<btCylinderShape*>(shape);
    
    if (cylinder)
        return 18;

    return 0;
}

void BodyManager::WriteInitialState(vector<char>* frame)
{
    WriteStateToFrame(frame);

    btCollisionShape* shape = body->getCollisionShape();

    btBoxShape* box = dynamic_cast<btBoxShape*>(shape);

    if (box)
    {
        frame->push_back((char)26);
        frame->push_back((char)0);
        WriteVector3(box->getHalfExtentsWithoutMargin() * 2, frame);
        return;
    }

    btSphereShape* sphere = dynamic_cast<btSphereShape*>(shape);

    if (sphere)
    {
        frame->push_back((char)10);
        frame->push_back((char)1);
        WriteScalar(sphere->getRadius(), frame);
        return;
    }

    btCylinderShape* cylinder = dynamic_cast<btCylinderShape*>(shape);

    if (cylinder)
    {
        frame->push_back((char)18);
        frame->push_back((char)2);
        WriteScalar(cylinder->getRadius(), frame);
        WriteScalar(cylinder->getHalfExtentsWithMargin()[cylinder->getUpAxis()], frame);
        return;
    }
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
	for (int i = 0; i < sizeof(n); i++)
		frame->push_back((char)(n >> i * 8 & 0xFF));
}

void BodyManager::WriteUInt(unsigned int n, vector<char>* frame)
{
	for (int i = 0; i < sizeof(n); i++)
		frame->push_back((char)(n >> i * 8 & 0xFF));
}

void BodyManager::WriteScalar(btScalar x, vector<char>* frame)
{
    double value = static_cast<double>(x);
	char* byteArray = reinterpret_cast<char*>(&value);

	for (int i = 0; i < sizeof(double); i++)
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
