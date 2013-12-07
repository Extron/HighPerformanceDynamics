#include "FrameState.h"


FrameState::FrameState()
{
}

FrameState::~FrameState() 
{
    for (unsigned i=0; i < objects.size(); i++)
        delete objects[i];
}

ObjectState * FrameState::get_object(unsigned i) 
{
    if (i >= objects.size()) return 0;

    return objects[i];
}

ObjectState * FrameState::get_object_id(unsigned id) 
{
    for (unsigned int i = 0; i < objects.size(); i++)
	{
		if (id == objects[i]->get_object_id())
			return objects[i];
	}

	return 0;
}

void FrameState::read(ifstream& fin)
{
    unsigned long long frameLengthInBytes;
    fin.read((char*)&frameLengthInBytes, UNSIGNEDLONG);
    fin.read((char*)&frameID, UNSIGNED);
    fin.read((char*)&numOfObjects, UNSIGNED);

    unsigned short singleObjectLengthInBytes;
    fin.read((char*)&singleObjectLengthInBytes, UNSIGNEDSHORT);

    for (unsigned int i = 0; i < numOfObjects; i++)
	{
        ObjectState* o = new ObjectState;
		o->read(fin);
		objects.push_back(o);
    }
}
