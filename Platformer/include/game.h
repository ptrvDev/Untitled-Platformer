#pragma once

/*!
\file game.h
*/

#include <iostream>

#include <Box2D/Box2D.h>
#include <SFML/Graphics.hpp>
#include <map>


#include "player.h"
#include "enemyAI.h"

#include "staticBlock.h"
#include "staticConvex.h"
#include "dynamicBlock.h"

#include "sensorObject.h"
#include "SensorContactListener.h"

#include "heartCollectible.h"
#include "swordCollectible.h"

#include "ResourcesManager.h"
#include "graphics.h"


#include "SFMLDebugDraw.h"

#define _USE_MATH_DEFINES
#include <math.h>

/*! \class Game
\brief Holds all information about the game, blocks, balls etc and allows updating and rendering.
*/

class Game
{

private:

	sf::RenderWindow* window; //!<The render window
	float fFrameTime = 1.f / 60.f;
	sf::Clock clock;

	sf::View m_view, ui_view; //!< The view maps from physical co-ordinates to rendering co-ordinat
	sf::Vector2f viewCoordinates = sf::Vector2f(6.4, 3.6); //resolution should be 640x360
	sf::Vector2f m_worldSize = sf::Vector2f(41.42f, 8.17f); //!< Size of the physical worlds is 8 X 6 metres


	b2World* m_pWorld = nullptr; //!< Pointer to the Box2D world.  Using pointers as BOX2D has it's own memory management
	const int mk_iVelIterations = 7; //!< On each update there will be 7 velocity iterations in the physics engine
	const int mk_iPosIterations = 5; //!< On each update there will be 5 position iterations on each update
	const b2Vec2 mk_gravity = b2Vec2(0.f, 9.81f); //!< gravity


	enum _entityCategory {
		CHARACTER = 0x0001,
		ENEMY = 0x0002,
		STATICBLOCK = 0x0004,
		GROUNDDETECTION = 0X006,
		JUMPSENSOR = 0x008,
		HEART = 0x0010,
		SPIKES = 0X0012,
		FALLOFFMAP = 0x0014,
		ENEMYSWORD = 0x00016,
		PLAYERSWORD = 0x00018,
		SWORDCOLLECTIBLE =  0x00020,
		DYNAMICBLOCK = 0x00022
	}; //!< used for collision filtering



	std::map<int, StaticBlock> m_staticBlocks; //!< A collection of static square shapes. 
	std::map<int, DynamicBlock> m_dynamicBlocks; //!< A collection of dynamic square shapes. 
	std::map<int, StaticConvex> m_staticConvexShapes; //!< A collection of static convex shapes. 
	std::map<int, sensorObject> m_sensorObjects; //!< A collection of sensors
	std::map<int, sensorObject> m_enemyWeapons; //!< A collection of sensors  (only for enemy weapons)
	std::map<int, heartCollectible> heartCollectibles; //!< A collection of collectibles

	swordCollectible sword;//!< Sword object that the player can collect in order to be able to attack the enemies

	bool heartCollected[3]; //!< Every boolean corresponds to a heartCollectibles object. If its collected its true, else it's false
	bool swordCollected;
	bool playerHasSword = false;
	bool playerIsDead = false;


	SensorContactListener m_listener; //!< Contact listener for collisions

	Player player; //!< The playable character

	enemyAI skeleton[2]; //!< A collection of enemyAI objects
	sf::Sprite character_health;//!<Characters health bar


	graphics* background; //!< Layer 1 Image 
	graphics* playerLayer; //!< Layer 2 Image
	graphics* foreground; //!< Layer 3 Image


	bool m_debug = false; //!< Toggle for debug drawing
	SFMLDebugDraw m_debugDraw; //!< Box2D debug drawing
	void toggleDebug() { m_debug = !m_debug; } //!< Toggle for debug drawing

	bool fallingPlatformsDeleted = false;
	bool enemyPosSet[2];
	bool lookAtEnemy0 = true;

public:


	Game(); //!< Constructor which sets up the game
	~Game(); //!< Destructor which cleans all the pointer memory up

	void RunGame(); //!< Main game loop
	void CheckLose(); //!< Check lose condition
	void CheckWin(); //!< Check win condition

	void update(float timestep); //!< Update the game with give timestep
	void draw(); //!< Draw the game to the render context

	void instantiateObjects(); //!< Instantiates all of the static/dynamic blocks, sensors, characters and collectibles
	void setEnemyNewPos();
	void checkCollectiblesState(); //!< Check whether or not the collectibles have been collected by the player
	void checkFallingPlatformState();


};
