#include "SensorContactListener.h"

void SensorContactListener::BeginContact(b2Contact * contact)
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	bool isSensorA = contact->GetFixtureA()->IsSensor();
	bool isSensorB = contact->GetFixtureB()->IsSensor();

	if (isSensorA)
	{
		StaticSensor * sensor = static_cast<StaticSensor *>(bodyA->GetUserData());
		sensor->onAction(bodyB);
	}

	if (isSensorB)
	{
		StaticSensor * sensor = static_cast<StaticSensor *>(bodyB->GetUserData());
			sensor->onAction(bodyA);
	}

}

void SensorContactListener::EndContact(b2Contact * contact)		
{
	b2Body* bodyA = contact->GetFixtureA()->GetBody();
	b2Body* bodyB = contact->GetFixtureB()->GetBody();

	bool isSensorA = contact->GetFixtureA()->IsSensor();
	bool isSensorB = contact->GetFixtureB()->IsSensor();

	if (isSensorA)
	{
		StaticSensor * sensor = static_cast<StaticSensor *>(bodyA->GetUserData());
		sensor->offAction(bodyB);
	}

	if (isSensorB)
	{
		StaticSensor * sensor = static_cast<StaticSensor *>(bodyB->GetUserData());
		sensor->offAction(bodyA);
	}





}

