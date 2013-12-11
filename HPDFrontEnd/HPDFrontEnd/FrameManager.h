#include <vector>
#include <iostream>
#include <fstream>
#include <mpi.h>

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
     * The processor rank of the running process.
     */
    int rank;

    /**
     * The number of running processes.
     */
    int size;



	/**
	 * Gets a list of all bodies that have changed their state since the last update.
	 */
	vector<BodyManager>* GetActiveBodies();

public:
    /**
     * Creates a new frame manager to maintain frame writing.
     *
     * @param rank - The rank of the process that created this frame manager.
     * @param size - The number of processes that are running the program.
     */
	FrameManager(int rank, int size);

    /**
     * Writes the initial state of the world to a buffer.  This is needed because 
     * to render the frames later, we need to know their types, and some bodies will
     * be static, in which case they will not store their positions in the frame buffers,
     * and this provides a way to store them once, so that we know where they are when we render.
     */
    void WriteInitialState(ofstream* fout);

	/**
	 * Writes the frame to a buffer that can be saved.
	 */
	void WriteFrame(unsigned int frameID, ofstream* fout);

	/**
	 * Adds a new body manager to the frame manager.
	 */
	void AddBody(btRigidBody* body, unsigned int id);
};
