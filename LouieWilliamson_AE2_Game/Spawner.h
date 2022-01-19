#pragma once
#include <vector>
#include "Timer.h"
#include "Asteroid.h"
#include "Pickup.h"
#include "DXSetup.h"
#include "Enemy.h"

using namespace std;

//Enum to define the type of spawner
enum SpawnType { asteroid, enemy, pickup };

class Spawner
{
public:
	Spawner(SpawnType type, Player* p, DXSetup* dx);
	~Spawner();
	void Update();

	void GenerateSpawnPoint();
	void RandomLookAt();
	void RandomRotation();
	float RandomScale();
	void PopulateVector();
	void UpdateVector();
	void DeletePointers();

	void SpawnAsteroid();
	void SpawnPickup();
	void SpawnEnemy();

	void DrawActives(XMMATRIX* view, XMMATRIX* projection, bool isLit);

	//used for object pooling
	vector<Asteroid*> ActiveAsteroids;
	vector<Enemy*> ActiveEnemies;
	vector<Enemy*> Enemies;

	//pickups
	Pickup* speedPickup;
	Pickup* healthPickup;
	Pickup* shieldPickup;

private:
	Timer spawnTimer;
	DXSetup* p_dx;
	Player* player;
	SpawnType spawnType;

	//----------------------------------------------ENEMIES-----------------------------------	
	Enemy* e1 = nullptr;
	Enemy* e2 = nullptr;
	Enemy* e3 = nullptr;
	Enemy* e4 = nullptr;
	Enemy* e5 = nullptr;
	Enemy* e6 = nullptr;
	Enemy* e7 = nullptr;
	Enemy* e8 = nullptr;
	Enemy* e9 = nullptr;
	Enemy* e10 = nullptr;
	const char* enemyObj = "assets/Models/shiptest.obj";
	const char* enemyTexture = "assets/Textures/enemyship2.jpg";
	const char* chaseTexture = "assets/Textures/enemychasetxt.jpg";

	//----------------------------------------------PICKUPS-----------------------------------
	void GenerateSpawnInLineWithScreen();
	const char* pickupObj = "assets/Models/mySphere.obj";
	const char* healthTexture = "assets/Textures/Pickup_Health.png";
	const char* speedTexture = "assets/Textures/Pickup_Speed.png";
	const char* shieldTexture = "assets/Textures/Pickup_Shield.png";
	const char* weaponTexture = "assets/Textures/Pickup_Wpn.png";

	vector<Pickup*> Pickups;
	bool activePickup = false;
	//----------------------------------------------ASTEROIDS-----------------------------------
	Asteroid* ast1 = nullptr;
	Asteroid* ast2 = nullptr;
	Asteroid* ast3 = nullptr;
	Asteroid* ast4 = nullptr;
	Asteroid* ast5 = nullptr;
	Asteroid* ast6 = nullptr;
	Asteroid* ast7 = nullptr;
	Asteroid* ast8 = nullptr;
	Asteroid* ast9 = nullptr;
	Asteroid* ast10 = nullptr;
	const char* asteroidObj = "assets/Models/asteroid_export.obj";
	const char* asteroidTxt = "assets/Textures/rock.jpg";

	vector<Asteroid*> Asteroids;

	//------------------------------------------------TIMER-----------------------------------
	float currentTime = 0;
	float spawnTime = 5;

	//spawn values (to be generated) ----------------SPAWNING-------------------------------
	//minimum x and y spawn values
	int minX = -1000, minY = -400;
	//maximum x and y spawn values
	int maxX = 1050, maxY = 785;
	//inactive z pos (behind the camera)
	int minZ = -40; 
	//z spawn pos (edge of far clipping plane)
	int maxZ = 1962;	
	
	float spawnX = 0, spawnY = 0, spawnZ = maxZ + 10; // so it spawns outside clipping plane

	//look at values (to be generated)---------------LOOK AT------------------------------
	float lookatX = 0, lookatY = 0, lookatZ = 22;
	int minLookX = -19, minLookY = 5;
	int maxLookX = 45, maxLookY = 44;

	//rotation values (to be generated)--------------ROTATION-----------------------------
	float rotx = 0, roty = 0, rotz = 0;
};