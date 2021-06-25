/*!
\file game.cpp
*/
#include "game.h"

Game::Game()
{
	window = new sf::RenderWindow(sf::VideoMode(1024, 576), "2D Platformer Game"); //2048x1152
	
	window->setFramerateLimit(60);
	m_view.setCenter(0.f, 0.f);
	m_view.setSize(viewCoordinates);
	m_view.setViewport(sf::FloatRect(0.f, 0.f, 1.f, 1.f));

	ui_view.setCenter(0.f, 0.f);
	ui_view.setSize(viewCoordinates);
	ui_view.setViewport(sf::FloatRect(0.f, 0.f, 0.25f, 0.25f));


	m_pWorld = new b2World(mk_gravity);
	m_debugDraw.setWorld(m_pWorld);

	instantiateObjects();

	m_pWorld->SetContactListener(&m_listener);

	heartCollected[0] = false;
	heartCollected[1] = false;
	heartCollected[2] = false;

	swordCollected = false;

	enemyPosSet[0] = false;
	enemyPosSet[1] = false;

	background = new graphics(sf::Vector2f(15.f, -2.f), 1);
	playerLayer = new graphics(sf::Vector2f(15.f, -2.f), 2);
	//foreground = new graphics(sf::Vector2f(0.f, 0.f), 3);
}



Game::~Game()
{
	// Clean up all pointers
	delete m_pWorld;
	m_pWorld = nullptr;		

	delete background;
	background = nullptr;
	delete playerLayer;
	playerLayer = nullptr;
	//delete foreground;
	//foreground = nullptr;
}



void Game::RunGame()
{
	while (window->isOpen())
	{
		sf::Event event;

		while (window->pollEvent(event))
		{
			sf::Vector2i mouse = sf::Mouse::getPosition(*window);
			sf::Vector2f mouse_world = window->mapPixelToCoords(mouse);
			float x1 = mouse_world.x;
			float y1 = mouse_world.y;

			float x2 = mouse.x;
			float y2 = mouse.y;

			switch (event.type)
			{

			case sf::Event::Closed:
				window->close();
				break;


			case sf::Event::KeyPressed:
				//
				break;


			case sf::Event::MouseButtonPressed:
				if (event.mouseButton.button == sf::Mouse::Left)
				{
					std::cout << "MapPixelToCoords X: " << x1 << std::endl;
					std::cout << "MapPixelToCoords Y: " << y1 << std::endl;

					std::cout << "The png's X: " << x2 << std::endl;
					std::cout << "The png's Y: " << y2 << std::endl;
				}
				break;


			default:
				break;
			}
		}

		float m_fElapsedTime = clock.getElapsedTime().asSeconds();
		// If a frame has past the update the physics
		if (m_fElapsedTime > fFrameTime)
		{
			update(m_fElapsedTime);
			clock.restart();
		}

		window->clear(sf::Color::White);
		draw();
		window->display();
	}
}



void Game::CheckLose()
{
	if (character_health.getTextureRect().top == 96) {

		playerIsDead = true;
	}
}



void Game::CheckWin()
{
}



void Game::update(float timestep)
{
	
	
	// Update the world
	m_pWorld->Step(timestep, mk_iVelIterations, mk_iVelIterations);
	sf::Vector2f playerMovement = player.getPosition();
	if (!playerIsDead)
	{
		m_view.setCenter(sf::Vector2f(playerMovement));
	}
	else if (playerIsDead) {
		skeleton[0].playerAlive = false;
		skeleton[1].playerAlive = false;

	}


	//Update the player and his health
	player.update();
	if (lookAtEnemy0) {
		player.whereIsEnemy(skeleton[0].whereIsPlayer());
	}
	else {
		player.whereIsEnemy(skeleton[1].whereIsPlayer());
	}
	character_health = player.getHealthSpr();

	//Update the enemies
	skeleton[0].update();
	skeleton[0].checkPlayerPosX(&player);

	skeleton[1].update();
	skeleton[1].checkPlayerPosX(&player);

	//Update the weapon sensor positions
	if (playerHasSword) {
		m_sensorObjects.at(0).update(player.getSwordPos(), player.getSwordAngle());
	}
	m_sensorObjects.at(1).update(skeleton[0].getSwordPos(), skeleton[0].getSwordAngle());
	m_sensorObjects.at(2).update(skeleton[1].getSwordPos(), skeleton[1].getSwordAngle());

	//Update the heart collectibles
	for (int i = 0; i != heartCollectibles.size(); i++) {
		heartCollectibles.at(i).update();
	}

	//update the falling platform sensors positions (since the falling patform objects position change) 
	for (int i = 0; i != m_dynamicBlocks.size(); i++)
	{
		m_dynamicBlocks.at(i).update();
	}

	CheckLose();
	checkCollectiblesState();
	checkFallingPlatformState();
	setEnemyNewPos();
	if (!fallingPlatformsDeleted) {
		m_sensorObjects.at(19).update(m_dynamicBlocks.at(0).getPos(), 0);
		m_sensorObjects.at(20).update(m_dynamicBlocks.at(1).getPos(), 0);
	}

	// Delete debug shapes
	if (m_debug) m_debugDraw.clear();
	//toggle debug
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Tab))
	{
		toggleDebug();
	}
}



