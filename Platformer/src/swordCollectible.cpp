#include "swordCollectible.h"

swordCollectible::swordCollectible(b2World* world, sf::Vector2f position, sf::Vector2f size)
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

	//CHANGE PICTURE!!!
	pTex = ResourcesManager::Acquire("Assets/Sword.png");


	setTexture(*pTex);
	setPosition(position);
	setOrigin(sf::Vector2f(16 * 0.5, 16 * 0.5));
	setScale(spriteScaleX, spriteScaleY);

}

void swordCollectible::destroyBody(b2World* world)
{
	std::cout << "sword collected";
	world->DestroyBody(m_body);
}
