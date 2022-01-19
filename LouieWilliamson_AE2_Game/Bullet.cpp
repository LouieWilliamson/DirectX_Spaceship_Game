#include "Bullet.h"



Bullet::Bullet(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx, bool isPlayer) :GameObject(modelFile, textureFile, x, y, z, p_dx)
{
	FiredByPlayer = isPlayer;
}

Bullet::~Bullet()
{
}

//Called every frame to update the bullet
void Bullet::UpdateBullet()
{
	Update();
	
	//If the bullet is fired by the player it move Z+ (away from the screen). If fired by an enemy it moves Z- (towards the screen)
	if (!FiredByPlayer)
	{
		model->IncrementZpos(enemySpeed);
	}
	else
	{
		model->MoveForward(friendlySpeed);
	}

	//if fired by player
	if (FiredByPlayer)
	{
		//check if its gone too far, if it has disable it
		if (GetZ() > 800)
		{
			SetActive(false);
		}
	}
	else
	//if fired by an enemy
	{
		//check if its gone too far, if it has disable it
		if (GetZ() < -50)
		{
			SetActive(false);
		}
	}
}
