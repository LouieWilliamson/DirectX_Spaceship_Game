#include "Asteroid.h"



Asteroid::Asteroid(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx) :GameObject(modelFile, textureFile, x, y, z, p_dx)
{
	SetSpeed(normalSpeed);
	model->SetScale(10);
}


Asteroid::~Asteroid()
{

}
//Called every frame, updates the asteroid if it is active
void Asteroid::UpdateAst()
{
	Update();

	if (active)
	{
		//Constant Rotation
		RotateX(Xrot * rotationMultiplier);
		RotateY(Yrot * rotationMultiplier);
		RotateZ(Zrot * rotationMultiplier);

		if (GetZ() < -45)
		{
			SetActive(false);
		}
	}
	else
	{
		SetPos(offScreenX, offScreenY, offScreenZ);
	}
}

//Sets the position the asteroid moves towards
void Asteroid::SetLookAt(float x, float y, float z)
{
	LookatX = x;
	LookatY = y;
	LookatZ = z;
}

//Moves towards the look at position
void Asteroid::TowardsLookAt()
{
	float dx = LookatX - GetX();
	float dy = LookatY - GetY();
	float dz = LookatZ - GetZ();

	float vMultiplier = 0.35f;
	dx *= vMultiplier;
	dy *= vMultiplier;
	dz *= vMultiplier;

	SetVelocity(dx, dy, dz);
}

//Called when the asteroid is respawned/ despawned
void Asteroid::SetActive(bool isActive)
{
	active = isActive; 
	collided = false; 
	SetSpeed(normalSpeed);
}

//Called when collided with the player. Reduces the speed and flips the x and y movement to mimic a bounce effect
void Asteroid::CollidedWithPlayer()
{
	SetVelocity(-GetVelocityX(), -GetVelocityY(), GetVelocityZ());
	SetSpeed(speedAfterCollided);
	collided = true;
}

//Called when collided with an enemy. Reduces the speed and flips the x and y movement to mimic a bounce effect
void Asteroid::CollidedWithEnemy()
{
	SetVelocity(-GetVelocityX(), -GetVelocityY(), GetVelocityZ());
	SetSpeed(speedAfterCollided);
}
