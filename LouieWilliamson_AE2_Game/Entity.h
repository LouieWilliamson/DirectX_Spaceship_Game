#pragma once
#include "GameObject.h"
#include "Score.h"
class Entity :
	public GameObject
{
public:
	Entity(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx);
	~Entity();

	int GetHealth() { return health; }
	void ChangeHealth(int change) { health += change; }
	void SetWpnDamage(int newDamage) { weaponDamage = newDamage; }
	float GetXSpeed() { return xSpeed; }
	float GetYSpeed() { return ySpeed; }
	float GetZSpeed() { return boostSpeed; }
	void SetMoveSpeed(float newXSpeed, float newYspeed, float newZspeed) { xSpeed = newXSpeed; ySpeed = newYspeed; boostSpeed = newZspeed; }

private:
	int health = 100;
	int weaponDamage = 25;
	float xSpeed = 600;
	float ySpeed = 300;
	float boostSpeed = 500;
};

