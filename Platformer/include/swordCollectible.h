#pragma once
#include "collectible.h"

class swordCollectible : public collectible
{

public:

	swordCollectible() {};
	~swordCollectible() {};
	swordCollectible(b2World* world, sf::Vector2f position, sf::Vector2f size);
	void destroyBody(b2World* world);

};

