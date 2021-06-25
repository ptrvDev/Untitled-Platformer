#include "player.h"

Player::Player() {

};

Player::Player(b2World* world, const sf::Vector2f position, const sf::Vector2f healthPos, const sf::Vector2f size, const float orientation, uint16 _categoryBits, uint16 _maskBits) {

	//Stats
	walkSpeed = 2.f;
	health = 90.f;
	fixtureSize = size;

	_world = world;
	_position = position;
	_size = size;
	_orientation = orientation;

	l_bodyDef.position.Set(position.x, position.y);
	l_bodyDef.angle = orientation * DEG2RAD;
	l_bodyDef.type = b2_dynamicBody;
	l_bodyDef.fixedRotation = true;

	m_body = world->CreateBody(&l_bodyDef);

	l_shape.SetAsBox(fixtureSize.x * 0.5f, fixtureSize.y * 0.5f);
	l_shape.m_radius = 0.0f;


	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = 1;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = _categoryBits;
	l_fixtureDef.filter.maskBits = _maskBits;


	m_body->CreateFixture(&l_fixtureDef);
	fix = m_body->GetFixtureList();
	pTex = ResourcesManager::Acquire("Assets/playerSpritesheet128x96.png");

	intRectSource.left = 0;
	intRectSource.top = 0;
	intRectSource.width = 128;
	intRectSource.height = 96;

	setTexture(*pTex);
	setTextureRect(intRectSource);
	setPosition(position);
	setScale(spriteScaleX, spriteScaleY);

	setOrigin(sf::Vector2f(96 * 0.5f, 96 * 0.5f));
	setRotation(orientation);

	pTex = ResourcesManager::Acquire("Assets/playerHearths32x80.png");
	playerHealthSpr.setTexture(*pTex);
	playerHealthSpr.setPosition(healthPos);
	playerHealthSpr.setScale(spriteScaleX * 7.5f, spriteScaleY * 7.5f);
	ui_intRectSource.left = 0;
	ui_intRectSource.top = 0;
	ui_intRectSource.width = 48;
	ui_intRectSource.height = 16;
	playerHealthSpr.setTextureRect(ui_intRectSource);



	b2BodyDef sword_bodyDef;
	b2PolygonShape sword_shape;
	b2FixtureDef sword_fixtureDef;

	swordState = 0;

	sword_bodyDef.position.Set(_position.x, _position.y);
	sword_bodyDef.angle = _orientation * DEG2RAD;
	sword_bodyDef.type = b2_dynamicBody;
	sword_bodyDef.fixedRotation = true;

	sword_body = _world->CreateBody(&sword_bodyDef);
	sword_body->SetActive(false);

	sword_sizeX = _size.x * 0.4f;
	sword_sizeY = _size.y * 0.3f;
	sword_shape.SetAsBox(sword_sizeX, sword_sizeY);
	sword_shape.m_radius = 0.0f;

	sword_fixtureDef.density = mk_fDensity;
	sword_fixtureDef.friction = 1;
	sword_fixtureDef.shape = &sword_shape;
	sword_fixtureDef.filter.categoryBits = 0x0018;
	sword_fixtureDef.filter.maskBits = 24;

	sword_body->CreateFixture(&sword_fixtureDef);


}

Player::~Player() {

}

