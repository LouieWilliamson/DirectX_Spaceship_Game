#include "Spawner.h"



Spawner::Spawner(SpawnType type, Player* p, DXSetup* dx)
{
	spawnType = type;
	player = p;
	p_dx = dx;
	PopulateVector();
	spawnTimer.Reset();

	if (spawnType == SpawnType::enemy)
	{
		spawnTime = 10;
	}
}

//Cleans up pointers
Spawner::~Spawner()
{
	DeletePointers();
}

//Called every frame. It runs different code depending on the type of spawner (asteroid, enemy or pickup)
void Spawner::Update()
{
	UpdateVector();
	
	switch (spawnType)
	{
	case asteroid:
		//if all asteroids are not active
		if (ActiveAsteroids.size() < Asteroids.size())
		{
			//check time against spawn timer and spawn an asteroid if necessary
			if (currentTime > spawnTime)
			{
				SpawnAsteroid();
				currentTime = 0;
				spawnTime = 3;
			}
			currentTime += spawnTimer.DeltaTime();
			spawnTimer.Tick();
		}
		break;
	case enemy:
		//add to timer, if timer more than spawntime, spawn enemy, reset timer and generate new spawn time
		if (currentTime > spawnTime)
		{
			//spawn a pickup, reset timer and make new random spawn timer
			SpawnEnemy();
			currentTime = 0;

			//sets a new random spawn time between 5 and 10
			spawnTime = rand() % (10 - 5 + 1) + 5;
		}

		currentTime += spawnTimer.DeltaTime();
		spawnTimer.Tick();

		break;
	case pickup:		
		//check if any pickups are active
		activePickup = false;
		for (int i = 0; i < Pickups.size(); i++)
		{
			if (Pickups[i]->GetActive()) activePickup = true;
		}
		//if there are no active pickups, tick the timer and check if timer is more than spawntime
		if (!activePickup)
		{
			if (currentTime > spawnTime)
			{
				//spawn a pickup, reset timer and make new random spawn timer
				SpawnPickup();
				currentTime = 0;

				//random number between 5 and 10
				spawnTime = rand() % (10 - 5 + 1) + 5;
			}

			currentTime += spawnTimer.DeltaTime();
			spawnTimer.Tick();
		}
		break;
	default:
		break;
	}

}
//This handles the asteroid spawning
void Spawner::SpawnAsteroid()
{
	GenerateSpawnPoint();

	//Generate point for asteroid to move towards 
	RandomLookAt();

	//Generate rotation for asteroid
	RandomRotation();

	//for each asteroid
	for (int i = 0; i < Asteroids.size(); i++)
	{
		//if it isn't active, activate it
		if (!Asteroids[i]->GetActive())
		{
			//sets the asteroid to active and sets the position, lookat, rotation and scale randomly
			Asteroids[i]->SetActive(true);
			Asteroids[i]->SetPos(spawnX, spawnY, spawnZ);
			Asteroids[i]->SetLookAt(lookatX, lookatY, lookatZ);
			Asteroids[i]->TowardsLookAt();
			Asteroids[i]->SetAsteroidRot(rotx, roty, rotz);
			Asteroids[i]->SetScale(RandomScale());
			Asteroids[i]->SetScale(15);
			
			//breaks here to make sure only one is spawned
			break;
		}
	}
}
//This handles the pickup spawning
void Spawner::SpawnPickup()
{
	GenerateSpawnInLineWithScreen();

	//generate random number to randomise pickup type
	int randomPickup = rand() % (3 - 1 + 1) + 1;

	//sets the pickup active and sets the spawn position
	switch (randomPickup)
	{
	case 1:
		healthPickup->SetActive(true);
		healthPickup->SetPickedUp(false);
		healthPickup->SetPos(spawnX, spawnY, spawnZ);
		break;
	case 2:
		speedPickup->SetActive(true);
		speedPickup->SetPickedUp(false);
		speedPickup->SetPos(spawnX, spawnY, spawnZ);
		break;
	case 3:
		shieldPickup->SetActive(true);
		shieldPickup->SetPickedUp(false);
		shieldPickup->SetPos(spawnX, spawnY, spawnZ);
		break;
	default:
		healthPickup->SetActive(true);
		healthPickup->SetPickedUp(false);
		healthPickup->SetPos(spawnX, spawnY, spawnZ);
		break;
	}

}
//This handles the enemy spawning
void Spawner::SpawnEnemy()
{
	//random number between 1 and 3 -- used to select a random enemy type to spawn
	int randomtype = rand() % (3 - 1 + 1) + 1;

	//check each enemy in the list
	for (int i = 0; i < Enemies.size(); i++)
	{
		//if its not active assign a type from the random number and spawn it
		if (!Enemies[i]->GetActive())
		{
			//Chase enemy type
			if (randomtype == 1)
			{
				GenerateSpawnPoint();
				Enemies[i]->SetPos(spawnX, spawnY, spawnZ);
				Enemies[i]->SetRot(Enemies[i]->GetRotX(), Enemies[i]->GetRotY(), -Enemies[i]->GetRotZ());
				Enemies[i]->TowardsLookAt();
				Enemies[i]->SetType(EnemyType::chase);
			} 
			//Circle movement enemy type
			else if (randomtype == 2)
			{
				GenerateSpawnInLineWithScreen();
				Enemies[i]->SetPos(spawnX, spawnY, spawnZ);
				Enemies[i]->SetCircleCentre(spawnX, spawnY, spawnZ);
				Enemies[i]->SetRot(Enemies[i]->GetRotX(), Enemies[i]->GetRotY(), -Enemies[i]->GetRotZ());
				Enemies[i]->SetType(EnemyType::circle);
			} 
			//Straight line movement enemy type
			else if (randomtype == 3)
			{
				GenerateSpawnInLineWithScreen();
				Enemies[i]->SetPos(spawnX, spawnY, spawnZ);
				Enemies[i]->SetRot(Enemies[i]->GetRotX(), Enemies[i]->GetRotY(), -Enemies[i]->GetRotZ());
				Enemies[i]->SetType(EnemyType::line);
			}
			Enemies[i]->SetActive(true);

			//breaks here to make sure only one is spawned
			break;
		}
	}
}
//Draws the active objects
void Spawner::DrawActives(XMMATRIX* view, XMMATRIX* projection, bool isLit)
{
	switch (spawnType)
	{
	case asteroid:
		for (int i = 0; i < Asteroids.size(); i++)
		{
			if (Asteroids[i]->GetActive())
			{
				Asteroids[i]->model->Draw(view, projection, isLit);
			}
		}
		break;
	case enemy:
		for (int i = 0; i < Enemies.size(); i++)
		{
			if (Enemies[i]->GetActive())
			{
				Enemies[i]->model->Draw(view, projection, isLit);
				Enemies[i]->DrawBullets(view, projection, false);
			}
		}
		break;
	case pickup:
		for (int i = 0; i < Pickups.size(); i++)
		{
			if (Pickups[i]->GetActive())
			{
				//enables transparency
				p_dx->ImmediateContext->OMSetBlendState(p_dx->AlphaBlendEnable, 0, 0xffffffff);
				Pickups[i]->model->Draw(view, projection, isLit);
				p_dx->ImmediateContext->OMSetBlendState(p_dx->AlphaBlendDisable, 0, 0xffffffff);
			}
		}
		break;
	default:
		break;
	}
}
//Generates a random spawn point between the min and max values
void Spawner::GenerateSpawnPoint()
{
	spawnX = rand() % (maxX - minX + 1) + minX;
	spawnY = rand() % (maxY - minY + 1) + minY;
}

