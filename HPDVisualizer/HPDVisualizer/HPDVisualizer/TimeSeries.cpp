#include "TimeSeries.h"

TimeSeries::TimeSeries() 
{
    nFrames = 0;
    nextFrameIndex = 0;
}

TimeSeries::~TimeSeries() 
{
    for (unsigned i=0; i < frames.size(); i++)
        delete frames[i];
}

FrameState* TimeSeries::get_frame(unsigned i) 
{
    if (i >= frames.size()) return 0;

    return frames[i];
}

FrameState* TimeSeries::get_next_frame() 
{
    FrameState* nextFrame = get_frame(nextFrameIndex);
    nextFrameIndex++;

    if (nextFrameIndex >= frames.size()) nextFrameIndex = 0;

    return nextFrame;
}


void TimeSeries::read(const char* filename) 
{
    cout << "Loading scene data from " << filename << endl;

    nFrames = 0;
    nextFrameIndex = 0;

    ifstream fin;
    fin.open(filename, ios::in|ios::binary);

    unsigned short headerLengthInBytes;
    fin.read((char*)&headerLengthInBytes, UNSIGNEDSHORT);
    fin.read((char*)&nFrames, UNSIGNED);

	unsigned long long initialFrameSize;
	fin.read((char*)&initialFrameSize, UNSIGNEDLONG);
	
	unsigned int objects;
	fin.read((char*)&objects, UNSIGNED);

	for (unsigned int i = 0; i < objects; i++)
	{
		ObjectState* state = new ObjectState();
		state->read(fin);

		objectStates.push_back(state);

		ObjectType* type = new ObjectType();
		type->read(fin);
		objectTypes.push_back(type);
	}

    for (unsigned int i = 0; i < nFrames; i++)
	{
        FrameState* frame = new FrameState();
		frame->read(fin);
        frames.push_back(frame);
    }

    fin.close();
}