void Game::draw()
{
	//Set the view
	window->setView(m_view);	

	window->draw(*background);
	window->draw(*playerLayer);


	/* Uncomment in order to check the shapes
	for (int i = 0; i != m_staticBlocks.size(); i++)
	{
		StaticBlock block = m_staticBlocks.at(i);
		window->draw(block);
	}

	for (int i = 0; i != m_staticConvexShapes.size(); i++) {
		StaticConvex convex = m_staticConvexShapes.at(i);
		window->draw(convex);
	}*/

	for (int i = 0; i != m_dynamicBlocks.size(); i++)
	{
		DynamicBlock block = m_dynamicBlocks.at(i);
		window->draw(block);
	}

	// draw all heart collectibles
	for (int i = 0; i != heartCollectibles.size(); i++) {
		heartCollectible heartCollectible = heartCollectibles.at(i);
		window->draw(heartCollectible);
	}

	if (!swordCollected) {
		window->draw(sword);
	}

	window->draw(player);
	window->draw(skeleton[0]);
	window->draw(skeleton[1]);
	// Debug Draw
	if (m_debug) window->draw(m_debugDraw);

	//////////////////////////////////////////////////
	//                                              //
	//ONLY DRAW FOR THE UI VIEW BELLOW THIS COMMENT //
	//                                              //
	//////////////////////////////////////////////////

	window->setView(ui_view);
	window->draw(character_health);



}



