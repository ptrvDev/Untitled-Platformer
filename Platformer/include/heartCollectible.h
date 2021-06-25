#pragma once

#include "collectible.h"

class heartCollectible : public collectible
{

public:

	heartCollectible() {};
	~heartCollectible() {};
	heartCollectible(b2World* world, sf::Vector2f position, sf::Vector2f size);
	void destroyBody(b2World* world);
	void update();

};

