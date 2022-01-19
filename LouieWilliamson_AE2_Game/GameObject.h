#pragma once
#include <d3d11.h> 
#include <dxgi.h> 
#include <D3DX11.h>
#include <windows.h>
#include <DxErr.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include "Model.h"

class GameObject
{
public:
	GameObject(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx);
	~GameObject();
	
	void Update();
	bool HasCollided(GameObject* g) { model->CheckCollision(g->model); }
	void LookAt(float x, float y, float z) { model->LookAt(x, y, z); }
	
	//Rotation
	void RotateX(float x) { model->IncrementXrot(x); }
	void RotateY(float y) { model->IncrementYrot(y); }
	void RotateZ(float z) { model->IncrementZrot(z); }
	//Setters
	void SetPos(float x, float y, float z) { model->SetPos(x, y, z); }
	void SetScale(float scale) { model->SetScale(scale); }
	void SetRot(float x, float y, float z);
	void SetSpeed(float spd) { speed = spd; }
	void SetVelocity(float vX, float vY, float vZ) { VelocityX = vX; VelocityY = vY; VelocityZ = vZ; }
	//Getters
	float GetX() { return model->GetX(); }
	float GetY() { return model->GetY(); }
	float GetZ() { return model->GetZ(); }
	float GetRotX() { return model->GetRotX(); }
	float GetRotY() { return model->GetRotY(); }
	float GetRotZ() { return model->GetRotZ(); }
	float GetScale() { return model->GetScale(); }
	float GetSpeed() { return speed; }
	float GetVelocityX() { return VelocityX; }
	float GetVelocityY() { return VelocityY; }
	float GetVelocityZ() { return VelocityZ; }
	
	Model* model;
private:
	float VelocityX = 0;
	float VelocityY = 0;
	float VelocityZ = 0;

	float speed = 0.001f;
};