void Game::instantiateObjects()
{	
	//Player object
	float posX = -1.;
	float posY = -5.f;
	float sizeX = 0.2f;
	float sizeY = 0.6f;
	sf::Color colorForBlock;
	sf::Vector2f topLeftOfWindow = window->mapPixelToCoords(sf::Vector2i(-2, -1));
	player = Player(m_pWorld, sf::Vector2f(posX, posY), topLeftOfWindow, sf::Vector2f(sizeX, sizeY), 0.0f, CHARACTER, ENEMY | STATICBLOCK | GROUNDDETECTION | HEART | SPIKES | ENEMYSWORD | SWORDCOLLECTIBLE | DYNAMICBLOCK);
	
	//Skeleton 0 object
	posX = 31;
	posY =  -6.f;
	sizeX = 0.1f;
	sizeY = 0.6f;
	skeleton[0] = enemyAI(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.0f, ENEMY, CHARACTER | PLAYERSWORD | STATICBLOCK | JUMPSENSOR | GROUNDDETECTION | HEART);

	//Skeleton 1 object
	posX = 30.;
	posY = -6.f;
	sizeX = 0.1f;
	sizeY = 0.6f;
	skeleton[1] = enemyAI(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.0f, ENEMY, CHARACTER | PLAYERSWORD | STATICBLOCK | JUMPSENSOR | GROUNDDETECTION | HEART);

	


    /////////////////////////////////////////////
    ////                                     ////
    ////                                     ////
    ////        staticBlock Objects          ////
    ////                                     ////
    ////                                     ////
    /////////////////////////////////////////////	  

	//Wall block 0 (forgot to add this)
	posX = -1.85f;
	posY = -0.05f;
	sizeX = 0.2;
	sizeY = 5.f;
	colorForBlock = sf::Color::Cyan;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(0, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

    //Ground block 0
	posX = -1.6f;
	posY = -0.05f;
	sizeX = 4.6;
	sizeY = 0.2f;
	colorForBlock = sf::Color::Green;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(1, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Wall block 1
	posX = 0.6f;
	posY = 0.75f;
	sizeX = 0.2;
	sizeY = 1.5f;
	colorForBlock = sf::Color::Cyan;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(2, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Wall block 2
	posX = 1.55f;
	posY = 0.75f;
	sizeX = 0.2;
	sizeY = 1.5f;
	colorForBlock = sf::Color::Cyan;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(3, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Ground block 1
	posX = 3.25f;
	posY = -0.05;
	sizeX = 3.6;
	sizeY = 0.2f;
	colorForBlock = sf::Color::Green;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(4, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));
	 
	//Wall block 3
	posX = 4.95;
	posY = -0.35f;
	sizeX = 0.2;
	sizeY = 0.5f;
	colorForBlock = sf::Color::Cyan;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(5, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Spike 0
	posX = 8.1f;
	posY = -0.465f;
	sizeX = 2.5f;
	sizeY = 0.2f;
	colorForBlock = sf::Color::Red;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(6, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Ground block 2
	posX = 7.485f;
	posY = -0.75f;
	sizeX = 0.3;
	sizeY = 0.1f;
	colorForBlock = sf::Color::Green;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(7, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Ground block 3
	posX = 8.6f;
	posY = -0.65f;
	sizeX = 0.3;
	sizeY = 0.1f;
	colorForBlock = sf::Color::Green;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(8, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Ground block 4
	posX = 9.9f;
	posY = -0.925f;
	sizeX = 1.3;
	sizeY = 0.1f;
	colorForBlock = sf::Color::Green;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(9, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Wall block 4
	posX = 10.3f;
	posY = -0.845f;
	sizeX = 0.1;
	sizeY = 1.f;
	colorForBlock = sf::Color::Cyan;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(10, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Ground block 5
	posX = 14.f;
	posY = -1.375f;
	sizeX = 7.5;
	sizeY = 0.1f;
	colorForBlock = sf::Color::Green;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(11, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Ground block 6
	posX = 18.05;
	posY = -2.05f;
	sizeX = 2.;
	sizeY = 0.1f;
	colorForBlock = sf::Color::Green;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(12, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));
	
	//Wall block 5
	posX = 17.75;
	posY = -1.3f;
	sizeX = 0.1;
	sizeY = 1.f;
	colorForBlock = sf::Color::Cyan;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(13, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Ground block 7
	posX = 19.8;
	posY = -2.5f;
	sizeX = 2.1;
	sizeY = 0.1f;
	colorForBlock = sf::Color::Green;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(14, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));
	
	//Wall block 6
	posX = 18.8;
	posY = -2.f;
	sizeX = 0.1;
	sizeY = 1.f;
	colorForBlock = sf::Color::Cyan;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(15, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Wall block 7
	posX = 20.8;
	posY = -1.5f;
	sizeX = 0.1;
	sizeY = 2.f;
	colorForBlock = sf::Color::Cyan;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(16, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Ground block 8
	posX = 28.2;
	posY = -2.5f;
	sizeX = 7;
	sizeY = 0.1f;
	colorForBlock = sf::Color::Green;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(17, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Wall block 6
	posX = 24.75;
	posY = -2.f;
	sizeX = 0.1;
	sizeY = 1.f;
	colorForBlock = sf::Color::Cyan;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(18, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));

	//Wall block 8
	posX = 31.7;
	posY = -2.8f;
	sizeX = 0.1;
	sizeY = 2.f;
	colorForBlock = sf::Color::Cyan;
	m_staticBlocks.insert(std::pair<int, StaticBlock>(19, StaticBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), colorForBlock, 0.f, true, STATICBLOCK, CHARACTER | ENEMY)));




	/////////////////////////////////////////////
    ////                                     ////
    ////                                     ////
    ////        convexShape Objects          ////
    ////                                     ////
    ////                                     ////
    /////////////////////////////////////////////	

	//Convex Shape 0 
	posX = 5.2f;
	posY = -0.73f;
	b2Vec2 verts_1[4];
	verts_1[0].Set(-0.325, 0);
	verts_1[1].Set(0.175, -0.2);
	verts_1[2].Set(1.645, -0.2);
	verts_1[3].Set(1.645, 0);
	m_staticConvexShapes.insert(std::pair<int, StaticConvex>(0, StaticConvex(m_pWorld, sf::Vector2f(posX, posY), verts_1, 4, STATICBLOCK, CHARACTER)));

	//Convex Shape 2 (not ground)
	posX = 25.f;
	posY = -4.f;
	b2Vec2 verts_2[3];
	verts_2[0].Set(0 , 0);
	verts_2[1].Set(1, 1);
	verts_2[2].Set(2, 0);
	m_staticConvexShapes.insert(std::pair<int, StaticConvex>(1, StaticConvex(m_pWorld, sf::Vector2f(posX, posY), verts_2, 3, STATICBLOCK, CHARACTER)));




  /////////////////////////////////////////////
  ////                                     ////
  ////                                     ////
  ////         DynamicBlocks               ////
  ////                                     ////
  ////                                     ////
  /////////////////////////////////////////////

    //Falling Platform 0  
	posX = 22.22;
	posY = -2.52f;
	sizeX = 0.45;
	sizeY = 0.1f;
	colorForBlock = sf::Color::Magenta;
	m_dynamicBlocks.insert(std::pair<int, DynamicBlock>(0, DynamicBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0, colorForBlock, DYNAMICBLOCK, CHARACTER)));

	//Falling Platform 1  
	posX = 23.82;
	posY = -2.52f;
	sizeX = 0.45;
	sizeY = 0.1f;
	colorForBlock = sf::Color::Magenta;
	m_dynamicBlocks.insert(std::pair<int, DynamicBlock>(1, DynamicBlock(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0, colorForBlock, DYNAMICBLOCK, CHARACTER)));




	/////////////////////////////////////////////
	////                                     ////
	////                                     ////
	////         Collectibles                ////
	////                                     ////
	////                                     ////
	/////////////////////////////////////////////

	//Sword collectible
	posX = 25.25f;
	posY = -2.8f;
	sizeX = 0.3f;
	sizeY = 0.3f;
	sword = swordCollectible(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY));


	//Heart 0 
	posX = 30.;
	posY = -2.8f;
	sizeX = 0.2f;
	sizeY = 0.2f;
	heartCollectibles.insert(std::pair<int, heartCollectible>(0, heartCollectible(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY))));




   /////////////////////////////////////////////
   ////                                     ////
   ////                                     ////
   ////            SensorObjects            ////
   ////                                     ////
   ////                                     ////
   /////////////////////////////////////////////

    //Players weapon sensor (uncomment for testing purposes only)
	//m_sensorObjects.insert(std::pair<int, sensorObject>(-1, sensorObject(m_pWorld, sf::Vector2f(-1000, 0), sf::Vector2f(player.getSwordSize().x, player.getSwordSize().y), player.getSwordAngle(), nullptr, &player, &skeleton[1], nullptr, false, nullptr, 0, PLAYERSWORD, ENEMY)));
	//m_sensorObjects.at(-1).setUserData(&m_sensorObjects.at(-1));

	//Players weapon sensor 
	m_sensorObjects.insert(std::pair<int, sensorObject>(0, sensorObject(m_pWorld, sf::Vector2f(-1000, 0), sf::Vector2f(player.getSwordSize().x, player.getSwordSize().y), player.getSwordAngle(), nullptr, &player, &skeleton[1], nullptr, false, nullptr, 0, PLAYERSWORD, ENEMY)));
	m_sensorObjects.at(0).setUserData(&m_sensorObjects.at(0));

	//Skeleton 0 weapon sensor
	m_sensorObjects.insert(std::pair<int, sensorObject>(1, sensorObject(m_pWorld, sf::Vector2f(-29.25, -0.35), sf::Vector2f(skeleton[0].getSwordSize().x * 2, skeleton[0].getSwordSize().y * 2), skeleton[0].getSwordAngle(), nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, ENEMYSWORD, CHARACTER)));
	m_sensorObjects.at(1).setUserData(&m_sensorObjects.at(1));

	//Skeleton 1 weapon sensor
	m_sensorObjects.insert(std::pair<int, sensorObject>(2, sensorObject(m_pWorld, sf::Vector2f(-29.25, -0.35), sf::Vector2f(skeleton[1].getSwordSize().x * 2, skeleton[1].getSwordSize().y * 2), skeleton[1].getSwordAngle(), nullptr, &player, &skeleton[1], nullptr, false, nullptr, 0, ENEMYSWORD, CHARACTER)));
	m_sensorObjects.at(2).setUserData(&m_sensorObjects.at(2));

	//Sensor for player ground collision (ground block 0)
	posX = -1.6f;
	posY = -0.05;
	sizeX = 4.6f;
	sizeY = 0.2f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(3, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, GROUNDDETECTION, CHARACTER)));
	m_sensorObjects.at(3).setUserData(&m_sensorObjects.at(3));

    //Sensor for player ground collision (ground block 1)
	posX = 3.25f;
	posY = -0.05;
	sizeX = 3.6;
	sizeY = 0.2f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(4, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, GROUNDDETECTION, CHARACTER)));
	m_sensorObjects.at(4).setUserData(&m_sensorObjects.at(4));

	posX = 5.2;
	posY = -0.73f;
	//The vertices are already set (Check out the ConvexShape objects above)
	m_sensorObjects.insert(std::pair<int, sensorObject>(6, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(0, 0), 0.f,nullptr, &player, nullptr, nullptr, true, verts_1, 4, GROUNDDETECTION, CHARACTER)));
	m_sensorObjects.at(6).setUserData(&m_sensorObjects.at(6));

	//Spike 0 sensor
	posX = 8.1f;
	posY = -0.465f;
	sizeX = 2.5f;
	sizeY = 0.2f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(7, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, SPIKES, CHARACTER)));
	m_sensorObjects.at(7).setUserData(&m_sensorObjects.at(7));

	//Sensor for player ground collision (ground block 2)
	posX = 7.485f;
	posY = -0.75f;
	sizeX = 0.3;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(8, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, GROUNDDETECTION, CHARACTER)));
	m_sensorObjects.at(8).setUserData(&m_sensorObjects.at(8));

	//Sensor for player ground collision (ground block 3)
	posX = 8.6f;
	posY = -0.65f;
	sizeX = 0.3;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(9, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, GROUNDDETECTION, CHARACTER)));
	m_sensorObjects.at(9).setUserData(&m_sensorObjects.at(9));

	//Sensor for player ground collision (ground block 4)
	posX = 9.9f;
	posY = -0.925f;
	sizeX = 1.3;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(10, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, GROUNDDETECTION, CHARACTER)));
	m_sensorObjects.at(10).setUserData(&m_sensorObjects.at(10));
	
	
	//Sensor for player ground collision (ground block 5)
	posX = 14.f;
	posY = -1.375f;
	sizeX = 7.5;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(11, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, GROUNDDETECTION, CHARACTER)));
	m_sensorObjects.at(11).setUserData(&m_sensorObjects.at(11));

	//Sensor for player ground collision (ground block 6)
	posX = 18.05;
	posY = -2.05f;
	sizeX = 2.;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(12, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, GROUNDDETECTION, CHARACTER)));
	m_sensorObjects.at(12).setUserData(&m_sensorObjects.at(12));

	//Sensor for player ground collision (ground block 7)
	posX = 19.8;
	posY = -2.5f;
	sizeX = 2.1;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(13, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, GROUNDDETECTION, CHARACTER)));
	m_sensorObjects.at(13).setUserData(&m_sensorObjects.at(13));

	//Sensor for player ground collision (ground block 8)
	posX = 28.2;
	posY = -2.5f;
	sizeX = 7;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(14, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, GROUNDDETECTION, CHARACTER)));
	m_sensorObjects.at(14).setUserData(&m_sensorObjects.at(14));
	m_sensorObjects.insert(std::pair<int, sensorObject>(15, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, GROUNDDETECTION, ENEMY)));
    m_sensorObjects.at(15).setUserData(&m_sensorObjects.at(15));

	//JumpSensor 0 (sensor for the skeleton (when to jump)
	posX = 16.2;
	posY = -1.375f;
	sizeX = 0.3f;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(16, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, JUMPSENSOR, ENEMY)));
	m_sensorObjects.at(16).setUserData(&m_sensorObjects.at(16));

	//JumpSensor 1 (sensor for the skeleton (when to jump)
	posX = 19.25f;
	posY = -2.05f;
	sizeX = 2.;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(17, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, JUMPSENSOR, ENEMY)));
	m_sensorObjects.at(17).setUserData(&m_sensorObjects.at(17));

	//Player under map death sensor
	posX = 22.f;
	posY = -1.75f;
	sizeX = 6.4f;
	sizeY = 0.2f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(18, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], nullptr, false, nullptr, 0, FALLOFFMAP, CHARACTER)));
	m_sensorObjects.at(18).setUserData(&m_sensorObjects.at(18));

	//Sensor for falling platform 0 
	posX = 22.22;
	posY = -2.52f;
	sizeX = 0.45;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(19, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, &m_dynamicBlocks.at(0), &player, &skeleton[0], nullptr, false, nullptr, 0, DYNAMICBLOCK, CHARACTER)));
	m_sensorObjects.at(19).setUserData(&m_sensorObjects.at(19));

	//Sensor for falling platform 1 
	posX = 23.82;
	posY = -2.52f;
	sizeX = 0.45;
	sizeY = 0.1f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(20, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, &m_dynamicBlocks.at(1), &player, &skeleton[0], nullptr, false, nullptr, 0, DYNAMICBLOCK, CHARACTER)));
	m_sensorObjects.at(20).setUserData(&m_sensorObjects.at(20));

	////Sword collectible sensor
	posX = 25.25f;
	posY = -2.8f;
	sizeX = 0.3f;
	sizeY = 0.3f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(21, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], &heartCollectibles.at(0), false, nullptr, 0, SWORDCOLLECTIBLE, CHARACTER)));
	m_sensorObjects.at(21).setUserData(&m_sensorObjects.at(21));

	//Heart 0 Sensor
	posX = 30.f;
	posY = -2.8f;
	sizeX = 0.2f;
	sizeY = 0.2f;
	m_sensorObjects.insert(std::pair<int, sensorObject>(22, sensorObject(m_pWorld, sf::Vector2f(posX, posY), sf::Vector2f(sizeX, sizeY), 0.f, nullptr, &player, &skeleton[0], &heartCollectibles.at(0), false, nullptr, 0, HEART, CHARACTER)));
	m_sensorObjects.at(22).setUserData(&m_sensorObjects.at(22));
}



