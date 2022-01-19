#include "Player.h"



Player::Player(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx):Entity(modelFile, textureFile, x, y, z, p_dx)
{
	startX = x;
	startY = y;
	startZ = z;
	score = new Score(scoreFile);
	model->LookAt(GetX(), GetY(), GetZ() + 1);
	
	//initialises the shield
	shield = new GameObject((char*)"assets/Models/mySphere.obj", (char*)"assets/Textures/shield.png", GetX(), GetY(), GetZ(), p_dx);
	shield->SetScale(2.25f);

	//initialises the bullets
	for (int i = 0; i < maxBullets; i++)
	{
		bullet[i] = new Bullet((char*)"assets/Models/mySphere.obj", (char*)"assets/Textures/bullets.jpg", 0, 0, 0, p_dx, true);
		bullet[i]->SetScale(0.15f);
	}
}

//Cleans up pointers
Player::~Player()
{
	if (score)
	{
		delete score;
		score = nullptr;
	}
	for (int i = 0; i < maxBullets; i++)
	{
		delete bullet[i];
		bullet[i] = nullptr;
	}
	if (shield)
	{
		delete shield;
		shield = nullptr;
	}
}

//Called every frame, updates player position and score
void Player::UpdatePlayer()
{
	Update();
	shield->SetPos(GetX(), GetY(), GetZ());
	playerScore = score->GetScore();

	CheckBoundaries();
}

//Stops the player from moving off screen
void Player::CheckBoundaries()
{
	float x = GetX();
	float y = GetY();
	float z = GetZ();

	if (x >= maxX)
	{
		SetPos(maxX, GetY(), GetZ());
		atRight = true;
	}
	else if (x <= minX)
	{
		SetPos(minX, GetY(), GetZ());
		atLeft = true;
	}
	else
	{
		atLeft = false;
		atRight = false;
	}

	if (y >= maxY)
	{
		SetPos(GetX(), maxY, GetZ());
		atTop = true;
	}
	else if (y <= minY)
	{
		SetPos(GetX(), minY, GetZ());
		atBottom = true;
	}
	else
	{
		atTop = false;
		atBottom = false;
	}

	if (z >= maxZ)
	{
		SetPos(GetX(), GetY(), maxZ);
	}
	else if (z <= minZ)
	{
		SetPos(GetX(), GetY(), minZ);
	}
}

//Pitches the ship upwards
void Player::LookUp()
{
	float maxRot = -0.35f;

	if (GetRotX() > maxRot)
	{
		RotateX(-0.02f);
	}
}
//Pitches the ship downwards
void Player::LookDown()
{
	float maxRot = 0.25f;

	if (GetRotX() < maxRot)
	{
		RotateX(0.02f);
	}
}
//Rotates the X back to normal (used when the player moves to an edge)
void Player::ResetXRotation()
{
	float x = GetRotX();

	if (x != 0)
	{
		if (x < 0)
		{
			RotateX(0.02f);
		}
		else if (x > 0)
		{
			RotateX(-0.02f);
		}
	}
}
//Rotates the player left
void Player::LookLeft()
{
	if (GetRotZ() < 0.25f)
	{
		RotateZ(0.02f);
	}
	if (GetRotY() > -0.25f)
	{
		RotateY(-0.02f);
	}
}
//Rotates the player right
void Player::LookRight()
{
	if (GetRotZ() > -0.25f)
	{
		RotateZ(-0.02f);
	}

	if (GetRotY() < 0.25f)
	{
		RotateY(0.02f);
	}
}

//Rotates the ZY back to normal (used when the player moves to an edge)
void Player::ResetZYRotation()
{
	float z = GetRotZ();
	float y = GetRotY();
	
	if (z != 0)
	{
		if (z < 0)
		{
			RotateZ(0.02f);
		}
		else if (z > 0)
		{
			RotateZ(-0.02f);
		}
	}

	if (y != 0)
	{
		if (y < 0)
		{
			RotateY(0.02f);
		}
		else if (y > 0)
		{
			RotateY(-0.02f);
		}
	}
}
//Loops through the player bullets. If it isnt active, activate it, set pos and rotation and break from the loop
//to ensure only one bullet is fired.
void Player::Shoot()
{
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
//Works out the ammo but minusing active bullets from maxbullets
int Player::GetAmmo()
{
	int activeBullets = 0;

	for (int i = 0; i < maxBullets; i++)
	{
		if (bullet[i]->GetActive())
		{
			activeBullets++;
		}
	}

	int ammo = maxBullets - activeBullets;

	return ammo;
}

//Draws any active bullets
void Player::DrawBullets(XMMATRIX* view, XMMATRIX* projection, bool isLit)
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (bullet[i]->GetActive())
		{
			bullet[i]->model->Draw(view, projection, isLit);
		}
	}
}
//Updates any active bullets
void Player::UpdateBullets()
{
	for (int i = 0; i < maxBullets; i++)
	{
		if (bullet[i]->GetActive())
		{
			bullet[i]->UpdateBullet();
		}
	}
}
