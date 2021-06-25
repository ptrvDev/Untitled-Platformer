#pragma once


#include "character.h"

class Player : public Character
{
private:
	
	//Box2D stuff
	b2Fixture* fix; //!<box2D fixture  used in couple functions
	b2BodyDef l_bodyDef; //!<BodyDef is used only in the complete constructor but since all of the other box2D variables are here, I decided to put the body definition here as well
	b2PolygonShape l_shape; //!< polygon shape of the body, variable sused for resizing the body
	b2FixtureDef l_fixtureDef; //!<fixture definition, variable used for resizing the body
	sf::Vector2f fixtureSize; //!< the size of the body depending if its rolling or not

	enum States
	{
		idleState,
		runState,
		jumpState,
		rollState,
		deathState,
		attackState,
		getHitState
	} playerState; //!< All of the player states
	States currentState; //!< The current state of the character

	sf::Texture playerHealthTexture; //!< Players health texture displayed in top left corner
	sf::Sprite playerHealthSpr; //!< Sprite for the health texture 
	sf::IntRect ui_intRectSource; //!< The internal rectangle(tile) of the whole PNG image (used for animation)

	bool canMove = true; //!< If it's true, the player can move/jump/roll
	bool canAttack = false; //!< Player can't attack until he collects the sword
	bool hasSword = false; //!< This bool is used for the animator (I am not using the canAttack bool because it is set to false after the player dies and the death animation is glitched)
	bool facingRight = true; //!< indicates which position the player is facing
	bool isJumping = false;  //!< Is the player jumping?
	bool isMoving = false; //!< Is the player moving?
	bool isRolling = false; //!< Is the player doing a roll?
	bool isAttacking = false; //!< Is the player attacking?
	bool enemyIsOnTheRight; //!< Indicates whether the enemy skeleton is on the right or left side of the player
	sf::Clock rollClock; //!< When the clock runs out of time, the player is no longer in roll state

	b2World* _world;
	sf::Vector2f _position;
	sf::Vector2f _size;
	float _orientation;


public:

	Player(); //!< Default constructor
	Player(b2World* world, const sf::Vector2f position, const sf::Vector2f healthPos, const sf::Vector2f size, const float orientation, uint16 _categoryBits, uint16 _maskBits);//!< Complete contructor
	~Player(); //!< Default destructor

	void processKeyPress(); //!< Process user input
	void update(); //!< Update function
	void stateController(); //!< Used to switch between the different states
	
	void animation(States state); //!< Animation function which depends on the state of the character

	void move(float force); //!< Function used to move the character left and right
	void jump(); //!< Function used to make the character jump
	void jumpTweak(); //!< Function used to fix some bugs in the jump() function which is run in the update() function 
	void roll(); //!< Function used to make the character roll
	void rollFixture(); //!< resizing the fixture of the character to a tiny square  
	void originalFixture(); //!< resetting fixture after the roll function

	void swordCollected();
	bool isSwordCollected();
	void swordController();
	b2Vec2 getSwordPos();
	b2Vec2 getSwordSize();
	float getSwordAngle();

	void groundContactTrue(); //!< This function is triggered when the character starts colliding with the ground
	void groundContactFalse(); //!< This function is triggered when the character stops colliding with the ground

	sf::Sprite getHealthSpr(); //!< Function used to acquire the players health sprite in order to update it from the game.cpp
	void healthManagement(); //!< Function which controls the healthbar and is checking whether or not the player is alive

	void recieveDamage(float damage, bool damageFromSpike); //!< this function is called when the player is hit by and enemy or a hazard
	void recieveHeartCollectible(float hp); //!< this function is called when the player 

	void whereIsEnemy(bool enemyIsOnTheLeft);

	void death(); //!< This function is called when the player runs out of health points

};

