#include "Camera.h"



Camera::Camera(float x, float y, float z, float camera_rotation, GameObject* player)
{
	target = player;
	
	//sets the look at position and applies an offset
	m_x = target->GetX() + offsetX;
	m_y = target->GetY() + offsetY;
	m_z = target->GetZ() + offsetZ;

	m_camera_rotation = camera_rotation;

	m_dx = sin(m_camera_rotation * (XM_PI / 180.0));
	m_dz = cos(m_camera_rotation * (XM_PI / 180.0));
}


Camera::~Camera()
{
}

//This rotates the camera on the Y axis. It is not used in game but works as expected.
void Camera::Rotate(float degrees)
{
	m_camera_rotation += degrees;

	m_dx = sin(m_camera_rotation * (XM_PI / 180.0));
	m_dz = cos(m_camera_rotation * (XM_PI / 180.0));
}

//Moves camera forward. If distance < 0, it moves backwards
void Camera::Forward(float distance)
{
	m_x += m_dx * distance;
	m_z += m_dz * distance;
}

//Strafes the camera left and right. It is not used in game but works as expected.
void Camera::Strafe(float distance)
{
	//calculate forward vector, which is calculated from 2 normalised points we know: lookat minus position
	XMVECTOR forward = XMVector3Normalize(m_lookat - m_position);

	//calculate right vector, which is a normalised perpendicular vector to forward and up (hint: use cross product)
	XMVECTOR right = XMVector3Cross(forward, m_up); //cross product of (forward and up vectors);

	//update m_x and m_z, by multiplying right x and z by distance
	m_x += XMVectorGetX(right) * distance;
	m_z += XMVectorGetZ(right) * distance;
	
}
//Moves camera forward. If distance < 0, it moves down. It is not used in game but works as expected.
void Camera::Up(float distance)
{
	m_y += XMVectorGetY(m_up) * distance;
	m_z += XMVectorGetZ(m_up) * distance;
}

//Returns the view matrix of the camera
XMMATRIX Camera::GetViewMatrix()
{
	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	m_lookat = XMVectorSet(m_x + m_dx, m_y, m_z + m_dz, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	XMMATRIX view = XMMatrixLookAtLH(m_position, m_lookat, m_up);

	return view;
}
//Returns a view matrix with the camera looking at a given position
XMMATRIX Camera::LookAt(float x, float y, float z)
{
	m_x = lookatX; 
	m_y = lookatY;
	m_z = lookatZ;  
	

	m_position = XMVectorSet(m_x, m_y, m_z, 0.0);
	m_lookat = XMVectorSet(x, y, z + 5, 0.0);
	m_up = XMVectorSet(0.0, 1.0, 0.0, 0.0);

	XMMATRIX view = XMMatrixLookAtLH(m_position, m_lookat, m_up);

	return view;
}