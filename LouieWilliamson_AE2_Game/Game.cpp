#include "Game.h"



Game::Game(HWND window)
{
	hWnd = window;
}

//Cleans up object pointers
Game::~Game()
{

	if (g_2DText)
	{
		delete g_2DText;
		g_2DText = nullptr;
	}

	if (astSpawner)
	{
		delete astSpawner;
		astSpawner = nullptr;
	}

	if (pickupSpawner)
	{
		delete pickupSpawner;
		pickupSpawner = nullptr;
	}

	if (enemySpawner)
	{
		delete enemySpawner;
		enemySpawner = nullptr;
	}

	if (player)
	{
		delete player;
		player = nullptr;
	}

	if (earth)
	{
		delete earth;
		earth = nullptr;
	}

	if (camera)
	{
		delete camera;
		camera = nullptr;
	}

	if (skybox)
	{
		delete skybox;
		skybox = nullptr;
	}
}
//Initialises the Input, DirectX, Objects, and Skybox
void Game::Initialise(HINSTANCE g_hInst)
{
	//initialise input
	if (FAILED(input.Initialise(g_hInst, hWnd)))
	{
		DXTRACE_MSG("Failed to initalise input");
		Close();
	}

	//initialise directx 
	dx.Initialise(hWnd);

	InitialiseGameobjects();

	//initialise skybox
	skybox = new Skybox(&dx);

	if (FAILED(skybox->Initialise()))
	{
		DXTRACE_MSG("Failed to initialise skybox");
		Close();
	}
}

