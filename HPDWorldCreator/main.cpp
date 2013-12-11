#include <iostream>
#include <cstring>

#include "btBulletDynamicsCommon.h"
#include "LinearMath/btSerializer.h"

using namespace std;

char* FindArg(int argc, char** argv, char flag)
{
    for (int i = 0; i < argc; i++)
    {
        if (argv[i][0] == '-' && argv[i][1] == flag && i < argc - 1)
            return argv[i + 1];
    }

    return 0;
}

btVector3 GetPosition(int index)
{
    btVector3 position(0, 0, 0);

    return position;
}

btQuaternion GetOrientation(int index)
{
    btVector3 axis(1, 0, 0);

    btQuaternion orientation(axis, 0);

    return orientation;
}

btScalar GetMass(int index)
{
    return 1;
}

btCollisionShape* GetShape(char* type, int index)
{
    btCollisionShape* shape;

    if (!type)
        return new btBoxShape(btVector3(1, 1, 1));

    if (!strcmp(type, "box"))
        shape = new btBoxShape(btVector3(1, 1, 1));
    else if (!strcmp(type, "sphere"))
        shape = new btSphereShape(1);
    else if (!strcmp(type, "cylinder"))
        shape = new btCylinderShape(btVector3(1, 1, 1));
    else 
        shape = new btBoxShape(btVector3(1, 1, 1));

    return shape;
}

int main(int argc, char** argv)
{
    char* objCountStr = FindArg(argc, argv, 'n');
    char* objType = FindArg(argc, argv, 't');
    char* output = FindArg(argc, argv, 'f');

    unsigned int objCount = 100;

    if (objCountStr)
        objCount = atoi(objCountStr);

    cout << "Beginning world creation." << endl;
    
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);
    btBroadphaseInterface* overlappingPairCache = new btDbvtBroadphase();
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    btDiscreteDynamicsWorld* world = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);
    world->setGravity(btVector3(0, -10, 0));

    //Add the floor of the level
    btBoxShape* groundShape = new btBoxShape(btVector3(btScalar(100),btScalar(0.25),btScalar(100)));

    btTransform groundTransform;
    groundTransform.setIdentity();
    groundTransform.setOrigin(btVector3(-50, 0, -50));

    btVector3 inertia(0, 0, 0);

    btDefaultMotionState* groundMS = new btDefaultMotionState(groundTransform);

    btRigidBody::btRigidBodyConstructionInfo rbInfo(0.0, groundMS, groundShape, inertia);
    btRigidBody* body = new btRigidBody(rbInfo);
    world->addRigidBody(body);

    for (int i = 0; i < objCount; i++)
    {
        cout << "Adding object " << i << endl;

        btCollisionShape* shape = GetShape(objType, i);

        btScalar mass = GetMass(i);

        if (mass)
            shape->calculateLocalInertia(mass, inertia);

        btTransform transform;
        transform.setIdentity();
        transform.setOrigin(GetPosition(i));
        transform.setRotation(GetOrientation(i));

        btDefaultMotionState* ms = new btDefaultMotionState(transform);

        btRigidBody* rb = new btRigidBody(mass, ms, shape, inertia);
        rb->setWorldTransform(transform);

        world->addRigidBody(rb);
    }

    btDefaultSerializer* serializer = new btDefaultSerializer();
    world->serialize(serializer);

    cout << "Writing world to file." << endl;

    FILE* file = fopen(output, "wb");
    fwrite(serializer->getBufferPointer(), serializer->getCurrentBufferSize(), 1, file);
    fclose(file);

    return 0;
}
