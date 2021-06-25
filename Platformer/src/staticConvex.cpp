#include "staticConvex.h"

StaticConvex::StaticConvex(b2World* world, const sf::Vector2f& position, b2Vec2* vertices, const int verticesCount, uint16 _categoryBits, uint16 _maskBits)
{
	b2BodyDef l_bodyDef;
	b2PolygonShape l_shape;
	b2FixtureDef l_fixtureDef;

	l_bodyDef.position.Set(position.x, position.y);

	m_body = world->CreateBody(&l_bodyDef);


	l_shape.Set(vertices, verticesCount);

	for (int i = 0; i < verticesCount; i++) {
		
		float tempX = vertices[i].x;
		float tempY = vertices[i].y;
		std::cout << "vertice number " << i << " X coordinate is: " << tempX << " Y coordinate is: " << tempY << std::endl;
		setPointCount(verticesCount);
		setPoint(i, sf::Vector2f(tempX, tempY));
	}

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = mk_fFriction;
	l_fixtureDef.restitution = mk_fRestitution;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = _categoryBits;
	l_fixtureDef.filter.maskBits = _maskBits;

	m_body->CreateFixture(&l_fixtureDef);

	setPosition(position);
	setOutlineThickness(0.f);
	setFillColor(sf::Color::Blue);

}
