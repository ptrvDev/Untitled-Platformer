#include "sensorObject.h"

sensorObject::sensorObject(b2World* _world, sf::Vector2f position, sf::Vector2f size, float orientation, DynamicBlock* _fallingPlatform, Player* _player,enemyAI* skeleton, heartCollectible* _heart, bool isConvex, b2Vec2* vertices, int verticesCount, uint16 _categoryBits, uint16 _maskBits)
{
	if (isConvex == false) {
		initRectangle(_world, position, size, orientation, _categoryBits, _maskBits );
		world = _world;
		fallingPlatform = _fallingPlatform;
		player = _player;
		enemyai = skeleton;
		heart = _heart;
		category = _categoryBits;
		mask = _maskBits;
	}
	else if (isConvex == true) {
		initConvex(_world, position, vertices, verticesCount, _categoryBits, _maskBits);
		world = _world;
		player = _player;
		enemyai = skeleton;
		category = _categoryBits;
		mask = _maskBits;
	}
}

void sensorObject::destroySensor(b2World* world)
{
	world->DestroyBody(m_body);
}

void sensorObject::update(b2Vec2 _position, float _angle)
{
	m_body->SetTransform(_position, _angle);
}

void sensorObject::onAction(b2Body* other)
{

	if (category == 6 && mask ==  1) //Ground detection for player
	{ 
		player->groundContactTrue();
	}
	else if (category == 34 && mask == 1) //Falling Dynamic platform ground detection for player
	{
		fallingPlatform->enableTimer();
		player->groundContactTrue();
	}
	else if (category == 6 && mask == 2) //Ground detection for enemy
	{
		enemyai->groundContactTrue();
	} 
	else if (category == 22 && mask == 1) //enemy sword detection for player
	{
		player->recieveDamage(15, false);
	}
	else if (category == 24 && mask == 2) //player sword detection for enemy
	{
		enemyai->recieveDamage(15);
	}


	//Spike collision detection for player
	else if (category == 18 && mask ==  1) 
	{
		player->groundContactTrue();
		player->recieveDamage(100, true);
	}

	//if the enemyAI collides with a particular groundsensor then the enemy jumps
	else if (category == 8 && mask == 2) 
	{
		enemyai->jump();
	}

	//Health collectible collision detection for player
	else if (category == 16 && mask == 1) {
		std::cout << "health ++ ";
		player->recieveHeartCollectible(30);
	}

	//Sword collectible collision detection for player
	else if (category == 32 && mask == 1) {
		std::cout << "player has collected sword";
		player->swordCollected();
	}

	//If the player falls of map he collides with an invisible wall and dies 
	else if (category == 20 && mask == 1) {
		player->recieveDamage(100, false);
	}


}

void sensorObject::offAction(b2Body* other)
{
	//the player is no grounded anymore
	if (category == 6 && mask == 1) {
		player->groundContactFalse();
	}
	//the enemy is no grounded anymore
	else if (category == 6 && mask ==  2) {
		enemyai->groundContactFalse();
	}
	else if (category == 34 && mask == 1) //Falling Dynamic platform ground detection for player
	{
		player->groundContactFalse();
	}
}
