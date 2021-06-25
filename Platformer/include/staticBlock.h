#pragma once

/*!
\file staticBlock.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>

#include "physicalThing.h"

/*! \class StaticBlock
\brief A simple block which can not move or rotate but can collide with stuff
*/

class StaticBlock : public sf::RectangleShape, public PhysicalThing
{
private :

public:
	StaticBlock(b2World* world, const sf::Vector2f& position, const sf::Vector2f& size,sf::Color color, const float& orientation, const bool isGround, uint16 _categoryBits, uint16 _maskBits); //!< Complete contructor

};