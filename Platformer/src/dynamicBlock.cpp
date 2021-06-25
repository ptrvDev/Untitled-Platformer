/*!
\file dynamicBlock.cpp
*/

#include "dynamicBlock.h"

DynamicBlock::DynamicBlock(b2World * world, const sf::Vector2f& position, const sf::Vector2f &size, const float orientation, sf::Color color, uint16 _categoryBits, uint16 _maskBits)
{
	fixedPosX = position.x;
	fixedPosY = position.y;
	_position = position;
	_size = size;
	_orientation = orientation;

	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.angle = orientation * DEG2RAD;
	l_bodyDef.type = b2_dynamicBody;
	l_bodyDef.gravityScale = 0;
	l_bodyDef.fixedRotation = true;


	m_body = world->CreateBody(&l_bodyDef);
	m_body->SetUserData(this); // used by our contact listener

	l_shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = mk_fDensity * 1000;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = _categoryBits;
	l_fixtureDef.filter.maskBits = _maskBits;

	m_body->CreateFixture(&l_fixtureDef);
	fix = m_body->GetFixtureList();
	pTex = ResourcesManager::Acquire("Assets/wood.png");

	setTexture(*pTex);
	setPosition(position);
	setOrigin(44 * 0.5f, 9 * 0.5f);
	setRotation(orientation);
	setScale(0.01, 0.01);


}

void DynamicBlock::update()
{

	if (startTimer) {
		b2Vec2 pos = m_body->GetPosition();
		setPosition(pos.x, pos.y);
		if (clock.getElapsedTime().asSeconds() > 1) {
			fallDown();
		}

			//std::cout << "FALLING";
		
	}
		b2Vec2 pos = m_body->GetPosition();
		setPosition(pos.x, pos.y);
		float angle = m_body->GetAngle() * RAD2DEG;
		setRotation(angle);



	//if (goToY != 0) {

		//if (pos.y < goToY) {
		//	b2Vec2 linVel = m_body->GetLinearVelocity();
		//	float velChange = speedY - linVel.x;
		//	float impulse = m_body->GetMass() * velChange;
		//	m_body->ApplyLinearImpulseToCenter(b2Vec2(0, speedY), true);
		//}
	//}



}

b2Vec2 DynamicBlock::getPos()
{
	b2Vec2 pos = m_body->GetPosition();
	return pos;
}

void DynamicBlock::destroyBody(b2World* world)
{
	world->DestroyBody(m_body);
}

void DynamicBlock::enableTimer()
{
	startTimer = true;
	clock.restart();
}

void DynamicBlock::fallDown()
{
	m_body->DestroyFixture(fix);
	l_shape.SetAsBox(_size.x * 0.5f, _size.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = 0 ;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = 34;
	l_fixtureDef.filter.maskBits = 1;

	m_body->CreateFixture(&l_fixtureDef);

	b2Vec2 linVel = m_body->GetLinearVelocity();
	float velChange = 0.25 - linVel.x;
	float impulse = m_body->GetMass() * velChange;
	m_body->ApplyLinearImpulseToCenter(b2Vec2(0, impulse), true);
}



