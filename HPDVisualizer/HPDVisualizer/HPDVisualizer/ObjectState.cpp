#include "ObjectState.h"


ObjectState::ObjectState() 
{
    objectID = 0;
    xPos = 0;
    yPos = 0;
    zPos = 0;
    xRot = 0;
    yRot = 0;
    zRot = 0;
    aRot = 0;
}

ObjectState::~ObjectState() 
{
}

void ObjectState::read(ifstream& fin) 
{
    fin.read((char*)&objectID, UNSIGNED);
    fin.read((char*)&xPos, DOUBLE);
    fin.read((char*)&yPos, DOUBLE);
    fin.read((char*)&zPos, DOUBLE);
    fin.read((char*)&xRot, DOUBLE);
    fin.read((char*)&yRot, DOUBLE);
    fin.read((char*)&zRot, DOUBLE);
    fin.read((char*)&aRot, DOUBLE);
}