#include <vector>

#include "Dynamics/btRigidBody.h"
#include "LinearMath/btVector3.h"
#include "LinearMath/btScalar.h"

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
	 * Writes the body's state to a given frame buffer.
	 */
	void WriteStateToFrame(vector<char>* frame);

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
	 * Converts an integer to a byte array and writes that to the frame buffer.
	 */
	void WriteInt(int n, vector<char>* frame);

	/**
	 * Converts an integer to a byte array and writes that to the frame buffer.
	 */
	void WriteUInt(unsigned int n, vector<char>* frame);

	/**
	 * Converts a btScalar to a byte array and writes that to the frame buffer.
	 */
	void WriteScalar(btScalar x, vector<char>* frame);

	/**
	 * Converts a btVector to a byte array and writes that to the frame buffer.
	 */
	void WriteVector3(btVector3 v, vector<char>* frame);

	/**
	 * Converts an axis-angle (btVector, btScalar) to a byte array and writes that to the frame buffer.
	 */
	void WriteAxisAngle(btVector3 a, btScalar th, vector<char>* frame);
};