//Generates a random spawn point between the min and max values which are in line with the screen boundaries
void Spawner::GenerateSpawnInLineWithScreen()
{
	spawnZ = 600;

	if (spawnType == SpawnType::enemy)
	{
		spawnX = rand() % (Enemies[0]->maxX - Enemies[0]->minX + 1) + Enemies[0]->minX;
		spawnY = rand() % (Enemies[0]->maxY - Enemies[0]->minY + 1) + Enemies[0]->minY;
	}
	else if (spawnType == SpawnType::pickup)
	{
		spawnX = rand() % (Pickups[0]->maxX - Pickups[0]->minX + 1) + Pickups[0]->minX;
		spawnY = rand() % (Pickups[0]->maxY - Pickups[0]->minY + 1) + Pickups[0]->minY;
	}

}
//Sets a random look at position within or near the screen coordinates (used for the asteroids)
void Spawner::RandomLookAt()
{
	//generate random x and y near the screen to move towards
	lookatX = rand() % (maxLookX - minLookX + 1) + minLookX;
	lookatY = rand() % (maxLookY - minLookY + 1) + minLookY;
}

//Sets a random axis to rotate (used for the asteroids)
void Spawner::RandomRotation()
{
	int rotation = rand() % 3 + 1;

	switch (rotation)
	{
	case 1:
		rotx = 1;
		roty = 0;
		rotz = 0;
		break;
	case 2:
		rotx = 0;
		roty = 1;
		rotz = 0;
		break;
	case 3:
		rotx = 0;
		roty = 0;
		rotz = 1;
		break;
	default:
		rotx = 0;
		roty = 0;
		rotz = 0;
		break;
	}
}

