#include "Skybox.h"



Skybox::Skybox(DXSetup* p_dx)
{
	m_device = p_dx->device;
	m_device_context = p_dx->ImmediateContext;
	rasterStateCullBack = p_dx->rastStateCullBack;
	rasterStateCullNone = p_dx->rastStateCullNone;

	cbuffer.RedAmount = 1.0f;
	cbuffer.Scale = 1.0f;
}

//Cleans up pointers
Skybox::~Skybox()
{
	if (m_pRasterSolid)	m_pRasterSolid->Release();
	if (m_pRasterSkybox) m_pRasterSkybox->Release();
	if (m_pDepthWriteSkybox) m_pDepthWriteSkybox->Release();
	if (m_pDepthWriteSolid) m_pDepthWriteSolid->Release();
	
	m_pVertexBuffer->Release();
	m_pVertexShader->Release();
	m_pPixelShader->Release();
	m_pInputLayout->Release();
	m_pConstantBuffer0->Release();
	m_pTexture0->Release();
	m_pSampler0->Release();
}

//Initalises verts, buffers, shaders, input layout and texture for the skybox
HRESULT Skybox::Initialise()
{
	HRESULT hr = S_OK;

	POS_COL_TEX_VERTEX vertices[] =
	{
		//back face
		{XMFLOAT3(-1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f)},

		//front face
		{XMFLOAT3(-1.0f, -1.0f, -1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f ,-1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, -1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f)},

		//left face
		{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f)},

		//right face
		{XMFLOAT3(1.0f, -1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, -1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f)},

		//bottom face
		{XMFLOAT3(1.0f, -1.0f, -1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(1.0f, -1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-1.0f, -1.0f, -1.0f), XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},

		//top face
		{XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, -1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-1.0f, 1.0f, 1.0f),	XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f),	XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(1.0f, 1.0f, 1.0f),	XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f),	XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-1.0f, 1.0f, -1.0f),	XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f),	XMFLOAT2(0.0f, 1.0f)}
	};

	//Set up and create vertex buffer
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	bufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	bufferDesc.ByteWidth = sizeof(vertices);
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	hr = m_device->CreateBuffer(&bufferDesc, NULL, &m_pVertexBuffer);
	//Create the buffer

	if (FAILED(hr))//return an error code if failed
	{
		return hr;
	}

	//Create constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // Can use UpdateSubresource() to update
	constant_buffer_desc.ByteWidth = 80; // MUST be a multiple of 16, calculate from constant buffer struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as a constant buffer

	hr = m_device->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer0);
	if (FAILED(hr))
	{
		return hr;
	}

	//Copy the vertices into the buffer
	D3D11_MAPPED_SUBRESOURCE ms;

	//Lock the buffer to allow writing 
	m_device_context->Map(m_pVertexBuffer, NULL, D3D11_MAP_WRITE_DISCARD, NULL, &ms);

	//Copy the data
	memcpy(ms.pData, vertices, sizeof(vertices));

	//Unlock the buffer
	m_device_context->Unmap(m_pVertexBuffer, NULL);

	//Load and compile the pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob* VS, * PS, * error;

	hr = D3DX11CompileFromFile("sky_shader.hlsl", 0, 0, "VShader", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0) //Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//Don't fail if error is just a warning
		{
			return hr;
		}
	}

	hr = D3DX11CompileFromFile("sky_shader.hlsl", 0, 0, "PShader", "ps_4_0", 0, 0, 0, &PS, &error, 0);

	if (error != 0) //check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//Don't fail if error is just a warning
		{
			return hr;
		}
	}

	//Create shader objects
	hr = m_device->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVertexShader);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_device->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPixelShader);

	if (FAILED(hr))
	{
		return hr;
	}

	//Set the shader objects as active
	m_device_context->VSSetShader(m_pVertexShader, 0, 0);
	m_device_context->PSSetShader(m_pPixelShader, 0, 0);

	//Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		//Be very careful setting the correct dxgi format and D3D version
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 }
	};

	hr = m_device->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);

	if (FAILED(hr))
	{
		return hr;
	}

	m_device_context->IASetInputLayout(m_pInputLayout);

	D3DX11CreateShaderResourceViewFromFile(m_device, "assets/Skybox/skybox_space2.dds", NULL, NULL, &m_pTexture0, NULL);

	D3D11_SAMPLER_DESC sampler_desc;
	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	m_device->CreateSamplerState(&sampler_desc, &m_pSampler0);
	//sky box 
	D3D11_RASTERIZER_DESC rasterizer_desc;
	ZeroMemory(&rasterizer_desc, sizeof(rasterizer_desc));

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_BACK;
	hr = m_device->CreateRasterizerState(&rasterizer_desc, &m_pRasterSolid);

	rasterizer_desc.FillMode = D3D11_FILL_SOLID;
	rasterizer_desc.CullMode = D3D11_CULL_FRONT;
	hr = m_device->CreateRasterizerState(&rasterizer_desc, &m_pRasterSkybox);

	//depth stencil
	D3D11_DEPTH_STENCIL_DESC DSDecsc;
	ZeroMemory(&DSDecsc, sizeof(DSDecsc));
	DSDecsc.DepthEnable = true;
	DSDecsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	DSDecsc.DepthFunc = D3D11_COMPARISON_LESS;
	hr = m_device->CreateDepthStencilState(&DSDecsc, &m_pDepthWriteSolid);
	DSDecsc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO;
	hr = m_device->CreateDepthStencilState(&DSDecsc, &m_pDepthWriteSkybox);

	return S_OK;
}

//Draws the skybox and moves it to the cameras position
void Skybox::Draw(XMMATRIX* view, XMMATRIX* projection, float cameraX, float cameraY, float cameraZ)
{
	XMMATRIX world;
	
	//Set vertex buffer
	UINT stride = sizeof(POS_COL_TEX_VERTEX);
	UINT offset = 0;

	m_device_context->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);
	m_device_context->VSSetConstantBuffers(0, 1, &m_pConstantBuffer0);
	m_device_context->VSSetShader(m_pVertexShader, 0, 0);
	m_device_context->PSSetShader(m_pPixelShader, 0, 0);
	m_device_context->IASetInputLayout(m_pInputLayout);

	world = XMMatrixScaling(5.0f, 5.0f, 5.0f);
	world *= XMMatrixTranslation(cameraX, cameraY, cameraZ);

	cbuffer.WorldViewProjection = world * (*view) * (*projection);

	m_device_context->UpdateSubresource(m_pConstantBuffer0, 0, 0, &cbuffer, 0, 0);

	m_device_context->PSSetSamplers(0, 1, &m_pSampler0);
	m_device_context->PSSetShaderResources(0, 1, &m_pTexture0);

	//Set depth stencils so it renders behind all objects
	m_device_context->OMSetDepthStencilState(m_pDepthWriteSkybox, 1);
	m_device_context->RSSetState(m_pRasterSkybox);
	m_device_context->Draw(36, 0);
	m_device_context->RSSetState(m_pRasterSolid);
	m_device_context->OMSetDepthStencilState(m_pDepthWriteSolid, 1);
}
