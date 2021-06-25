#pragma once
#include <SFML/Graphics.hpp>
#include <Box2D/Box2D.h>

#include <memory>
#include <iostream>

#include "physicalThing.h"
#include "ResourcesManager.h"





class Character : public sf::Sprite, public PhysicalThing
{

protected:

	float walkSpeed; //!< walk speed value
	float health; //!< health value

	std::shared_ptr<sf::Texture> pTex; //!< A shared pointer to the texture used for the character
	sf::IntRect intRectSource; //!< The internal rectangle(tile) of the whole PNG image (used for animation)
	sf::Clock animClock; //!< Clock used for the animation to switch to the next tile 

	float spriteScaleX = 0.01; //!< the value used to scale down the X coordinate of the PNG
	float spriteScaleY = 0.01; //!< the value used to scale down the Y coordinate of the PNG

	bool isGrounded; //!< bool which tells whether or not the player is colliding with the ground

	b2Body* sword_body; //!< Box2D body of the sword
	int swordState; //!< State of the sword. The state consists of position and angle
	b2Vec2 sword_Pos; //!< Position of the sword
	float sword_sizeX; //!< Size of the sword (X dimension)
	float sword_sizeY; //!< Size of the sword (Y dimension)
	float sword_Angle; //!< Angle of the sword



public:

	virtual void update() = 0; //!< Update function
	virtual void stateController() = 0; //!< Used to switch between the different states
	virtual void move(float force) = 0; //!< Function used to move the character left and right
	virtual void jump() = 0; //!< Function used to make the character jump
	virtual void swordController() = 0; //!< Function that controls the states (position and angle) of the weapons of the player (or the AI)
	virtual b2Vec2 getSwordPos() = 0; //!< Returns the value of the swords position (could be useful in game.cpp for adjusting the swords sensor position)
	virtual b2Vec2 getSwordSize() = 0; //!< Returns the value of the swords size  (could be useful in game.cpp for adjusting the swords sensor size)
	virtual float getSwordAngle() = 0; //!< Returns the value of the swords angle (could be useful in game.cpp for adjusting the swords sensor angle)
	virtual void groundContactTrue() = 0; //!< This function is triggered when the character starts colliding with the ground
	virtual void groundContactFalse() = 0; //!< This function is triggered when the character stops colliding with the ground



};
