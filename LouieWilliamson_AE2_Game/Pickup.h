#pragma once
#include "GameObject.h"
#include "Timer.h"
#include "Player.h"

//Enum to define the type of pickup
enum PickupType { health, speedboost, shield };
 
class Pickup :
	public GameObject
{
public:
	Pickup(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx, PickupType type, Player* p);
	~Pickup();
	void UpdatePickup();
	void EnableEffect();
	void DisableEffect();
	void SetPickedUp(bool isPicked) { isPickedUp = isPicked; }
	bool GetPickedUp() { return isPickedUp; }
	bool GetActive() { return active; }
	void SetActive(bool isActive) { active = isActive; }
	void CheckBoundaries();

	//Spawning
	int minX = -6, minY = 10, maxX = 33, maxY = 34;

	bool EffectActive = false;
private:
	//Timer
	Timer effectTimer;
	float currentEffectTime = 0;
	bool timerStarted = false;
	float effectSeconds = 5;
	
	bool isPickedUp = false;
	bool active = false;
	bool pickedOnce = false;
	float MoveSpeed = 400.0f;

	Player* player;
	PickupType m_Type;
};