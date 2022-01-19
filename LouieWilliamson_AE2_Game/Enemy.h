#pragma once
#include "Entity.h"
#include "Timer.h"
#include "Bullet.h"
#include "Player.h"

enum EnemyType { chase,  line, circle};

class Enemy :
	public Entity
{
public:
	Enemy(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx, EnemyType eType, Player* p);
	~Enemy();

	void Shoot();
	void UpdateEnemy();
	bool GetActive() { return active; }
	void SetActive(bool isActive);
	void Movement();
	void CheckDeath();
	void SetType(EnemyType newType) { enemyType = newType; }
	void DrawBullets(XMMATRIX* view, XMMATRIX* projection, bool isLit);
	void UpdateBullets();
	void CircleMovement();
	void TowardsLookAt();
	bool GetAlreadyCollided() { return hasCollided; }
	void Collided();
	void CheckShootTimer();
	void SetCircleCentre(float x, float y, float z);
	const static int maxBullets = 5;
	Bullet* bullet[maxBullets];
	//Min and Max, X and Y spawn values
	int minX = -6, minY = 10, maxX = 33, maxY = 34;
private:
	//movement speeds
	float movespeed = 250.0f;
	float chaseSpeed = -1000.0f;
	//timer
	Timer shootTimer;
	float currentTime = 0;
	float TimeToShoot = 2.5f;

	XMFLOAT3 chaseLookAt;
	EnemyType enemyType;
	Player* player;

	bool active = false;
	bool hasCollided = false;
	//Circular Movement
	XMFLOAT2 circleCentre;
	float circleRadius = 25;
	float angle = 0.0f;
	float angleChange = 0.02f;
};

