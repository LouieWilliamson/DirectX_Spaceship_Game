#pragma once
#include <d3d11.h> 
#include <dxgi.h> 
#include <D3DX11.h>
#include <windows.h>
#include <DxErr.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include "DXSetup.h"
#include "Asteroid.h"
#include "Skybox.h"
#include "Input.h"
#include "text2D.h"
#include "Model.h"
#include "Camera.h"
#include "Timer.h"
#include "GameObject.h"
#include "Player.h"
#include "Pickup.h"
#include "Spawner.h"

enum Gamestate { Start, InGame, Paused, End };

class Game
{
public:
	Game(HWND window);
	~Game();
	//Functions
	void Initialise(HINSTANCE g_hInst);
	bool isRunning() { return running; }
	void Update();
	void SetFPS(int fps) { framespersecond = fps; }

	//FPS Timer
	Timer timer;
private:
	//DirectX Setup
	DXSetup dx;
	HWND hWnd;
	void CheckInput();
	void CheckQuit();
	XMMATRIX projection = XMMatrixPerspectiveFovLH(XMConvertToRadians(45.0f), 1280.0f / 960.0f, 1.0f, 2000.0f);
	XMMATRIX view;
	//Functions
	void InitialiseGameobjects();
	void Close() { running = false; };
	void UpdateObjects();
	void DrawObjects(XMMATRIX* view, XMMATRIX* projection);
	void DrawUI();
	void CheckPickupUI();
	void CheckDeath();
	void DrawEnd();
	void RenderText();
	//Collisions
	void AsteroidPlayerCollision();
	void EnemyPlayerCollision();
	void EnemyBulletCollision();
	void FriendlyBulletCollision();
	void EnemyAsteroidCollision();
	//Globals
	bool running = true;
	Skybox* skybox = nullptr;
	Input input;
	Text2D* g_2DText = nullptr;
	bool firedBullet = false;
	int framespersecond = 0;
	float textSize = 0.04f;
	bool isPlayerAlive = true;
	//objects
	Spawner* astSpawner = nullptr;
	Spawner* pickupSpawner = nullptr;
	Player* player = nullptr;
	Camera* camera = nullptr;
	GameObject* earth = nullptr;
	GameObject* moon = nullptr;
	Spawner* enemySpawner = nullptr;
};

