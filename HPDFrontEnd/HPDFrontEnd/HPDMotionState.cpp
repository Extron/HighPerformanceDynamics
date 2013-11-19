#include "HPDMotionState.h"


HPDMotionState::HPDMotionState(const btTransform& startTrans, const btTransform& centerOfMassOffset)
{
	stateChanged = false;
}

void HPDMotionState::setWorldTransform(const btTransform& centerOfMassWorldTrans)
{
	m_graphicsWorldTrans = centerOfMassWorldTrans * m_centerOfMassOffset;
	stateChanged = true;
}