void Player::processKeyPress()
{
	if (canMove) {
		//Move to the left
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
		{
			//without the next "if statement" the movement and animation is buggy - it disables moving left and right simultaniously
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			{
				move(0);
				playerState = idleState;
				isMoving = false;
			}
			else {
				facingRight = false;
				isMoving = true;
				move(-walkSpeed);
				setScale(-spriteScaleX, spriteScaleY); //flip character sprite

				//without next "if statement" there is a bug where if the player presses A or D while mid air, the jumping animation freezes 
				if (isGrounded && !isAttacking)
				{
					playerState = runState;
				}
			}
		}

		//Move to the right
		else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
		{
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			{
				isMoving = false;
				move(0);
				playerState = idleState;
			}
			else {
				isMoving = true;
				facingRight = true;
				move(walkSpeed);
				setScale(spriteScaleX, spriteScaleY);

				if (isGrounded && !isAttacking)
				{
					playerState = runState;
				}
			}
		}

		else {
			if (isJumping == false && isAttacking == false) {
				playerState = idleState;
			}
			isMoving = false;
		}

		//Jump
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
		{
			if (!isAttacking) {
				jump();
				isJumping = true;
			}

			//playerState = jumpState;
		}

		if (sf::Keyboard::isKeyPressed(sf::Keyboard::C)) 
		{
			if (canAttack && !isRolling && !isJumping) {
				playerState = attackState;
			}
			std::cout << "ATTACKING";
		}

		//Roll
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::X))
		{
			if (!isAttacking && !isJumping) {
				playerState = rollState;
				roll();
			}
		}
	}
}

void Player::update() {
	b2Vec2 pos = m_body->GetPosition();
	b2Vec2 vel = m_body->GetLinearVelocity();
	setPosition(pos.x, pos.y);

	processKeyPress();

	stateController();
	swordController();

	healthManagement();
	jumpTweak();
}



void Player::stateController()
{
	switch (playerState) {
	case idleState:
		animation(playerState);
		break;

	case runState:
		if (isGrounded)
		{
			animation(playerState);
		}
		break;
	case rollState:
		if (isGrounded) {
			animation(rollState);
			break;
		}
		
	case jumpState:
		animation(playerState);
		break;

	case deathState:
		animation(playerState);
		break;

	case attackState:
		animation(playerState);
		break;

	case getHitState:
		animation(playerState);
		break;

	}
}

void Player::swordCollected()
{
	canAttack = true;
	hasSword = true;
}

bool Player::isSwordCollected()
{
	return canAttack;
}

