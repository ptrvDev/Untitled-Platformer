#include "heartCollectible.h"



heartCollectible::heartCollectible(b2World* world, sf::Vector2f position, sf::Vector2f size)
{

	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.type = b2_staticBody;
	l_bodyDef.fixedRotation = true;

	m_body = world->CreateBody(&l_bodyDef);

	l_shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = 1;
	l_fixtureDef.shape = &l_shape;

	m_body->CreateFixture(&l_fixtureDef);
	m_body->SetActive(false);

	pTex = ResourcesManager::Acquire("Assets/heartCollectible.png");

	intRectSource.left = 0;
	intRectSource.top = 0;
	intRectSource.width = 16;
	intRectSource.height = 16;

	setTexture(*pTex);
	setTextureRect(intRectSource);
	setPosition(position);
	setOrigin(sf::Vector2f(16 * 0.5, 16 * 0.5));
	setScale(spriteScaleX, spriteScaleY);



}

void heartCollectible::destroyBody(b2World* world)
{
	std::cout << "heart collected";
	world->DestroyBody(m_body);
}

void heartCollectible::update()
{
	if (animClock.getElapsedTime().asSeconds() > 0.06f) {
		if (intRectSource.left == 176)
		{
			intRectSource.left = 0;
		}
		else {
			intRectSource.left += 16;
		}
		setTextureRect(intRectSource);
		animClock.restart();
	}
}


