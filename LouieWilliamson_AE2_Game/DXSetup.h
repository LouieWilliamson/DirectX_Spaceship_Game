#pragma once
#include <d3d11.h> 
#include <dxgi.h> 
#include <D3DX11.h>
#include <windows.h>
#include <DxErr.h>

#define _XM_NO_INTRINSICS_
#define XM_NO_ALIGNMENT

class DXSetup
{
public:
	DXSetup();
	~DXSetup();
	HRESULT Initialise(HWND window);
	ID3D11Device* device = NULL;
	ID3D11DeviceContext* ImmediateContext = NULL;
	
	D3D_DRIVER_TYPE g_driverType = D3D_DRIVER_TYPE_NULL;
	D3D_FEATURE_LEVEL g_featureLevel = D3D_FEATURE_LEVEL_11_0;
	IDXGISwapChain* swapChain = NULL;
	ID3D11RasterizerState* rastStateCullNone = NULL;
	ID3D11RasterizerState* rastStateCullBack = NULL;
	ID3D11RenderTargetView* BackBufferRTView = NULL;
	ID3D11DepthStencilView* ZBuffer = NULL;
	ID3D11BlendState* AlphaBlendEnable = NULL;
	ID3D11BlendState* AlphaBlendDisable = NULL;

};

