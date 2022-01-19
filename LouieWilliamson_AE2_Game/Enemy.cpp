#include "Enemy.h"



Enemy::Enemy(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx, EnemyType eType, Player* p):Entity(modelFile, textureFile, x, y, z, p_dx)
{
	enemyType = eType;
	player = p;

	SetScale(0.01f);

	circleCentre.x = GetX();
	circleCentre.y = GetY();

	shootTimer.Reset();

	for (int i = 0; i < maxBullets; i++)
	{
		bullet[i] = new Bullet((char*)"assets/Models/mySphere.obj", (char*)"assets/Textures/bullets.jpg", 0, 0, 0, p_dx, false);
		bullet[i]->SetScale(0.15f);
	}
}


Enemy::~Enemy()
{

}

//Called every frame, updates the enemy
void Enemy::UpdateEnemy()
{
	//if the enemy is active update it and its bullets
	if (active)
	{
		Update();
		UpdateBullets();

		Movement();

		//only shoot if the enemy is not a chase type
		if (enemyType != EnemyType::chase)
		{
			CheckShootTimer();
		}
	}
}
//Tick the shooting timer, if enough time has passed, shoot and reset the timer.
void Enemy::CheckShootTimer()
{
	//This statement ensures the timer is setup at the start
	if (shootTimer.DeltaTime() < 0)
	{
		shootTimer.Start();
	}
	if (currentTime > TimeToShoot)
	{
		Shoot();
		currentTime = 0;
	}

	shootTimer.Tick();
	currentTime += shootTimer.DeltaTime();
}

//Shoots a bullet
void Enemy::Shoot()
{
	//Loops through the enemy bullets. If it isnt active, activate it, set pos and rotation and break from the loop
	//to ensure only one bullet is fired.
	for (int i = 0; i < maxBullets; i++)
	{
		if (!bullet[i]->GetActive())
		{
			bullet[i]->SetActive(true);
			bullet[i]->SetPos(GetX(), GetY(), GetZ());
			bullet[i]->SetRot(GetRotX(), GetRotY(), GetRotZ());
			break;
		}
	}
}
//Sets the enemy active, called when it is respawned
void Enemy::SetActive(bool isActive)
{
	active = isActive;
	currentTime = 0;
	hasCollided = false;

	//stops the timer if inactive so it doesnt still time when inactive
	if (!isActive)
	{
		shootTimer.Stop();
	}
}
//This function handles the movemet depending on the enemy type
void Enemy::Movement()
{
	switch (enemyType)
	{
	case chase:
		//if it is a chase enemy, make sure it is looking in the direction its flying
		LookAt(chaseLookAt.x, chaseLookAt.y, chaseLookAt.z);
		break;
	case line:
		//Ensures it is looking and moving in the Z- direction (towards the screen)
		LookAt(GetX(), GetY(), player->GetZ() - 100);
		SetVelocity(GetVelocityX() , GetVelocityY(),-movespeed);
		break;
	case circle:
		//fly straight forward (+Z) but circular X and Y movement
		LookAt(GetX(), GetY(), player->GetZ() - 100);
		SetVelocity(GetVelocityX(), GetVelocityY(), -movespeed);
		CircleMovement();
		break;
	default:
		break;
	}
	//Check if it has flown out of bounds
	CheckDeath();
}
//Sets the circlecentre float2 used to generate circle movement
void Enemy::SetCircleCentre(float x, float y, float z)
{
	circleCentre.x = x;
	circleCentre.y = y;
}
//Calculate X and Y based on circle, circle radius and angle
void Enemy::CircleMovement()
{
	float x;
	float y;

	x = circleCentre.x + cos(angle) * circleRadius;
	y = circleCentre.y + sin(angle) * circleRadius;

	model->SetPos(x, y, GetZ());

	angle += angleChange;

	if (angle >= 360.0f) angle = 0;
}
//Sets the velocity of the enemy in the direction of the player, used for the chase enemy.
void Enemy::TowardsLookAt()
{
	float dx = player->GetX() - GetX();
	float dy = player->GetY() - GetY();
	float dz = player->GetZ() - GetZ();

	chaseLookAt = { dx, dy, dz };

	float vMultiplier = 2;
	dx *= vMultiplier;
	dy *= vMultiplier;
	dz *= vMultiplier;

	SetVelocity(chaseLookAt.x, chaseLookAt.y, chaseLookAt.z);
}
//Called when collides with the player to create a bounce effect
void Enemy::Collided()
{
	hasCollided = true;
	SetVelocity(-GetVelocityX(), -GetVelocityY(), GetVelocityZ());
}
//Checks whether the enemy has gone out of bounds or loses all its health
void Enemy::CheckDeath()
{
	if (GetZ() < - 55)
	{
		SetActive(false);
	}
	if (GetHealth() <= 0)
	{
		SetActive(false);
	}
}
//Loops through bullets and draws it if its active
void Enemy::DrawBullets(XMMATRIX* view, XMMATRIX* projection, bool isLit)
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (bullet[i]->GetActive())
		{
			bullet[i]->model->Draw(view, projection, isLit);
		}
	}
}
//Loops through bullets and updates it if its active
void Enemy::UpdateBullets()
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (bullet[i]->GetActive())
		{
			bullet[i]->UpdateBullet();
		}
	}
}
