#include <vector>
#include <iostream>
//#include <unistd>

#include "Dynamics/btRigidBody.h"

#include "BodyManager.h"

using namespace std;

class FrameManager
{
private:
	/**
	 * This is a list of all of the bodies within the scene.
	 */
	vector<BodyManager>* bodies;


	/**
	 * Gets a list of all bodies that have changed their state since the last update.
	 */
	vector<BodyManager>* GetActiveBodies();

	/**
	 * Converts an unsigned integer to a byte array and writes that to the frame buffer.
	 */
	void WriteUInt(unsigned int n, vector<char>* frame);

	/**
	 * Converts an unsigned short to a byte array and writes that to the frame buffer.
	 */
	void WriteUShort(unsigned short n, vector<char>* frame);

	/**
	 * Converts an unsigned long to a byte array and writes that to the frame buffer.
	 */
	void WriteULong(unsigned long n, vector<char>* frame);

public:
	FrameManager();

	/**
	 * Writes the frame to a buffer that can be saved.
	 */
	void WriteFrame(unsigned int frameID, vector<char>* buffer);

	/**
	 * Adds a new body manager to the frame manager.
	 */
	void AddBody(btRigidBody* body, unsigned int id);
};

