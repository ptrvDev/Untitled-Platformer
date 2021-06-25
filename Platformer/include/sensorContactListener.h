#pragma once

/*!
\file SensorContactListener.h
*/

#include <Box2D\Box2D.h>

#include "staticSensor.h"

/*! \class SensorContactListener
\brief A listener which listens for contacts between games objects and sensors
*/

class SensorContactListener : public b2ContactListener
{ 
public:
	int numFootContacts = 0;
	void BeginContact(b2Contact* contact); //!< Called when two fixtures begin to touch
	void EndContact(b2Contact* contact); //!< Called when two fixtures cease to touch

};