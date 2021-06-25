#pragma once

#include "character.h"
#include "player.h"

class enemyAI : public Character
{
private:
	enum States {
		runState,
		jumpState,
		attackState,
		idleState,
		getHitState,
		deathState
	} enemyState; //All of the enemy states

	States currentState;
	bool isJumping = false;
	bool isDead = false;
	bool isAttacking = false;
	bool canMove = true;
	bool playerIsOnTheRight = true;

	float dist;
	float playerPosX;
	float selfPosX;

	b2World* p_World;
	sf::Vector2f enemyPosition;	
	bool dontMove = true;


public:

	bool playerAlive;

	enemyAI();
	~enemyAI();

	enemyAI(b2World* world, const sf::Vector2f position, const sf::Vector2f size, const float orientation, uint16 _categoryBits, uint16 _maskBits);

	void setNewPosition(sf::Vector2f pos);

	void update();
	void stateController();

	void swordController();
	b2Vec2 getSwordPos();
	b2Vec2 getSwordSize();
	float getSwordAngle();

	void healthManagement();
	
	void animation(States state);

	void move(float force);
	void jump();
	
	void checkPlayerPosX(Player* player);
	bool whereIsPlayer();
	
	void recieveDamage(float damage);
	void death();

	void groundContactTrue(); //!< This function is triggered when the character starts colliding with the ground
	void groundContactFalse(); //!< This function is triggered when the character stops colliding with the ground




};

