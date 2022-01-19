#include "GameObject.h"



GameObject::GameObject(char* modelFile, char* textureFile, float x, float y, float z, DXSetup* p_dx)
{
	model = new Model(p_dx, (char*)modelFile, (char*)textureFile);
	model->SetPos(x,y,z);

}

//Cleans up model pointer
GameObject::~GameObject()
{
	if (model)
	{
		delete model;
		model = nullptr;
	}
}

//Updates model position based on velocity
void GameObject::Update()
{
	float xMove = VelocityX * speed;
	float yMove = VelocityY * speed;
	float zMove = VelocityZ * speed;

	model->IncrementXpos(xMove);
	model->IncrementYpos(yMove);
	model->IncrementZpos(zMove);
	
}

//Sets the rotation of the model
void GameObject::SetRot(float x, float y, float z)
{
	model->SetRotX(x);
	model->SetRotY(y);
	model->SetRotZ(z);
}