#pragma once
#include "objfilemodel.h"
#include "DXSetup.h"

struct MODEL_CONSTANT_BUFFER
{
	XMMATRIX WorldViewProject;			//64
	XMVECTOR directional_light_vector;	//16
	XMVECTOR directional_light_colour;	//16
	XMVECTOR ambient_light_colour;		//16
}; //						  Total Size : 112 bytes = 16 x 7

class Model
{
public:
	Model(DXSetup* p_dx, char* objFile, char* textureFile);
	~Model();
	HRESULT LoadObjModel(char* filename);
	void Draw(XMMATRIX* view, XMMATRIX* projection, bool isLit);
	void AddTexture(char* filename);
	void LookAt(float targetX, float targetY, float targetZ);
	void MoveForward(float distance);

	//Setters
	void SetPos(float x, float y, float z) { m_x = x; m_y = y; m_z = z; }
	void SetX(float num) { m_x = num; }
	void SetY(float num) { m_y = num; }
	void SetZ(float num) { m_z = num; }
	void SetRotX(float num) { m_xAngle = num; }
	void SetRotY(float num) { m_yAngle = num; }
	void SetRotZ(float num) { m_zAngle = num; }
	void SetScale(float num) { m_scale = num; }
	//Getters
	float GetX() { return m_x; }
	float GetY() { return m_y; }
	float GetZ() { return m_z; }
	float GetRotX() { return m_xAngle; }
	float GetRotY() { return m_yAngle; }
	float GetRotZ() { return m_zAngle; }
	float GetScale() { return m_scale; }
	//Incrementers --- Decrements if amount is negative
	void IncrementXpos(float amount) { m_x += amount; }
	void IncrementYpos(float amount) { m_y += amount; }
	void IncrementZpos(float amount) { m_z += amount; }
	void IncrementXrot(float amount) { m_xAngle += amount; }
	void IncrementYrot(float amount) { m_yAngle += amount; }
	void IncrementZrot(float amount) { m_zAngle += amount; }
	void IncrementScale(float amount) { m_scale += amount; }

	bool CheckCollision(Model* model);

private:
	DXSetup* p_dx;

	void CalculateModelCentrePoint();
	void CalculateBoundingSphereRadius();
	XMVECTOR GetBoundingSphereWorldSpacePosition();
	float GetBoundingSphereRadius();
	ID3D11Device* m_pD3DDevice = NULL;
	ID3D11DeviceContext* m_pImmediateContext = NULL;

	ObjFileModel* m_pObject = nullptr;
	ID3D11VertexShader* m_pVShader = NULL;
	ID3D11PixelShader* m_pPShader = NULL;
	ID3D11InputLayout* m_pInputLayout = NULL;
	ID3D11Buffer* m_pConstantBuffer = NULL;

	float m_x = 0, m_y = 0, m_z = 0;
	float m_xAngle = 0, m_yAngle = 0, m_zAngle = 0;
	float m_scale = 1;

	//texture
	ID3D11ShaderResourceView* m_pTexture = NULL;
	ID3D11SamplerState* m_pSampler = NULL;

	//lighting 
	XMVECTOR g_directional_light_shines_from = XMVectorSet(-1.0f, 0.0f, 0.0f, 0.0f); //shines down the +Z axis
	XMVECTOR g_directional_light_colour = XMVectorSet(0.6f, 0.6f, 0.9f, 0.0f); // slightly blue
	XMVECTOR g_ambient_light_colour = XMVectorSet(0.35f, 0.35f, 0.35f, 1.0f); // dark grey
	XMVECTOR g_ambient_overly_lit = XMVectorSet(2.0f, 2.0f, 2.0f, 2.0f);
	//Collision
	float m_bounding_sphere_centre_x = 0;
	float m_bounding_sphere_centre_y = 0;
	float m_bounding_sphere_centre_z = 0;
	float m_bounding_sphere_radius = 0;
};

