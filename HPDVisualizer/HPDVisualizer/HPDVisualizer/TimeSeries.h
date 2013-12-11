#pragma once

#include "FrameState.h"
#include "ObjectState.h"
#include "ObjectType.h"

class TimeSeries 
{
public:
    TimeSeries();
    ~TimeSeries();

    void read(const char* filename);
    FrameState* get_frame(unsigned int i);
    FrameState* get_next_frame();

	vector<ObjectType*> objectTypes;
	vector<ObjectState*> objectStates;

private:
    vector<FrameState*> frames;
    unsigned int nFrames;
    unsigned int nextFrameIndex;
};
