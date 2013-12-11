#pragma once

#include <btDefaultMotionState.h>

/**
 * This motion state is designed to easily maintain frame generation.  To ensure an efficient frame write, this
 * class stores whether or not the body's state has changed since the last frame.
 */
class HPDMotionState : public btDefaultMotionState
{
private:
	/**
	 * Indicates whether the motion state was changed recently.
	 */
	bool stateChanged;

public:
	/**
	 * Creates a new HPD motion state.
	 */
	HPDMotionState(const btTransform& startTrans = btTransform::getIdentity(), const btTransform& centerOfMassOffset = btTransform::getIdentity());

    ~HPDMotionState() {};

	/**
	 * Gets wether the motion state has changed since last checked.
	 */
	bool HasStateChanged() { return stateChanged; };

	/**
	 * Resets the motion state, indicating that it was recently checked.
	 */
	void ResetStateChanged() { stateChanged = false; };

	/**
	 * Sets the state's world transform.  When this is called, the state changed flag is set to true.
	 */
	void setWorldTransform(const btTransform& centerOfMassWorldTrans);
};