//Run every frame in main
void Game::Update()
{
	//Check if the player is dead
	CheckDeath();

	//If they are alive update and draw everything
	if (isPlayerAlive)
	{

		// Get user input
		CheckInput();
		CheckQuit();

		// Tick timer (used for fps)
		timer.Tick();

		// Update Objects and Update and Draw Text
		UpdateObjects();
	}
	// Clear the back buffer
	float rgba_clear_colour[4] = { 0.29f, 0.12f, 0.47f, 1.0f };
	dx.ImmediateContext->ClearRenderTargetView(dx.BackBufferRTView, rgba_clear_colour);
	dx.ImmediateContext->ClearDepthStencilView(dx.ZBuffer, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	dx.ImmediateContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Draw Objects 
	DrawObjects(&view, &projection);

	//Draw UI
	DrawUI();

	//Draws the game over text if the player dies
	if (!isPlayerAlive)
	{
		DrawEnd();
		CheckQuit();
	}

	RenderText();

	// Display what has just been rendered
	dx.swapChain->Present(0, 0);
}
//Draw the game over text onto the screen along with the player's final score
void Game::DrawEnd()
{
	g_2DText->AddText("YOU DIED", -0.6f, 0.4f, textSize * 4);
	g_2DText->AddText("Final Score ", -0.6f, 0.0f, textSize * 2);
	g_2DText->AddText(to_string(player->score->GetScore()), 0.35f, 0.0f, textSize * 2);
	g_2DText->AddText("Press ESC to Quit", -0.5f, -0.3f, textSize);
}
//Renders all the text that has been added
void Game::RenderText()
{
	dx.ImmediateContext->OMSetBlendState(dx.AlphaBlendEnable, 0, 0xffffffff);
	g_2DText->RenderText();
	dx.ImmediateContext->OMSetBlendState(dx.AlphaBlendDisable, 0, 0xffffffff);
}
//Updates all of the objects and does collision checks
void Game::UpdateObjects()
{
	player->UpdatePlayer();
	player->UpdateBullets();

	//Slowly rotates and very slowly moves towards the camera
	earth->Update();
	earth->model->IncrementYrot(0.002f);
	earth->model->IncrementZpos(-0.05f);

	//Slowly rotates and very slowly moves towards the camera
	moon->Update();
	moon->model->IncrementYrot(0.0025f);
	moon->model->IncrementZpos(-0.05f);

	astSpawner->Update();
	pickupSpawner->Update();
	enemySpawner->Update();

	//Collision checks
	AsteroidPlayerCollision();
	FriendlyBulletCollision();
	EnemyBulletCollision();
	EnemyPlayerCollision();
	EnemyAsteroidCollision();
}

//Draw all objects 
void Game::DrawObjects(XMMATRIX* view, XMMATRIX* projection)
{
	//models
	skybox->Draw(view, projection, camera->GetX(), camera->GetY(), camera->GetZ());
	player->model->Draw(view, projection, true);
	player->DrawBullets(view, projection, false);
	earth->model->Draw(view, projection, true);
	moon->model->Draw(view, projection, true);
	//spawners
	astSpawner->DrawActives(view, projection, true);
	pickupSpawner->DrawActives(view, projection, true);
	enemySpawner->DrawActives(view, projection, true);

	//if the player's shield is active, draw it (with transparency)
	if (player->GetShieldActive())
	{
		dx.ImmediateContext->OMSetBlendState(dx.AlphaBlendEnable, 0, 0xffffffff);
		player->DrawShield(view, projection, true);
		dx.ImmediateContext->OMSetBlendState(dx.AlphaBlendDisable, 0, 0xffffffff);
	}
}
//Draws the text ui
void Game::DrawUI()
{
	//Pickup UI Text
	CheckPickupUI();

	//Ammo
	g_2DText->AddText("Ammo", -1.0f, 0.92f, textSize);	
	g_2DText->AddText(to_string(player->GetAmmo()), -0.7f, 0.92f, textSize);
	//Health
	g_2DText->AddText("Health", -1.0f, 1.0f, textSize);
	g_2DText->AddText(to_string(player->GetHealth()), -0.7f, 1.0f, textSize);
	//Score
	g_2DText->AddText("Score", -0.2f, 1.0f, textSize); // x and y are between -1.0 and 1.0
	g_2DText->AddText(to_string(player->GetScore()), 0.1f, 0.99f, textSize);
	//FPS Counter
	g_2DText->AddText("FPS", 0.65f, 1.0f, textSize); // x and y are between -1.0 and 1.0
	g_2DText->AddText(to_string(framespersecond), 0.83f, 0.99f, textSize);

}

//Show the appropriate text if a pickup is active
void Game::CheckPickupUI()
{
	if (pickupSpawner->speedPickup->EffectActive)
	{
		g_2DText->AddText("Speed Boost", -0.25f, 0.60f, textSize);
	}
	if (pickupSpawner->healthPickup->EffectActive)
	{
		g_2DText->AddText("Health Increased", -0.35f, 0.60f, textSize);
	}
	if (pickupSpawner->shieldPickup->EffectActive)
	{
		g_2DText->AddText("Shield Activated", -0.33f, 0.60f, textSize);
	}
}

//checks if the player is still alive
void Game::CheckDeath()
{
	if (player->GetHealth() <= 0)
	{
		isPlayerAlive = false;
	}
}

//Handles collision between the player and asteroids
void Game::AsteroidPlayerCollision()
{
	//for each active asteroid
	for (int i = 0; i < astSpawner->ActiveAsteroids.size(); i++)
	{
		//if its Z pos is less than 100 (only checks asteroids close to the player)
		if (astSpawner->ActiveAsteroids[i]->GetZ() < 100)
		{
			//if this asteroid hasn't already hit the player (prevents multiple collisions)
			if (!astSpawner->ActiveAsteroids[i]->GetAlreadyCollided())
			{
				//check collision between this asteroid and the player
				if (astSpawner->ActiveAsteroids[i]->model->CheckCollision(player->model))
				{
					//if collided, call the function in the asteroid and damage the player
					astSpawner->ActiveAsteroids[i]->CollidedWithPlayer();

					//Apply damage to the player if they don't have their shield active
					if (!player->GetShieldActive()) player->ChangeHealth(-5);
				}
			}
		}
	}
}

//Handles collisions between the enemies and asteroids
void Game::EnemyAsteroidCollision()
{
	//for each active enemy
	for (int i = 0; i < enemySpawner->ActiveEnemies.size(); i++)
	{
		//for each active asteroid
		for (int j = 0; j < astSpawner->ActiveAsteroids.size(); j++)
		{
			//if enemy collides with asteroid, disable enemy and apply asteroid bounce effect
			if (enemySpawner->ActiveEnemies[i]->model->CheckCollision(astSpawner->ActiveAsteroids[j]->model))
			{
				enemySpawner->ActiveEnemies[i]->SetActive(false);
				astSpawner->ActiveAsteroids[j]->CollidedWithEnemy();
			}
		}
	}
}

//Handles collisions between the enemies and the player
void Game::EnemyPlayerCollision()
{
	//for each enemy
	for (int i = 0; i < enemySpawner->ActiveEnemies.size(); i++)
	{
		//if it is close to the player
		if (enemySpawner->ActiveEnemies[i]->GetZ() < 100)
		{
			//if it hasn't already collided with the player -- this is so it doesnt collide multiple times
			if (!enemySpawner->ActiveEnemies[i]->GetAlreadyCollided())
			{
				//if it is colliding
				if (enemySpawner->ActiveEnemies[i]->model->CheckCollision(player->model))
				{
					//call enemy collided function
					enemySpawner->ActiveEnemies[i]->Collided();

					//Apply damage to the player if they don't have their shield active
					if (!player->GetShieldActive()) player->ChangeHealth(-5);
				}
			}
		}
	}
}

//Handles collisions between the player and the enemies' bullets
void Game::EnemyBulletCollision()
{
	//for each enemy
	for (int i = 0; i < enemySpawner->Enemies.size(); i++)
	{
		//for each bullet
		for (int j = 0; j < enemySpawner->Enemies[i]->maxBullets; j++)
		{
			//if the bullet is active, do the collision check
			if (enemySpawner->Enemies[i]->bullet[j]->GetActive())
			{
				//if the active bullet has collided with player, apply damage and disable bullet
				if (enemySpawner->Enemies[i]->bullet[j]->model->CheckCollision(player->model))
				{
					enemySpawner->Enemies[i]->bullet[j]->SetActive(false);

					//Apply damage to the player if they don't have their shield active
					if (!player->GetShieldActive()) player->ChangeHealth(-5);
				}
			}
		}
	}
}

//Handles collisions between the player's bullets with any active asteroids or enemies
void Game::FriendlyBulletCollision()
{
	//for each bullet
	for (int i = 0; i < player->maxBullets; i++)
	{
		//if its active
		if (player->bullet[i]->GetActive())
		{
			//check collision with each active asteroid
			for (int j = 0; j < astSpawner->ActiveAsteroids.size(); j++)
			{
				//if they collide, disable both and add points
				if (player->bullet[i]->model->CheckCollision(astSpawner->ActiveAsteroids[j]->model))
				{
					player->bullet[i]->SetActive(false);
					astSpawner->ActiveAsteroids[j]->SetActive(false);
					player->score->AddToScore(25);
				}
			}
			//check collision with each active enemy
			for (int k = 0; k < enemySpawner->ActiveEnemies.size(); k++)
			{
				//if they collide, disable both and add points
				if (player->bullet[i]->model->CheckCollision(enemySpawner->ActiveEnemies[k]->model))
				{
					player->bullet[i]->SetActive(false);
					enemySpawner->ActiveEnemies[k]->SetActive(false);
					player->score->AddToScore(50);
				}
			}
		}
	}
}

//Used to check for quit input when the game is over
void Game::CheckQuit()
{
	input.ReadInputStates();
	if (input.isKeyPressed(DIK_ESCAPE)) DestroyWindow(hWnd);
}
//This function handles all of the game input -- Keyboard only
void Game::CheckInput()
{
	input.ReadInputStates();
	
	// ---------------------------- X AXIS MOVEMENT--------------------------------
	if (input.isKeyPressed(DIK_D))
	{
		player->SetVelocity(player->GetXSpeed() , player->GetVelocityY(), player->GetVelocityZ());
		if (!player->atRight)
		{
			player->LookRight();
		}
		else
		{
			player->ResetZYRotation();
		}
	}

	if (input.isKeyPressed(DIK_A))
	{
		player->SetVelocity(-player->GetXSpeed(), player->GetVelocityY(), player->GetVelocityZ());
		if (!player->atLeft)
		{
			player->LookLeft();
		}
		else
		{
			player->ResetZYRotation();
		}
	}

	if (input.isKeyPressed(DIK_D) && input.isKeyPressed(DIK_A))
	{
		player->SetVelocity(0, player->GetVelocityY(), player->GetVelocityZ());
		player->ResetZYRotation();
	}
	else if (!input.isKeyPressed(DIK_D) && !input.isKeyPressed(DIK_A))
	{
		player->SetVelocity(0, player->GetVelocityY(), player->GetVelocityZ());
		player->ResetZYRotation();
	}
	// ---------------------------- Y AXIS MOVEMENT--------------------------------
	if (input.isKeyPressed(DIK_W))
	{
		player->SetVelocity(player->GetVelocityX(), player->GetYSpeed(), player->GetVelocityZ());
		
		if (!player->atTop)
		{
			player->LookUp();
		}
		else
		{
			player->ResetXRotation();
		}
	}

	if (input.isKeyPressed(DIK_S))
	{
		player->SetVelocity(player->GetVelocityX(), -player->GetYSpeed(), player->GetVelocityZ());
		if (!player->atBottom)
		{
			player->LookDown();
		}
		else
		{
			player->ResetXRotation();
		}
	}

	if (input.isKeyPressed(DIK_W) && input.isKeyPressed(DIK_S))
	{
		player->SetVelocity(player->GetVelocityX(), 0, player->GetVelocityZ());
		player->ResetXRotation();
	}
	else if (!input.isKeyPressed(DIK_S) && !input.isKeyPressed(DIK_W))
	{
		player->SetVelocity(player->GetVelocityX(), 0, player->GetVelocityZ());
		player->ResetXRotation();
	}
	// ---------------------------- Z AXIS MOVEMENT--------------------------------
	if (input.isKeyPressed(DIK_LSHIFT))
	{
		player->SetVelocity(player->GetVelocityX(), player->GetVelocityY(), player->GetZSpeed());

	} 
	else
	{
		player->SetVelocity(player->GetVelocityX(), player->GetVelocityY(), -player->GetZSpeed() / 2);
	}

	//---------------------------- SHOOTING ---------------------------------------
	if (input.isKeyPressed(DIK_SPACE))
	{
		if (!firedBullet) player->Shoot();
		firedBullet = true;
	}
	else
	{
		firedBullet = false;
	}
}

//Initialises all Game Objects
void Game::InitialiseGameobjects()
{
	//text
	g_2DText = new Text2D("assets/Font.png", dx.device, dx.ImmediateContext);

	//player
	player = new Player((char*)"assets/Models/shiptest.obj", (char*)"assets/Textures/shipbright2.jpg", 13, 10, 10, &dx);
	player->SetScale(0.01f);


	//World Objects
	earth = new GameObject((char*)"assets/Models/mySphere.obj", (char*)"assets/Textures/earth.jpg", -753, 0, 1953, &dx);
	earth->SetScale(60);
	earth->SetRot(0.09f, 5.3f, 0.0f);

	moon = new GameObject((char*)"assets/Models/mySphere.obj", (char*)"assets/Textures/moon.jpg", -489, 164, 1815, &dx);
	moon->SetScale(earth->GetScale()/4);
	moon->SetRot(0.11f, 5.3f, 0.3f);

	//enemies
	enemySpawner = new Spawner(SpawnType::enemy, player, &dx);

	//asteroids
	astSpawner = new Spawner(SpawnType::asteroid, player, &dx);
	
	//pickups
	pickupSpawner = new Spawner(SpawnType::pickup, player, &dx);

	//camera
	camera = new Camera(0.0f, 0.0f, -0.5f, 0.0f, player);

	//sets the view matrix
	view = camera->LookAt(player->startX, camera->GetY(), player->startZ + 5);
}
