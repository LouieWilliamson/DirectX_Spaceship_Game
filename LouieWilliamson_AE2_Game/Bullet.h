#pragma once
#include "GameObject.h"
class Bullet :
	public GameObject
{
public:
	Bullet(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx, bool isPlayer);
	~Bullet();
	void SetDamage(float dmg) { damage = dmg; }
	float GetDamage() { return damage; }
	void SetActive(bool active) { isActive = active; }
	bool GetActive() { return isActive; }
	void UpdateBullet();
private:
	bool FiredByPlayer = true;
	float moveSpeed = 0;
	float friendlySpeed = 5;
	float enemySpeed = -5;
	float damage = 25;
	bool isActive = false; 
};

