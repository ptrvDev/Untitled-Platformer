#pragma once

/*!
\file dynamicBlock.h
*/

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "physicalThing.h"
#include "resourcesManager.h"

/*! \class DynamicBlock
\brief A simple block which can move, rotate and collide with stuff
*/

class DynamicBlock : public sf::Sprite, public PhysicalThing
{
private:
	b2Fixture* fix;
	b2BodyDef l_bodyDef;
	b2PolygonShape l_shape;
	b2FixtureDef l_fixtureDef;
	sf::Vector2f _position;
	sf::Vector2f _size;
	float _orientation;

	float fixedPosX;
	float fixedPosY;
	bool startTimer = false;
	sf::Clock clock;
	std::shared_ptr<sf::Texture> pTex; 

public:

	DynamicBlock(b2World * world, const sf::Vector2f& position, const sf::Vector2f &size, const float orientation, sf::Color color, uint16 _categoryBits, uint16 _maskBits); //!< Complete contructor
	void update(); //!< Update rendering infomation 
	void enableTimer();
	void fallDown();
	b2Vec2 getPos();
	void destroyBody(b2World* world);

};