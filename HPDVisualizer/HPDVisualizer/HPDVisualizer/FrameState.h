#pragma once

#include <vector>
#include <fstream>
#include <iostream>

#include "ObjectState.h"

using namespace std;

class FrameState 
{
public:
    FrameState();
    ~FrameState();

    unsigned int get_frame_id() { return frameID; }
    unsigned int get_num_objects() { return numOfObjects; }

    ObjectState* get_object(unsigned int i);
	ObjectState* get_object_id(unsigned int id);

    void read(ifstream& fin);

private:
    vector<ObjectState*> objects;
    unsigned int frameID;
	unsigned int numOfObjects;
};

