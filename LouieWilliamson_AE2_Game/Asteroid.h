#pragma once
#include "GameObject.h"
class Asteroid :
	public GameObject
{
public:
	Asteroid(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx);
	~Asteroid();

	void UpdateAst();
	void SetLookAt(float x, float y, float z);
	void TowardsLookAt();
	void SetActive(bool isActive);
	bool GetActive() { return active; }
	bool GetAlreadyCollided() { return collided; }
	void SetAsteroidRot(float x, float y, float z) { Xrot = x; Yrot = y; Zrot = z; }
	void CollidedWithPlayer();
	void CollidedWithEnemy();
private:
	//bools
	bool active = false;
	bool collided = false;
	//floats
	float Yrot = 0;
	float Xrot = 0;
	float Zrot = 0;
	float rotationMultiplier = 0.01f;

	float LookatX = 0;
	float LookatY = 0;
	float LookatZ = 0;

	float offScreenX = 0;
	float offScreenY = 0;
	float offScreenZ = -45;

	float normalSpeed = 0.005f;
	float speedAfterCollided = 0.002f;
};

