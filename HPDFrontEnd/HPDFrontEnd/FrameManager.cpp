#include "FrameManager.h"


FrameManager::FrameManager()
{
	bodies = new vector<BodyManager>();
}

void FrameManager::WriteFrame(unsigned int frameID, vector<char>* buffer)
{
	//TODO: This code is not parallizable yet.  Needs to be eventually to help speed up file read/write.
	vector<BodyManager>* activeBodies = GetActiveBodies();

	unsigned long length = (unsigned long)activeBodies->size() * (unsigned long)60 + (unsigned long)18;
	unsigned int count = activeBodies->size();
	unsigned short size = 60;

	WriteULong(length, buffer);
	WriteUInt(frameID, buffer);
	WriteUInt(count, buffer);
	WriteUShort(size, buffer);

	for (vector<BodyManager>::iterator i = activeBodies->begin(); i != activeBodies->end(); i++)
		i->WriteStateToFrame(buffer);
}

void FrameManager::AddBody(btRigidBody* body, unsigned int id)
{
	bodies->push_back(BodyManager(body, id));
}

vector<BodyManager>* FrameManager::GetActiveBodies()
{
	vector<BodyManager>* activeBodies = new vector<BodyManager>();

	for (vector<BodyManager>::iterator i = bodies->begin(); i != bodies->end(); i++)
	{
		HPDMotionState* motionState = dynamic_cast<HPDMotionState*>(i->GetMotionState());

		if (motionState && motionState->HasStateChanged())
			activeBodies->push_back(*i);
	}

	return activeBodies;
}

void FrameManager::WriteUInt(unsigned int n, vector<char>* frame)
{
	for (int i = 0; i < sizeof(n); i++)
		frame->push_back((char)(n >> i * 8 & 0xFF));
}

void FrameManager::WriteUShort(unsigned short n, vector<char>* frame)
{
	for (int i = 0; i < sizeof(n); i++)
		frame->push_back((char)(n >> i * 8 & 0xFF));
}

void FrameManager::WriteULong(unsigned long n, vector<char>* frame)
{
	for (int i = 0; i < sizeof(n); i++)
		frame->push_back((char)(n >> i * 8 & 0xFF));
}
