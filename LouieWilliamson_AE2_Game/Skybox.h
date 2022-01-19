#include <d3d11.h> 
#include <dxgi.h> 
#include <D3DX11.h>
#include <windows.h>
#include <DxErr.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

#include <DirectXMath.h>
#include "DXSetup.h"
using namespace DirectX;

struct POS_COL_TEX_VERTEX
{
	XMFLOAT3	pos;
	XMFLOAT4	Col;
	XMFLOAT2	Texture0;
};

struct CONSTANT_BUFFER1
{
	XMMATRIX WorldViewProjection; //64 bytes (4 x 4 = 16 floats | 16 x 4 = 64 bytes)
	float RedAmount; // 4 bytes
	float Scale;	// 4 bytes
	XMFLOAT2 packing_bytes; // 2 x 4 bytes = 8 bytes
}; // Total size = 80 bytes

class Skybox
{
public:
	Skybox(DXSetup* p_dx);
	~Skybox();
	HRESULT Initialise();
	void Draw(XMMATRIX* view, XMMATRIX* projection, float cameraX, float cameraY, float cameraZ);
private:
	ID3D11Device*				m_device = NULL;
	ID3D11DeviceContext*		m_device_context = NULL;

	ID3D11Buffer*				m_pVertexBuffer = NULL;
	ID3D11VertexShader*			m_pVertexShader = NULL;
	ID3D11PixelShader*			m_pPixelShader = NULL;
	ID3D11InputLayout*			m_pInputLayout = NULL;
	ID3D11Buffer*				m_pConstantBuffer0 = NULL;
	ID3D11ShaderResourceView*	m_pTexture0 = NULL;
	ID3D11SamplerState*			m_pSampler0 = NULL;
	CONSTANT_BUFFER1			cbuffer;

	ID3D11RasterizerState*		rasterStateCullNone = NULL;
	ID3D11RasterizerState*		rasterStateCullBack = NULL;

	ID3D11RasterizerState*		m_pRasterSolid = 0;
	ID3D11RasterizerState*		m_pRasterSkybox = 0;
	ID3D11DepthStencilState*	m_pDepthWriteSolid = 0;
	ID3D11DepthStencilState*	m_pDepthWriteSkybox = 0;
};

