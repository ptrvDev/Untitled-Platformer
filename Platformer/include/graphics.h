#pragma once
#include <iostream>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include "ResourcesManager.h"
#include "physicalThing.h"

#include "staticBlock.h"


class graphics : public sf::Sprite, public PhysicalThing
{

private:

	std::vector<StaticBlock> m_staticBlocks;
	std::shared_ptr<sf::Texture> pTex;
	b2World* m_pWorld = nullptr;
public:

	graphics();
	~graphics();
	graphics(const sf::Vector2f position, int layer);
	//layer 1 - foreground (set size to 0)
	//layer 2 - inbetween (set size to 0)
	//layer 3 - background (set size to 0)



	


};

