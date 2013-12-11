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

void BodyManager::WriteInitialState(ofstream* fout)
{
    WriteStateToFrame(fout);

    btCollisionShape* shape = body->getCollisionShape();

    btBoxShape* box = dynamic_cast<btBoxShape*>(shape);

    char size, type;

    if (box)
    {
        size = 26;
        type = 0;
        fout->write(&size, sizeof(char));
        fout->write(&type, sizeof(char));

        WriteVector3(box->getHalfExtentsWithoutMargin() * 2, fout);
        return;
    }

    btSphereShape* sphere = dynamic_cast<btSphereShape*>(shape);

    if (sphere)
    {        
        size = 10;
        type = 1;
        fout->write(&size, sizeof(char));
        fout->write(&type, sizeof(char));

        WriteScalar(sphere->getRadius(), fout);
        return;
    }

    btCylinderShape* cylinder = dynamic_cast<btCylinderShape*>(shape);

    if (cylinder)
    {
        size = 18;
        type = 2;
        fout->write(&size, sizeof(char));
        fout->write(&type, sizeof(char));

        WriteScalar(cylinder->getRadius(), fout);
        WriteScalar(cylinder->getHalfExtentsWithMargin()[cylinder->getUpAxis()], fout);
        return;
    }
}

void BodyManager::WriteStateToFrame(ofstream* fout)
{
	HPDMotionState* motionState = dynamic_cast<HPDMotionState*>(body->getMotionState());

	if (motionState && motionState->HasStateChanged())
	{
		btTransform transform;

		motionState->getWorldTransform(transform);

        fout->write((char*)&id, sizeof(unsigned int));

		WriteVector3(transform.getOrigin(), fout);
		btQuaternion rotation = transform.getRotation();
		WriteAxisAngle(rotation.getAxis(), rotation.getAngle(), fout);

		motionState->ResetStateChanged();
	}
}

btMotionState* BodyManager::GetMotionState()
{
	return body->getMotionState();
}

void BodyManager::WriteScalar(btScalar x, ofstream* fout)
{
    double value = static_cast<double>(x);
	char* byteArray = reinterpret_cast<char*>(&value);
    fout->write(byteArray, sizeof(double));
}

void BodyManager::WriteVector3(btVector3 v, ofstream* fout)
{
	WriteScalar(v.x(), fout);
	WriteScalar(v.y(), fout);
	WriteScalar(v.z(), fout);
}

void BodyManager::WriteAxisAngle(btVector3 a, btScalar th, ofstream* fout)
{
	WriteScalar(a.x(), fout);
	WriteScalar(a.y(), fout);
	WriteScalar(a.z(), fout);
	WriteScalar(th, fout);
}
