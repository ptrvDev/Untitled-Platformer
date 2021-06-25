#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>

#include <iostream>
#include "physicalThing.h"


/*! \class StaticConvex
\brief A simple convexShape which can not move or rotate but can collide with stuff
*/

class StaticConvex : public sf::ConvexShape, public PhysicalThing
{
public:
	StaticConvex(b2World* world, const sf::Vector2f& position,b2Vec2* vertices, const int verticesCount, uint16 _categoryBits, uint16 _maskBits); //!< Complete contructor
};