//Sets a random scale 
float Spawner::RandomScale()
{
	int scale = rand() % 6 + 5;
	float rScale = scale * 0.01f;
	return rScale;
}

//Populates the vectors depending on the spawner type
void Spawner::PopulateVector()
{
	switch (spawnType)
	{
		//initialise asteroids and place into asteroid vector
	case asteroid:
		ast1 =  new Asteroid((char*)asteroidObj, (char*)asteroidTxt, 0, 0, -50, p_dx);
		ast2 =  new Asteroid((char*)asteroidObj, (char*)asteroidTxt, 0, 0, -50, p_dx);
		ast3 =  new Asteroid((char*)asteroidObj, (char*)asteroidTxt, 0, 0, -50, p_dx);
		ast4 =  new Asteroid((char*)asteroidObj, (char*)asteroidTxt, 0, 0, -50, p_dx);
		ast5 =  new Asteroid((char*)asteroidObj, (char*)asteroidTxt, 0, 0, -50, p_dx);
		ast6 =  new Asteroid((char*)asteroidObj, (char*)asteroidTxt, 0, 0, -50, p_dx);
		ast7 =  new Asteroid((char*)asteroidObj, (char*)asteroidTxt, 0, 0, -50, p_dx);
		ast8 =  new Asteroid((char*)asteroidObj, (char*)asteroidTxt, 0, 0, -50, p_dx);
		ast9 =  new Asteroid((char*)asteroidObj, (char*)asteroidTxt, 0, 0, -50, p_dx);
		ast10 = new Asteroid((char*)asteroidObj, (char*)asteroidTxt, 0, 0, -50, p_dx);
		Asteroids.push_back(ast1);
		Asteroids.push_back(ast2);
		Asteroids.push_back(ast3);
		Asteroids.push_back(ast4);
		Asteroids.push_back(ast5);
		Asteroids.push_back(ast6);
		Asteroids.push_back(ast7);
		Asteroids.push_back(ast8);
		Asteroids.push_back(ast9);
		Asteroids.push_back(ast10);
		break;
		//initialise enemies and place into enemies vector
	case enemy:
		e1 =	new Enemy((char*)enemyObj, (char*)enemyTexture, 0, 0, -50, p_dx, EnemyType::line, player);
		e2 =	new Enemy((char*)enemyObj, (char*)enemyTexture, 0, 0, -50, p_dx, EnemyType::line, player);
		e3 =	new Enemy((char*)enemyObj, (char*)enemyTexture, 0, 0, -50, p_dx, EnemyType::line, player);
		e4 =	new Enemy((char*)enemyObj, (char*)enemyTexture, 0, 0, -50, p_dx, EnemyType::line, player);
		e5 =	new Enemy((char*)enemyObj, (char*)enemyTexture, 0, 0, -50, p_dx, EnemyType::line, player);
		e6 =	new Enemy((char*)enemyObj, (char*)enemyTexture, 0, 0, -50, p_dx, EnemyType::line, player);
		e7 =	new Enemy((char*)enemyObj, (char*)enemyTexture, 0, 0, -50, p_dx, EnemyType::line, player);
		e8 =	new Enemy((char*)enemyObj, (char*)enemyTexture, 0, 0, -50, p_dx, EnemyType::line, player);
		e9 =	new Enemy((char*)enemyObj, (char*)enemyTexture, 0, 0, -50, p_dx, EnemyType::line, player);
		e10 =	new Enemy((char*)enemyObj, (char*)enemyTexture, 0, 0, -50, p_dx, EnemyType::line, player);
		Enemies.push_back(e1);
		Enemies.push_back(e2);
		Enemies.push_back(e3);
		Enemies.push_back(e4);
		Enemies.push_back(e5);
		Enemies.push_back(e6);
		Enemies.push_back(e7);
		Enemies.push_back(e8);
		Enemies.push_back(e9);
		Enemies.push_back(e10);
		break;
		//initalises pickups and places them into pickup vector
	case pickup:
		healthPickup = new Pickup((char*)pickupObj, (char*)healthTexture, 22, 10, 10, p_dx, PickupType::health, player);
		speedPickup = new Pickup((char*)pickupObj, (char*)speedTexture, 22, 10, 10, p_dx, PickupType::speedboost, player);
		shieldPickup = new Pickup((char*)pickupObj, (char*)shieldTexture, 22, 10, 10, p_dx, PickupType::shield, player);
		Pickups.push_back(healthPickup);
		Pickups.push_back(speedPickup);
		Pickups.push_back(shieldPickup);
		break;
	default:
		break;
	}
}

