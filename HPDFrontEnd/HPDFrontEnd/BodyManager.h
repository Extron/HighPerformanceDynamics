#include <vector>

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
	void WriteStateToFrame(vector<byte>* frame);

private:
	/**
	 * A reference to the Bullet rigid body that this body manager maintains.
	 */
	btRigidBody* body;

	/**
	 * The unique ID of the body.
	 */
	unsigned int id;
};

