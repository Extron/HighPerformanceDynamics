#include <vector>
#include <fstream>

#include "Dynamics/btRigidBody.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btScalar.h"
#include <btBulletCollisionCommon.h>

#include "HPDMotionState.h"


using namespace std;

/**
 * A body manager strores references and ID values to each body within the world, which allows its properties to be quickly written 
 * to each frame.
 */
class BodyManager
{
public:
	/**
	 * Creates a new rigid body manager.
	 */
	BodyManager(btRigidBody* body, unsigned int id);
   
    /**
     * Gets the size of the initial buffer of the object, used when writing the
     * object's initial state to a file.
     */ 
    int GetInitialStateSize();

    /**
     * Writes the initial state of the body to a file.
     *
     * @param fout - The file to write to.
     */
    void WriteInitialState(ofstream* fout);

	/**
	 * Writes the body's state to a file.
     *
     * @param fout - The file to write to.
	 */
	void WriteStateToFrame(ofstream* fout);

	/** 
     * Gets the motion state of the body.
	 */
	btMotionState* GetMotionState();

private:
	/**
	 * A reference to the Bullet rigid body that this body manager maintains.
	 */
	btRigidBody* body;

	/**
	 * The unique ID of the body.
	 */
	unsigned int id;



	/**
	 * Converts a btScalar to a byte array and writes that to the frame buffer.
	 */
	void WriteScalar(btScalar x, ofstream* fout);

	/**
	 * Converts a btVector to a byte array and writes that to the frame buffer.
	 */
	void WriteVector3(btVector3 v, ofstream* fout);

	/**
	 * Converts an axis-angle (btVector, btScalar) to a byte array and writes that to the frame buffer.
	 */
	void WriteAxisAngle(btVector3 a, btScalar th, ofstream* fout);
};