void Player::swordController()
{
	if (canAttack) {
		if (intRectSource.top == 480) {

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


		if (!isAttacking) {
			//sword_body->SetActive(true);
			sword_Pos.x = m_body->GetPosition().x;
			sword_Pos.y = m_body->GetPosition().y;
			sword_Angle = 0.f;
			sword_body->SetTransform(sword_Pos, sword_Angle);
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

		if (facingRight)
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
		else if (!facingRight) {
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
				sword_Pos.y = m_body->GetPosition().y + 0.1f;
				sword_Angle = swordAngleDiff[3];
				sword_body->SetTransform(sword_Pos, sword_Angle);
				break;
			case 5:
				sword_Pos.x = m_body->GetPosition().x + swordDiffX[5];
				sword_Pos.y = m_body->GetPosition().y + 0.1f;
				sword_Angle = swordAngleDiff[3];
				sword_body->SetTransform(sword_Pos, sword_Angle);
				break;
			}
		}
		else {
			//This is the quickes bugfix I could think of
			sword_Pos.y = 1000;
		}
	}

}

b2Vec2 Player::getSwordPos()
{
	return sword_body->GetPosition();
}

b2Vec2 Player::getSwordSize()
{
	b2Vec2 sword_Size;
	sword_Size.x = sword_sizeX;
	sword_Size.y = sword_sizeY;
	return sword_Size;
}

float Player::getSwordAngle()
{
	return sword_Angle;
}


void Player::animation(States state)
{
	if (state == 0)
	{
		if (currentState != state)
		{
			if (hasSword) {
				intRectSource.top = 96;
			}
			else {
				intRectSource.top = 0;
			}
			
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}
		
		if (animClock.getElapsedTime().asSeconds() > 0.5f) {
			if (intRectSource.left == 640)
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

	if (state == 1)
	{

		if (currentState != state)
		{
			if (!hasSword) {
				intRectSource.top = 192;
			}
			if (hasSword) {
				intRectSource.top = 960;
			}
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}

		if (animClock.getElapsedTime().asSeconds() > 0.06f) {
			if (intRectSource.left == 1408)
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

	if (state == 2)
	{

		if (currentState != state)
		{
			if (!hasSword) {
				intRectSource.top = 384;
			}
			else if (hasSword) {
				intRectSource.top = 1152;
			}


			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}

		if (animClock.getElapsedTime().asSeconds() > 0.075f) {
			if (intRectSource.left == 1664)
			{
				intRectSource.left = 1664;
			}
			else {
				intRectSource.left += 128;
			}
			setTextureRect(intRectSource);
			animClock.restart();
		}
	}

	if (state == 3) {

		if (currentState != state)
		{
			if (!hasSword) {
				intRectSource.top = 288;
			}
		else if (hasSword) {
			intRectSource.top = 1056;
		}
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}
		if (animClock.getElapsedTime().asSeconds() > 0.075f) {
			if (intRectSource.left == 1664)
			{
				canMove = true;
				isRolling = false;
				intRectSource.left = 0;
				playerState = idleState;
			}

			else {
				intRectSource.left += 128;
				if (intRectSource.left == 1280)
				{
					originalFixture();
				}
			}

			setTextureRect(intRectSource);
			animClock.restart();
		}
	}

	if (state == 4) {
		if (currentState != state)
		{
			if (!hasSword) {
				intRectSource.top = 864;
			}
			else if (hasSword) {
				intRectSource.width = 150;
				intRectSource.top = 1248;
			}
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}
		if (animClock.getElapsedTime().asSeconds() > 0.05f) {
			if (!hasSword)
			{
				if (intRectSource.left == 640)
				{
					intRectSource.left = 640;
				}
				else {
					intRectSource.left += 128;
				}
			} 
			else if (hasSword) {
				if (intRectSource.left == 780)
				{
					intRectSource.left = 780;
				}
				else {
					intRectSource.left += 156;
				}
			}
	
			setTextureRect(intRectSource);
			animClock.restart();
		}
	}

	if (state == 5) {
		isAttacking = true;
		if (currentState != state)
		{
			intRectSource.top = 480;
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}
		if (animClock.getElapsedTime().asSeconds() > 0.1f) {

			if (intRectSource.left == 768) {
				intRectSource.left = 0;
				isAttacking = false;
			}
			else {
				intRectSource.left += 128;
			}
			setTextureRect(intRectSource);
			animClock.restart();
		}
	}

	if (state == 6) {
		if (currentState != state)
		{
			if (!hasSword) {
				intRectSource.top = 672;
			}
			else if (hasSword) {
				intRectSource.top = 768;
			}
			intRectSource.left = 0;
			setTextureRect(intRectSource);
			currentState = state;
		}
		
		if (animClock.getElapsedTime().asSeconds() > 0.075f) {

			if (intRectSource.left == 384) {
				intRectSource.left = 384;
				canMove = true;
			}
			else {
				intRectSource.left += 128;
			}
			setTextureRect(intRectSource);
			animClock.restart();
		}
	}
			
}

void Player::move(float force) {

	b2Vec2 linVel = m_body->GetLinearVelocity();
	float velChange = force - linVel.x;
	float impulse = m_body->GetMass() * velChange;
	m_body->ApplyLinearImpulseToCenter(b2Vec2(impulse, 0), true);


}

void Player::jump() {

	b2Vec2 vec = m_body->GetLinearVelocity();

	if (isGrounded == true) {
		float impulse = m_body->GetMass() * 2;
		m_body->ApplyLinearImpulse(b2Vec2(0, -impulse), m_body->GetWorldCenter(), true);
	}
}

void Player::jumpTweak()
{
	if (isGrounded == false && isRolling ==false) {
		isJumping = true;
	}
	else {
		isJumping = false;
	}

	if (isJumping) {
		playerState = jumpState;
	}

	if (isJumping == true && isMoving == false) {

		b2Vec2 vel;
		vel.x = 0;
		vel.y = m_body->GetLinearVelocity().y;
		m_body->SetLinearVelocity(vel);
	}
}

void Player::roll()
{
	rollFixture();
	canMove = false;
	isRolling = true;
	float impulse = m_body->GetMass() * 2;
	if (facingRight)
	{
		m_body->ApplyLinearImpulse(b2Vec2(impulse * 2.15f, 0.f), m_body->GetWorldCenter(), true);
	}
	else {
		m_body->ApplyLinearImpulse(b2Vec2(-impulse * 2.15f, 0.f), m_body->GetWorldCenter(), true);
	}
}

void Player::rollFixture()
{

	setOrigin(sf::Vector2f(128 * 0.5f, 128 * 0.5f));
	m_body->DestroyFixture(fix);
	l_shape.SetAsBox(0.2 * 0.5f, 0.2 * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = 1;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = 1;
	l_fixtureDef.filter.maskBits = 2 | 4 | 6 | 16 | 18 | 20 | 22 | 32 | 34;

	m_body->CreateFixture(&l_fixtureDef);
}

void Player::originalFixture()
{

	setOrigin(sf::Vector2f(96 * 0.5f, 96 * 0.5f));
	m_body->DestroyFixture(fix);
	l_shape.SetAsBox(fixtureSize.x * 0.5f, fixtureSize.y * 0.5f);
	l_shape.m_radius = 0.0f;

	l_fixtureDef.density = mk_fDensity;
	l_fixtureDef.friction = 1;
	l_fixtureDef.shape = &l_shape;
	l_fixtureDef.filter.categoryBits = 1;
	l_fixtureDef.filter.maskBits = 2 | 4 | 6 | 16 | 18 | 20 | 22 | 32 | 34;

	m_body->CreateFixture(&l_fixtureDef);
}


void Player::groundContactTrue()
{
	//std::cout << "ground contact TRUE" << std::endl;
	isGrounded = true;
}

void Player::groundContactFalse()
{
	//std::cout << "ground contact FALSE" << std::endl;
	isGrounded = false;
}

sf::Sprite Player::getHealthSpr()
{
	return playerHealthSpr;
}

void Player::healthManagement()
{

	if (health > 90) {
		health = 90;
	}
	else if (health == 90) {

		ui_intRectSource.top = 0;
		playerHealthSpr.setTextureRect(ui_intRectSource);
	}
	else if (health == 75) {
		ui_intRectSource.top = 16;
		playerHealthSpr.setTextureRect(ui_intRectSource);
	}
	else if (health == 60) {
		ui_intRectSource.top = 32;
		playerHealthSpr.setTextureRect(ui_intRectSource);
	}
	else if (health == 45) {
		ui_intRectSource.top = 48;
		playerHealthSpr.setTextureRect(ui_intRectSource);
	}
	else if (health == 30) {
		ui_intRectSource.top = 64;
		playerHealthSpr.setTextureRect(ui_intRectSource);
	}
	else if (health == 15) {
		ui_intRectSource.top = 80;
		playerHealthSpr.setTextureRect(ui_intRectSource);
	}
	else if (health == 0) {
		ui_intRectSource.top = 96;
		playerHealthSpr.setTextureRect(ui_intRectSource);
		l_fixtureDef.filter.categoryBits = 4;
		death();
	}
	else if (health < 0) {
		health = 0;
	}
}

void Player::recieveDamage(float damage, bool damageFromSpike)
{
	health -= damage;
	canMove = false;
	if (damageFromSpike) {
		playerState = deathState;
	} if (!damageFromSpike) {
		playerState = getHitState;
		float impulse = m_body->GetMass() * 5;
		if (enemyIsOnTheRight) {
			m_body->ApplyLinearImpulse(b2Vec2(impulse * 0.75f, 0), m_body->GetWorldCenter(), true);
		}else if (!enemyIsOnTheRight) {
			m_body->ApplyLinearImpulse(b2Vec2(-impulse * 0.75f, 0), m_body->GetWorldCenter(), true);
		}
	}


}

void Player::recieveHeartCollectible(float hp)
{
	std::cout << "player health ++";
	health += hp;
}

void Player::whereIsEnemy(bool enemyIsOnTheLeft) {

	enemyIsOnTheRight = -enemyIsOnTheLeft;
}
void Player::death()
{
	canMove = false;
	playerState = deathState;
}

