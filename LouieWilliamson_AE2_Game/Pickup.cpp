#include "Pickup.h"



Pickup::Pickup(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx, PickupType type, Player* p):GameObject(modelFile, textureFile, x, y, z, p_dx)
{
	m_Type = type;
	player = p;
	SetVelocity(0, 0, -MoveSpeed);
	SetScale(GetScale() * 1.5f);

	//Sets the amount of time the affect and text ui lasts for depending on the pickup type
	switch (m_Type)
	{
	case health:
		effectSeconds = 3;
		break;
	case speedboost:
		effectSeconds = 10;
		break;
	case shield:
		effectSeconds = 15;
		break;
	default:
		break;
	}
}


Pickup::~Pickup()
{

}

//Called every frame, updates the pickup
void Pickup::UpdatePickup()
{
	//if the pickup is active in the game
	if (active)
	{
		Update();
		CheckBoundaries();

		//if it hasnt been picked up
		if (!isPickedUp)
		{
			//check collision with player
			if (model->CheckCollision(player->model))
			{
				//if they collide, enable the pickup effect
				isPickedUp = true;
				EnableEffect();
				SetActive(false);
			}
		}
	}
	//If the effect is active
	if (EffectActive)
	{
		//and the timer hasnt started already
		if (!timerStarted)
		{
			//Initialise the timer
			effectTimer.Reset();
			timerStarted = true;
		}
		effectTimer.Start();
		effectTimer.Tick();

		currentEffectTime += effectTimer.DeltaTime();

		//If the timer > time of the effect then disable it
		if (currentEffectTime > effectSeconds) DisableEffect();
	}
}
//Enables an effect depending on the pickup type
void Pickup::EnableEffect()
{
	EffectActive = true;
	currentEffectTime = 0;

	switch (m_Type)
	{
	case health:
		//add 25 health
		player->ChangeHealth(25);
		break;
	case speedboost:
		//double the players speed
		player->SetMoveSpeed(player->GetXSpeed() *2 , player->GetYSpeed() * 2, player->GetZSpeed() * 2);
		break;
	case shield:
		//activate the shield
		player->SetShieldActive(true);
		break;
	default:
		break;
	}
}
//Disables the pickup effect, called by the effect timer
void Pickup::DisableEffect()
{
	EffectActive = false;
	currentEffectTime = 0;
	effectTimer.Stop();

	switch (m_Type)
	{
	case speedboost:
		//halfs the players speed
		player->SetMoveSpeed(player->GetXSpeed() / 2, player->GetYSpeed() / 2, player->GetZSpeed() / 2);
		break;
	case shield:
		//disables the shield
		player->SetShieldActive(false);
		break;
	default:
		break;
	}
}
//Checks if the pickup has gone out of bounds and disables it if so
void Pickup::CheckBoundaries()
{
	if (GetZ() < 8)
	{
		SetActive(false);
		SetPickedUp(false);
	}
}
