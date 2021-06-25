/*!
\file staticBlock.cpp
*/

#include "staticBlock.h"

StaticBlock::StaticBlock(b2World * world, const sf::Vector2f& position, const sf::Vector2f& size, sf::Color color, const float& orientation, const bool isGround, uint16 _categoryBits, uint16 _maskBits)
{

	b2BodyDef l_bodyDef;
	b2PolygonShape l_shape;
	b2FixtureDef l_fixtureDef;
	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.angle = orientation * DEG2RAD;
		
	m_body = world->CreateBody(&l_bodyDef);
	m_body->SetUserData(this); // used by our contact listener

	l_shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = _categoryBits;
	l_fixtureDef.filter.maskBits = _maskBits;
	


	m_body->CreateFixture(&l_fixtureDef);
	setPosition(position);
	setSize(size);
	setOrigin(size * 0.5f);
	//setOrigin(0, size.y);
	setRotation(orientation);
	setFillColor(color);
	setOutlineThickness(0.f);

}

