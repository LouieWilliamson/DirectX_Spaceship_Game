#pragma once
#include "Entity.h"
#include "Score.h"
#include "Bullet.h"

class Player :
	public Entity
{
public:
	Player(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx);
	~Player();

	void UpdatePlayer();
	void CheckBoundaries();
	int GetScore() { return playerScore; }	
	void Shoot();
	int GetAmmo();
	bool GetShieldActive() { return isShieldActive; }
	void SetShieldActive(bool isActive) { isShieldActive = isActive; }
	void LookUp();
	void LookDown();
	void LookLeft();
	void LookRight();
	void ResetZYRotation();
	void ResetXRotation();
	void DrawBullets(XMMATRIX* view, XMMATRIX* projection, bool isLit);
	void UpdateBullets();
	void DrawShield(XMMATRIX* view, XMMATRIX* projection, bool isLit) { shield->model->Draw(view, projection, isLit); }

	const static int maxBullets = 5;
	Bullet* bullet[maxBullets];
	Score* score = nullptr;
	bool atTop = false;
	bool atBottom = false;
	bool atLeft = false;
	bool atRight = false;
	float startX, startY, startZ;
private:
	float minX = -6.5f;
	float minY = 9.65f;
	float minZ = 10.0f;
	float maxX = 33.7f;
	float maxY = 34.55f;
	float maxZ = 30.0f;
	int playerScore = 0;
	const char* scoreFile = "Highscores.txt";
	GameObject* shield = nullptr;
	bool isShieldActive = false;
};

