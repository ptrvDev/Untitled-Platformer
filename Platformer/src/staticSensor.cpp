/*!
\file staticSensor.cpp
*/

#include "staticSensor.h"

void StaticSensor::initRectangle(b2World * world, sf::Vector2f position, sf::Vector2f size, float orientation, uint16 _categoryBits, uint16 _maskBits)
{
	b2BodyDef l_bodyDef;
	b2PolygonShape l_shape;
	b2FixtureDef l_fixtureDef;

	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.angle = orientation * DEG2RAD;

	m_body = world->CreateBody(&l_bodyDef);

	l_shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = _categoryBits;
	l_fixtureDef.filter.maskBits = _maskBits;

	l_fixtureDef.isSensor = true; //Make it a sensor

	m_body->CreateFixture(&l_fixtureDef);

	setPosition(position);
	setSize(size);
	setOrigin(size * 0.5f);
	setRotation(orientation);
}

void StaticSensor::initConvex(b2World* world, sf::Vector2f& position, b2Vec2* vertices, int verticesCount, uint16 _categoryBits, uint16 _maskBits)
{
	b2BodyDef l_bodyDef;
	b2PolygonShape l_shape;
	b2FixtureDef l_fixtureDef;

	l_bodyDef.position.Set(position.x, position.y);

	m_body = world->CreateBody(&l_bodyDef);

	l_shape.Set(vertices, verticesCount);

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = _categoryBits;
	l_fixtureDef.filter.maskBits = _maskBits;

	l_fixtureDef.isSensor = true; //Make it a sensor

	m_body->CreateFixture(&l_fixtureDef);

	setPosition(position);
	setOutlineThickness(0.f);
	setFillColor(sf::Color::Blue);
}

void StaticSensor::initCircle(b2World * world, sf::Vector2f position, float radius, uint16 _categoryBits, uint16 _maskBits)
{
	b2BodyDef l_bodyDef;
	b2CircleShape l_shape;
	b2FixtureDef l_fixtureDef;

	l_bodyDef.position.Set(position.x, position.y);

	m_body = world->CreateBody(&l_bodyDef);

	l_shape.m_radius = radius;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = _categoryBits;
	l_fixtureDef.filter.maskBits = _maskBits;

	l_fixtureDef.isSensor = true; //Make it a sensor

	m_body->CreateFixture(&l_fixtureDef);

	setPosition(position);
	setSize(sf::Vector2f(radius * 2.0f, radius * 2.0f));
	setOrigin(getSize() * 0.5f);
}
