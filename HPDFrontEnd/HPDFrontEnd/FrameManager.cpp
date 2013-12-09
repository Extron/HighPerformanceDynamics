#include "FrameManager.h"


FrameManager::FrameManager(int rank, int size)
{
	bodies = new vector<BodyManager>();

    this->rank = rank;
    this->size = size;
}

void FrameManager::WriteInitialState(ofstream* fout)
{
    unsigned long length;
    unsigned int count = bodies->size();

    for (vector<BodyManager>::iterator i = bodies->begin(); i != bodies->end(); i++)
        length += (unsigned long)i->GetInitialStateSize();

    unsigned int* counts = new unsigned int[size];
    unsigned long* lengths = new unsigned long[size];

    MPI_Allgather(&count, 1, MPI_UNSIGNED, counts, 1, MPI_UNSIGNED, MPI_COMM_WORLD);
    MPI_Allgather(&length, 1, MPI_UNSIGNED_LONG, lengths, 1, MPI_UNSIGNED_LONG, MPI_COMM_WORLD);

    unsigned long streamPos = fout->tellp();
    unsigned int totalCount = 0;
    unsigned long totalLength = 0;

    if (!rank)
    {
        for (int i = 0; i < size; i++)
        {
            totalLength += lengths[i];
            totalCount += counts[i];
        }

        fout->write((char*)&length, sizeof(unsigned long));
        fout->write((char*)&totalCount, sizeof(unsigned int));

        for (vector<BodyManager>::iterator i = bodies->begin(); i != bodies->end(); i++)
            i->WriteInitialState(fout);
    }
    else
    {
        //First, we need to compute the position in the file to start writing.  This is done by
        //looking at the size of all previous process' file slices and adding them.
        for (int i = 0; i < rank; i++)
            totalLength += lengths[i];

        fout->seekp(streamPos + totalLength);

        for (vector<BodyManager>::iterator i = bodies->begin(); i != bodies->end(); i++)
            i->WriteInitialState(fout);
    }
}

void FrameManager::WriteFrame(unsigned int frameID, ofstream* fout)
{
    //The active bodies will be different for each processor.  Since frame size is not determined prior to
    //write, we need to communicate each process's frame slice size before writing, so each frame knows
    //where to write. We can also use asynchronous writing to speed this up, too.
	vector<BodyManager>* activeBodies = GetActiveBodies();

	unsigned int count = activeBodies->size();
	unsigned short objSize = 60;

    //Let process 0 be the first frame slice.  Thus, it needs to know the sum of all process slice sizes.
    
    unsigned int* counts = new unsigned int[size];

    //MPI_Allgather will grab all of the count variables from each process, combine them into a single
    //array, and then disribute that array to all processes.
    MPI_Allgather(&count, 1, MPI_UNSIGNED, counts, 1, MPI_UNSIGNED, MPI_COMM_WORLD);

    unsigned long streamPos = fout->tellp();
    unsigned int totalCount = 0;

    if (!rank)
    {
        for (int i = 0; i < size; i++)
            totalCount += counts[i];

        unsigned long length = (unsigned long)totalCount * (unsigned long)objSize + (unsigned long)18;

        fout->write((char*)&length, sizeof(unsigned long));
        fout->write((char*)&frameID, sizeof(unsigned int));
        fout->write((char*)&totalCount, sizeof(unsigned int));
        fout->write((char*)&objSize, sizeof(unsigned short));

        for (vector<BodyManager>::iterator i = activeBodies->begin(); i != activeBodies->end(); i++)
            i->WriteStateToFrame(fout);
    }
    else
    {
        //First, we need to compute the position in the file to start writing.  This is done by
        //looking at the size of all previous process' file slices and adding them.
        for (int i = 0; i < rank; i++)
            totalCount += counts[i];
            
        unsigned long length = (unsigned long)totalCount * (unsigned long)objSize + (unsigned long)18;
        
        fout->seekp(streamPos + length);

        for (vector<BodyManager>::iterator i = activeBodies->begin(); i != activeBodies->end(); i++)
            i->WriteStateToFrame(fout);                    
    }
}

void FrameManager::AddBody(btRigidBody* body, unsigned int id)
{
	bodies->push_back(BodyManager(body, id));
}

/**
 * This method is assumed to return only the active bodies that each processor is responsible for, and not all
 * of the active bodies in the world.
 */
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
