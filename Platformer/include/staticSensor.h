#pragma once

#pragma once

/*!
\file staticSensor.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "physicalThing.h"

/*! \class StaticSensor
\brief A simple block which can not move or rotate and does not coliide with circles
*/

class StaticSensor : public sf::RectangleShape, public PhysicalThing
{
public:
	void initRectangle(b2World * world, sf::Vector2f position, sf::Vector2f size, float orientation, uint16 _categoryBits, uint16 _maskBits); 
	void initConvex(b2World* world, sf::Vector2f& position, b2Vec2* vertices, int verticesCount, uint16 _categoryBits, uint16 _maskBits);
	void initCircle(b2World * world, sf::Vector2f position, float radius, uint16 _categoryBits, uint16 _maskBits);
	virtual void onAction(b2Body * other) = 0; //!< Thing the sensor does when hit
	virtual void offAction(b2Body * other) = 0; //!< Thing the sensor does when hit stops

};