#include "graphics.h"

graphics::graphics()
{
}

graphics::~graphics()
{
}

graphics::graphics(const sf::Vector2f position, int layer)
{
	if (layer == 1)
	{
		pTex = ResourcesManager::Acquire("Assets/BackgroundLayer.png");
		setTexture(*pTex);
		setPosition(position);
		setScale(0.01, 0.01);
		setOrigin(sf::Vector2f(getTexture()->getSize().x * 0.5f, getTexture()->getSize().y * 0.5f));
	}
	else if (layer == 2)
	{
		pTex = ResourcesManager::Acquire("Assets/playerLayer.png");
		setTexture(*pTex);
		setPosition(position);
		setScale(0.01, 0.01);
		setOrigin(sf::Vector2f(getTexture()->getSize().x * 0.5f, getTexture()->getSize().y * 0.5f)); b2BodyDef l_bodyDef;
	}
	else if (layer == 3)
	{
		pTex = ResourcesManager::Acquire("Assets/ForegroundLayer.png");
		setTexture(*pTex);
		setPosition(position);
		setScale(0.01, 0.01);
		setOrigin(sf::Vector2f(getTexture()->getSize().x * 0.5f, getTexture()->getSize().y * 0.5f));
	}
}

