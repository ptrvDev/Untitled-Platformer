#pragma once

#include "staticSensor.h"
#include "player.h"
#include "enemyAI.h"
#include "heartCollectible.h"
#include "staticBlock.h"
#include "dynamicBlock.h"
class sensorObject : public StaticSensor
{
private:

	uint16 category; //!< used for collision filtering
	uint16 mask; //!< used for collision filtering

	Player* player; //!< pointer to the player object
	enemyAI* enemyai; //!< pointer to the enemy object
	heartCollectible* heart;
	b2World* world;
	DynamicBlock* fallingPlatform;

public:
	
	sensorObject() {}; //!< Default constructor
	sensorObject(b2World* _world, sf::Vector2f position, sf::Vector2f size, float orientation, DynamicBlock* _fallingPlatform, Player* _player,enemyAI* skeleton,heartCollectible* _heart, bool isConvex, b2Vec2* vertices, int verticesCount, uint16 _categoryBits, uint16 _maskBits);//<! Complete constructor
	void destroySensor(b2World* world);
	void update(b2Vec2 _position, float _angle);
	void onAction(b2Body* other); //!< when objectA starts colliding with object B this function is run
	void offAction(b2Body* other);//!< when objectA stops colliding with object B this function is run


};

