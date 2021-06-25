#pragma once
#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include <iostream>

#include "resourcesManager.h"
#include "physicalThing.h"

class collectible : public sf::Sprite, public PhysicalThing
{
protected:

	uint16 category; //!< used for collision filtering
	uint16 mask; //!< used for collision filtering

	std::shared_ptr<sf::Texture> pTex; //!< A shared pointer to the texture used for the character
	sf::IntRect intRectSource; //!< The internal rectangle(tile) of the whole PNG image (used for animation)
	sf::Clock animClock; //!< Clock used for the animation to switch to the next tile 

	float spriteScaleX = 0.01; //!< the value used to scale down the X coordinate of the PNG
	float spriteScaleY = 0.01; //!< the value used to scale down the Y coordinate of the PNG

	//Box2D stuff
	b2BodyDef l_bodyDef; //!< collectible box2D body definition
	b2PolygonShape l_shape; //!< collectible box2D shape
	b2FixtureDef l_fixtureDef; //!<collectible box2D  fixture definition

public:

	virtual void destroyBody(b2World* world) = 0;

};