//Updates the vector which holds the active objects depending on the type of spawner -- also calls the update function for each active object
void Spawner::UpdateVector()
{
	switch (spawnType)
	{
	case asteroid:
		ActiveAsteroids.clear();
		for (int i = 0; i < Asteroids.size(); i++)
		{
			if (Asteroids[i]->GetActive())
			{
				Asteroids[i]->UpdateAst();
				ActiveAsteroids.push_back(Asteroids[i]);
			}
		}
		break;
	case enemy:
		ActiveEnemies.clear();
		for (int i = 0; i < Enemies.size(); i++)
		{
			if (Enemies[i]->GetActive())
			{
				Enemies[i]->UpdateEnemy();
				ActiveEnemies.push_back(Enemies[i]);
			}
		}
		break;
	case pickup:
		for (int i = 0; i < Pickups.size(); i++)
		{
			Pickups[i]->UpdatePickup();
		}
	default:
		break;
	}
}
//Cleans up all pointers
void Spawner::DeletePointers()
{
	//asteroids
	delete ast1;
	ast1 = nullptr;

	delete ast2;
	ast2 = nullptr;

	delete ast3;
	ast3 = nullptr;

	delete ast4;
	ast4 = nullptr;

	delete ast5;
	ast5 = nullptr;

	delete ast6;
	ast6 = nullptr;

	delete ast7;
	ast7 = nullptr;

	delete ast8;
	ast8 = nullptr;

	delete ast9;
	ast9 = nullptr;

	delete ast10;
	ast10 =nullptr;

	//pickups
	delete healthPickup;
	healthPickup = nullptr;

	delete speedPickup;
	speedPickup = nullptr;

	delete shieldPickup;
	shieldPickup = nullptr;

	//enemies
	delete e1;
	e1 = nullptr;

	delete e2;
	e2 = nullptr;

	delete e3;
	e3 = nullptr;

	delete e4;
	e4 = nullptr;

	delete e5;
	e5 = nullptr;

	delete e6;
	e6 = nullptr;

	delete e7;
	e7 = nullptr;

	delete e8;
	e8 = nullptr;

	delete e9;
	e9 = nullptr;

	delete e10;
	e10 = nullptr;
}

