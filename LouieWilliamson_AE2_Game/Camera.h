#include "GameObject.h"

using namespace DirectX;

#pragma once
class Camera
{
public:

	Camera(float x, float y, float z, float camera_rotation, GameObject* player);
	~Camera();

	XMMATRIX GetViewMatrix();
	XMMATRIX LookAt(float x, float y, float z);
	void Rotate(float degrees);
	void Forward(float distance);
	void Strafe(float distance);
	void Up(float distance);

	float GetX() { return m_x; }
	float GetY() { return m_y; }
	float GetZ() { return m_z; }

	bool getRotateLeft() { return rotateLeft; }
	bool getRotateRight() { return rotateRight; }
	bool getMoveLeft() { return moveLeft; }
	bool getMoveRight() { return moveRight; }
	bool getMoveBack() { return moveBack; }
	bool getMoveForwards() { return moveForwards; }
	bool getMoveUp() { return moveUp; }
	bool getMoveDown() { return moveDown; }

	void setRotation(bool left, bool right) { rotateLeft = left; rotateRight = right; }
	void setLeft(bool left) { moveLeft = left; }
	void setRight(bool right) { moveRight = right; }
	void setBack(bool back) { moveBack = back; }
	void setForwards(bool forwards) { moveForwards = forwards; }
	void setUpandDown(bool up, bool down) { moveUp = up; moveDown = down; }
private:

	GameObject* target;
	float lookatX = 12.7f;
	float lookatY = 19.0f;
	float lookatZ = -38.9f;

	bool rotateLeft = false;
	bool rotateRight = false;
	bool moveLeft = false;
	bool moveRight = false;
	bool moveBack = false;
	bool moveForwards = false;
	bool moveUp = false;
	bool moveDown = false;

	float m_x;
	float m_y;
	float m_z;

	float m_dx;
	float m_dy;
	float m_dz;
	float m_camera_rotation;
	XMVECTOR m_position;
	XMVECTOR m_lookat;
	XMVECTOR m_up;

	float cameraSpeed = 0.01f;
	float camRotateSpeed = 0.05f;

	float offsetX = 0;
	float offsetY = 5;
	float offsetZ = -30;

};