void Game::setEnemyNewPos()
{
	
	if (player.getPosition().x > 26 && enemyPosSet[1] == false) {
		lookAtEnemy0 = false;
		sf::Vector2f newPosition;
		newPosition.x = 31.25;
		newPosition.y = -3.f;
		skeleton[1].setNewPosition(newPosition);
		enemyPosSet[1] = true;
	}

	if (player.getPosition().x > 13.7 && enemyPosSet[0] == false) {
		sf::Vector2f newPosition;
		newPosition.x = 11.2;
		newPosition.y = -4.f;
		skeleton[0].setNewPosition(newPosition);
		enemyPosSet[0] = true;
	}

}



void Game::checkCollectiblesState()
{
	if (player.getPosition().x > 30.f && heartCollected[0] == false)
	{
		heartCollectibles.at(0).destroyBody(m_pWorld);
		heartCollectibles.erase(0);
		m_sensorObjects.at(22).destroySensor(m_pWorld);
		m_sensorObjects.erase(22);
		heartCollected[0] = true;
	}

	if (player.getPosition().x > 25.3f && swordCollected == false)
	{
		std::cout << "sword is collected";
		sword.destroyBody(m_pWorld);
		m_sensorObjects.at(21).destroySensor(m_pWorld);
		m_sensorObjects.erase(21);
		swordCollected = true;

	}

	playerHasSword = player.isSwordCollected();
}



void Game::checkFallingPlatformState()
{

	if (player.getPosition().x > 27.f && fallingPlatformsDeleted == false) {

		m_sensorObjects.at(19).destroySensor(m_pWorld);
		m_sensorObjects.erase(19);
		m_sensorObjects.at(20).destroySensor(m_pWorld);
		m_sensorObjects.erase(20);


		m_dynamicBlocks.at(1).destroyBody(m_pWorld);
		m_dynamicBlocks.erase(1);
		m_dynamicBlocks.at(0).destroyBody(m_pWorld);
		m_dynamicBlocks.erase(0);
		fallingPlatformsDeleted = true;

	}
}


