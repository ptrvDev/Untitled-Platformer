#include "enemyAI.h"

enemyAI::enemyAI()
{
}

enemyAI::~enemyAI()
{
}

enemyAI::enemyAI(b2World* world, const sf::Vector2f position, const sf::Vector2f size, const float orientation, uint16 _categoryBits, uint16 _maskBits)
{
	walkSpeed = 2.5f;
	health = 60.f;
	enemyState = runState;
	p_World = world;
	enemyPosition = position;

	//Box2D stuff
	b2BodyDef l_bodyDef;
	b2PolygonShape l_shape;
	b2FixtureDef l_fixtureDef;

	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.angle = orientation * DEG2RAD;
	l_bodyDef.type = b2_dynamicBody;
	l_bodyDef.fixedRotation = true;

	m_body = world->CreateBody(&l_bodyDef);

	l_shape.SetAsBox(size.x * 0.5f, size.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = 1;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = _categoryBits;
	l_fixtureDef.filter.maskBits = _maskBits;

	m_body->CreateFixture(&l_fixtureDef);

	pTex = ResourcesManager::Acquire("Assets/skeletonSpritesheet128x96.png");


	intRectSource.left = 0;
	intRectSource.top = 0;
	intRectSource.width = 128;
	intRectSource.height = 96;

	setTexture(*pTex);
	setTextureRect(intRectSource);
	setPosition(position);
	setScale(spriteScaleX, spriteScaleY);

	setOrigin(sf::Vector2f(96 * 0.5f, 128 * 0.5f));
	setRotation(orientation);




	b2BodyDef sword_bodyDef;
	b2PolygonShape sword_shape;
	b2FixtureDef sword_fixtureDef;

	swordState = 0;

	sword_bodyDef.position.Set(position.x , position.y);
	sword_bodyDef.angle = orientation * DEG2RAD;
	sword_bodyDef.type = b2_dynamicBody;
	sword_bodyDef.fixedRotation = true;

	sword_body = world->CreateBody(&sword_bodyDef);

	sword_sizeX = size.x * 0.4f;
	sword_sizeY = size.y * 0.3f;
	sword_shape.SetAsBox(sword_sizeX, sword_sizeY);
	sword_shape.m_radius = 0.0f;

	sword_fixtureDef.density = mk_fDensity;
	sword_fixtureDef.friction = 1;
	sword_fixtureDef.shape = &sword_shape;
	sword_fixtureDef.filter.categoryBits = 0x0018;
	sword_fixtureDef.filter.maskBits = 24;

	sword_body->CreateFixture(&sword_fixtureDef);



	playerAlive = true;
}

void enemyAI::setNewPosition(sf::Vector2f pos)
{

		b2Vec2 newPos;
		newPos.x = pos.x;
		newPos.y = pos.y;
		m_body->SetTransform(newPos, 0);
		dontMove = false;

}

void enemyAI::move(float force)
{
	if (canMove) {

		b2Vec2 linVel = m_body->GetLinearVelocity();
		float velChange = force - linVel.x;
		float impulse = m_body->GetMass() * velChange;
		m_body->ApplyLinearImpulseToCenter(b2Vec2( impulse, 0), true);
	 }
}

void enemyAI::jump()
{
	if (canMove) {
		isJumping = true;
		enemyState = jumpState;
		//std::cout << "enemy jumped"; 
		float impulse = m_body->GetMass() * 4.f;
		m_body->ApplyLinearImpulse(b2Vec2(0, -impulse), m_body->GetWorldCenter(), true);
	}
}

void enemyAI::checkPlayerPosX(Player* player)
{
	if (canMove) {
		playerPosX = player->getPosition().x;
		selfPosX = getPosition().x;
		dist = playerPosX - selfPosX;
		if (playerAlive) {
			if (dist > 0)
			{
				playerIsOnTheRight = true;
				//std::cout << playerIsOnTheRight << std::endl;
			}
			else if (dist < 0)
			{
				playerIsOnTheRight = false;
				//std::cout << playerIsOnTheRight << std::endl;

			}

			if (playerIsOnTheRight) {
				if (dist < 0.38f)
				{
					enemyState = attackState;
				}
			}
			else if (!playerIsOnTheRight)
			{
				if (dist > -0.38f)
				{
					enemyState = attackState;
				}
			}
		}

	}
}

bool enemyAI::whereIsPlayer()
{
	return playerIsOnTheRight;
}

void enemyAI::update()
{
	if (!dontMove) {
		if (!isDead) {
			if (!playerAlive) {
				std::cout << playerAlive;
				canMove = false;
				enemyState = idleState;
			}

			b2Vec2 pos = m_body->GetPosition();
			setPosition(pos.x, pos.y);
			swordController();
			if (playerIsOnTheRight)
			{
				move(walkSpeed);
				setScale(spriteScaleX, spriteScaleY);
			}
			else if (!playerIsOnTheRight) {
				move(-walkSpeed);
				setScale(-spriteScaleX, spriteScaleY);

			}
		}

		stateController();
		healthManagement();


	}

}

void enemyAI::swordController()
{

	if (intRectSource.top == 192) {
		if (intRectSource.left == 0) {
			swordState = 0;
		}
		else if (intRectSource.left == 128) {
			swordState = 1;
		}
		else if (intRectSource.left == 256) {
			swordState = 2;
		}
		else if (intRectSource.left == 384) {
			swordState = 3;
		}
		else if (intRectSource.left == 512) {
			swordState = 4;
		}
		else if (intRectSource.left == 640) {
			swordState = 5;
		}
	}

	float swordDiffX[6];
	float swordDiffY[6];
	float swordAngleDiff[6];

	swordDiffY[0] = 0.15f;
	swordDiffY[1] = 0.1f;
	swordDiffY[2] = -0.5f;
	swordDiffY[3] = 0.1f;
	swordDiffY[4] = 0.25f;
	swordDiffY[5] = 0.25f;




	if (playerIsOnTheRight)
	{
		swordDiffX[0] = -0.25f;
		swordDiffX[1] = -0.25f;
		swordDiffX[2] = -0.25f;
		swordDiffX[3] = 0.35f;
		swordDiffX[4] = 0.35f;
		swordDiffX[5] = 0.15f;

		swordAngleDiff[0] = 45.f;
		swordAngleDiff[1] = 45.f;
		swordAngleDiff[2] = 15.f;
		swordAngleDiff[3] = 80.f;
		swordAngleDiff[4] = 90.f;
		swordAngleDiff[5] = 90.f;
	}
	else if (!playerIsOnTheRight) 
	{
		swordDiffX[0] = 0.25f;
		swordDiffX[1] = 0.25f;
		swordDiffX[2] = 0.25f;
		swordDiffX[3] = -0.35f;
		swordDiffX[4] = -0.35f;
		swordDiffX[5] = -0.15f;

		swordAngleDiff[0] = -45.f;
		swordAngleDiff[1] = -45.f;
		swordAngleDiff[2] = -15.f;
		swordAngleDiff[3] = -80.f;
		swordAngleDiff[4] = -90.f;
		swordAngleDiff[5] = -90.f;
	}


	if (!isAttacking) {
		sword_Pos.x = m_body->GetPosition().x + swordDiffX[0];
		sword_Pos.y = m_body->GetPosition().y + 0.15f;
		sword_Angle = swordAngleDiff[0];
		sword_body->SetTransform(sword_Pos, sword_Angle);
	}
	

	if (isAttacking) {
		switch (swordState) {
		case 0:
			sword_Pos.x = m_body->GetPosition().x + swordDiffX[0];
			sword_Pos.y = m_body->GetPosition().y + 0.15f;
			sword_Angle = swordAngleDiff[0];
			sword_body->SetTransform(sword_Pos, sword_Angle);
			break;


		case 1:
			sword_Pos.x = m_body->GetPosition().x + swordDiffX[1];
			sword_Pos.y = m_body->GetPosition().y + 0.1f;
			sword_Angle = swordAngleDiff[1];
			sword_body->SetTransform(sword_Pos, sword_Angle);
			break;


		case 2:
			sword_Pos.x = m_body->GetPosition().x + swordDiffX[2];
			sword_Pos.y = m_body->GetPosition().y - 0.5f;
			sword_Angle = swordAngleDiff[2];
			sword_body->SetTransform(sword_Pos, sword_Angle);
			break;


		case 3:
			sword_Pos.x = m_body->GetPosition().x + swordDiffX[3];
			sword_Pos.y = m_body->GetPosition().y + 0.1f;
			sword_Angle = swordAngleDiff[3];
			sword_body->SetTransform(sword_Pos, sword_Angle);
			break;


		case 4:
			sword_Pos.x = m_body->GetPosition().x + swordDiffX[4];
			sword_Pos.y = m_body->GetPosition().y + 0.25f;
			sword_Angle = swordAngleDiff[4];
			sword_body->SetTransform(sword_Pos, sword_Angle);
			break;


		case 5:
			sword_Pos.x = m_body->GetPosition().x + swordDiffX[5];
			sword_Pos.y = m_body->GetPosition().y + 0.25f;
			sword_Angle = swordAngleDiff[5];
			sword_body->SetTransform(sword_Pos, sword_Angle);
			break;

		}
	}
	
}

b2Vec2 enemyAI::getSwordPos()
{
	return sword_body->GetPosition();
}

b2Vec2 enemyAI::getSwordSize()
{
	b2Vec2 sword_Size;
	sword_Size.x = sword_sizeX;
	sword_Size.y = sword_sizeY;
	return sword_Size;
}

float enemyAI::getSwordAngle()
{
	return sword_Angle;
}

void enemyAI::healthManagement()
{
	if (health <= 0) {
		isDead = true;
		canMove = false;
		m_body->SetActive(false);
		enemyState = deathState;
		std::cout << enemyState;
	}
}


void enemyAI::stateController()
{
	switch (enemyState) {

	case runState:
		animation(enemyState);
		break;

	case jumpState:
		animation(enemyState);
		break;

	case attackState:
		animation(enemyState);
		break;

	case idleState:
		animation(enemyState);
		break;

	case getHitState:
		animation(enemyState);
		break;

	case deathState:
		animation(enemyState);
		break;
	}
}

void enemyAI::animation(States state)
{
	if (state == 0) {
		walkSpeed = 2.f;
		isAttacking = false;
		if (currentState != state)
		{
			intRectSource.top = 0;
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}

		if (animClock.getElapsedTime().asSeconds() > 0.075f) {
			if (intRectSource.left == 640)
			{
				intRectSource.left = 0;
				if (dist > 1.f)
				{
					enemyState = runState;
				}
			}
			else {
				intRectSource.left += 128;
			}
			setTextureRect(intRectSource);
			animClock.restart();
		}

	}

	else if (state == 1) {
		isAttacking = false;
		if (currentState != state)
		{
			intRectSource.top = 96;
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}
		if (animClock.getElapsedTime().asSeconds() > 0.075f) {
			if (intRectSource.left == 640)
			{
				intRectSource.left = 0;
				enemyState = runState;
			}
			else {
				intRectSource.left += 128;
			}
			setTextureRect(intRectSource);
			animClock.restart();
		}
	}

	else if (state == 2) {
		isAttacking = true;
		walkSpeed = 0.1f;
		if (currentState != state)
		{
			intRectSource.top = 192;
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}
		if (animClock.getElapsedTime().asSeconds() > 0.1f) { //0.075
			if (intRectSource.left == 640)
			{
				intRectSource.left = 0;
				enemyState = runState;
			}
			else {
				intRectSource.left += 128;
			}
			setTextureRect(intRectSource);
			animClock.restart();
		}
	}

	else if (state == 3) {
		if (currentState != state)
		{
			intRectSource.top = 288;
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}
		if (animClock.getElapsedTime().asSeconds() > 0.2f) { //0.075
			if (intRectSource.left == 384)
			{
				intRectSource.left = 0;
			}
			else {
				intRectSource.left += 128;
			}
			setTextureRect(intRectSource);
			animClock.restart();
		}
	}

	else if (state == 4) {
		if (currentState != state)
		{
			intRectSource.top = 384;
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}
		if (animClock.getElapsedTime().asSeconds() > 0.075f) {
			if (intRectSource.left == 256)
			{
				intRectSource.left = 0;
				canMove = true;
				enemyState = runState;
			}
			else {
				intRectSource.left += 128;
			}
			setTextureRect(intRectSource);
			animClock.restart();
		}
	}

	else if (state == 5) {
	if (currentState != state)
	{
		intRectSource.top = 480;
		intRectSource.left = 0;
		setTextureRect(intRectSource);
		currentState = state;
	}
	if (animClock.getElapsedTime().asSeconds() > 0.2f) { //0.075
		if (intRectSource.left == 384)
		{
			intRectSource.left = 384;
		}
		else {
			intRectSource.left += 128;
		}
		setTextureRect(intRectSource);
		animClock.restart();
	}
	}
	
}

void enemyAI::groundContactTrue()
{
	isGrounded = true;
	std::cout << "enemy is grounded" << std::endl;
}

void enemyAI::groundContactFalse()
{
	isGrounded = false;
	walkSpeed = 3.5f;
	std::cout << "enemy isnt grounded" << std::endl;
}


void enemyAI::recieveDamage(float damage)
{
	if (!isDead) {
		canMove = false;
		enemyState = getHitState;
		health -= damage;
		float impulse = m_body->GetMass() * 5;
		if (playerIsOnTheRight) {
			m_body->ApplyLinearImpulse(b2Vec2(-impulse, 0), m_body->GetWorldCenter(), true);
		}
		else if (!playerIsOnTheRight) {
			m_body->ApplyLinearImpulse(b2Vec2(impulse, 0), m_body->GetWorldCenter(), true);
		}
	}
}
