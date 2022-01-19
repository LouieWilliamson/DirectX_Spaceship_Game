#include "Model.h"


Model::Model(DXSetup* p_dx, char* objFile, char* textureFile)
{
	m_pD3DDevice = p_dx->device;
	m_pImmediateContext = p_dx->ImmediateContext;

	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
	m_xAngle = 0.0f;
	m_yAngle = 0.0f;
	m_zAngle = 0.0f;

	m_scale = 1.0f;

	//Loads the given obj file and applies the given texture
	LoadObjModel(objFile);
	AddTexture(textureFile);
}
//Cleans up pointers
Model::~Model()
{
	if (m_pObject)
	{
		delete m_pObject;
		m_pObject = nullptr;
	}
	m_pTexture->Release();
	m_pSampler->Release();
	m_pVShader->Release();
	m_pPShader->Release();
	m_pInputLayout->Release();
	m_pConstantBuffer->Release();
}

//Loads the obj file, compiles the shaders and creates the input layout and constant buffer
HRESULT Model::LoadObjModel(char* filename)
{
	HRESULT hr = S_OK;

	m_pObject = new ObjFileModel(filename, m_pD3DDevice, m_pImmediateContext);

	if (m_pObject->filename == "FILE NOT LOADED") return S_FALSE;

	//Load and compile the pixel and vertex shaders - use vs_5_0 to target DX11 hardware only
	ID3DBlob* VS, * PS, * error;

	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelVS", "vs_4_0", 0, 0, 0, &VS, &error, 0);

	if (error != 0) //Check for shader compilation error
	{
		OutputDebugStringA((char*)error->GetBufferPointer());
		error->Release();
		if (FAILED(hr))//Don't fail if error is just a warning
		{
			return hr;
		}
	}

	hr = D3DX11CompileFromFile("model_shaders.hlsl", 0, 0, "ModelPS", "ps_4_0", 0, 0, 0, &PS, &error, 0);

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
	hr = m_pD3DDevice->CreateVertexShader(VS->GetBufferPointer(), VS->GetBufferSize(), NULL, &m_pVShader);

	if (FAILED(hr))
	{
		return hr;
	}

	hr = m_pD3DDevice->CreatePixelShader(PS->GetBufferPointer(), PS->GetBufferSize(), NULL, &m_pPShader);

	if (FAILED(hr))
	{
		return hr;
	}

	//Create and set the input layout object
	D3D11_INPUT_ELEMENT_DESC iedesc[] =
	{
		//Be very careful setting the correct dxgi format and D3D version
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD",0,DXGI_FORMAT_R32G32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0},
		{"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,D3D11_APPEND_ALIGNED_ELEMENT,D3D11_INPUT_PER_VERTEX_DATA,0}
	};

	hr = m_pD3DDevice->CreateInputLayout(iedesc, ARRAYSIZE(iedesc), VS->GetBufferPointer(), VS->GetBufferSize(), &m_pInputLayout);
	if (FAILED(hr))
	{
		return hr;
	}

	//Create constant buffer
	D3D11_BUFFER_DESC constant_buffer_desc;
	ZeroMemory(&constant_buffer_desc, sizeof(constant_buffer_desc));

	constant_buffer_desc.Usage = D3D11_USAGE_DEFAULT; // Can use UpdateSubresource() to update
	constant_buffer_desc.ByteWidth = 112; // MUST be a multiple of 16, calculate from constant buffer struct
	constant_buffer_desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER; // Use as a constant buffer

	hr = m_pD3DDevice->CreateBuffer(&constant_buffer_desc, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	//Used for bounding sphere collision
	CalculateModelCentrePoint();
	CalculateBoundingSphereRadius();

	return hr;
}
//Applies all transformations and lighting and draws the object.
void Model::Draw(XMMATRIX* view, XMMATRIX* projection, bool isLit)
{
	// ----------------------- WORLD MATRIX----------------------
	XMMATRIX world;
	
	world = XMMatrixScaling(m_scale, m_scale, m_scale);
	world *= XMMatrixRotationX(m_xAngle);
	world *= XMMatrixRotationY(m_yAngle);
	world *= XMMatrixRotationZ(m_zAngle);
	world *= XMMatrixTranslation(m_x, m_y, m_z);
	//------------------------ LIGHTING ------------------------------
	XMMATRIX transpose;
	MODEL_CONSTANT_BUFFER model_cb_values;
	transpose = XMMatrixTranspose(world);
	model_cb_values.directional_light_colour = g_directional_light_colour;

	//if object is affected by lighting
	if (isLit)
	{
		model_cb_values.ambient_light_colour = g_ambient_light_colour;
	}
	//if not, apply overly lit ambient to hide directional light (this is used for bullets)
	else
	{
		model_cb_values.ambient_light_colour = g_ambient_overly_lit;
	}

	model_cb_values.directional_light_vector = XMVector3Transform(g_directional_light_shines_from, transpose);
	model_cb_values.directional_light_vector = XMVector3Normalize(model_cb_values.directional_light_vector);
	//---------------------------------
	m_pImmediateContext->VSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	m_pImmediateContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);

	m_pImmediateContext->VSSetShader(m_pVShader, 0, 0);
	m_pImmediateContext->PSSetShader(m_pPShader, 0, 0);

	m_pImmediateContext->IASetInputLayout(m_pInputLayout);

	model_cb_values.WorldViewProject = world*(*view)*(*projection);

	m_pImmediateContext->UpdateSubresource(m_pConstantBuffer, 0, 0, &model_cb_values, 0, 0);
	

	m_pImmediateContext->PSSetSamplers(0, 1, &m_pSampler);
	m_pImmediateContext->PSSetShaderResources(0, 1, &m_pTexture);

	m_pObject->Draw();
}


//Creates texture and sampler state
void Model::AddTexture(char* filename)
{
	D3D11_SAMPLER_DESC sampler_desc;

	ZeroMemory(&sampler_desc, sizeof(sampler_desc));
	sampler_desc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampler_desc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampler_desc.MaxLOD = D3D11_FLOAT32_MAX;

	m_pD3DDevice->CreateSamplerState(&sampler_desc, &m_pSampler);

	D3DX11CreateShaderResourceViewFromFile(m_pD3DDevice, filename, NULL, NULL, &m_pTexture, NULL);
}

//Looks at a given position
void Model::LookAt(float targetX, float targetY, float targetZ)
{
	float dx = targetX - m_x;
	float dy = targetY - m_y;
	float dz = targetZ - m_z;
	float dxz = sqrt(targetX * targetX + targetZ * targetZ);
	
	SetRotX(-atan2(dy, dxz));
	SetRotY(atan2(dx, dz)); //I did have "* (180.0 / XM_PI)" here but converting to/from radians seems to break it

}

//Moves in the direction it is look (backwards if negative)
void Model::MoveForward(float distance)
{
	m_x += sin(m_yAngle) * distance;//(XM_PI / 180.0)) * distance;
	m_y += -sin(m_xAngle) * distance;
	m_z += cos(m_yAngle) * distance;//(XM_PI / 180.0)) * distance;
}

//Calculate the centre point of the model to be used for bounding sphere collisions
void Model::CalculateModelCentrePoint()
{
	float minX = 0.0f;
	float minY = 0.0f;
	float minZ = 0.0f;

	float maxX = 0.0f;
	float maxY = 0.0f;
	float maxZ = 0.0f;

	for (int i = 0; i < m_pObject->numverts; i++)
	{
		//minimum vert positions
		if (m_pObject->vertices[i].Pos.x <= minX) minX = m_pObject->vertices[i].Pos.x;
		if (m_pObject->vertices[i].Pos.y <= minY) minY = m_pObject->vertices[i].Pos.y;
		if (m_pObject->vertices[i].Pos.z <= minZ) minZ = m_pObject->vertices[i].Pos.z;
		//maximum vert positions
		if (m_pObject->vertices[i].Pos.x >= maxX) maxX = m_pObject->vertices[i].Pos.x;
		if (m_pObject->vertices[i].Pos.y >= maxY) maxY = m_pObject->vertices[i].Pos.y;
		if (m_pObject->vertices[i].Pos.z >= maxZ) maxZ = m_pObject->vertices[i].Pos.z;
	}
								 //midpoints
	m_bounding_sphere_centre_x = (maxX + minX) / 2;
	m_bounding_sphere_centre_y = (maxY + minY) / 2;
	m_bounding_sphere_centre_z = (maxZ + minZ) / 2;
}
//Calculates the minimum circle radius which encapsulates all verts of the object
void Model::CalculateBoundingSphereRadius()
{
	float x = 0.0f;
	float y = 0.0f;
	float z = 0.0f;

	float highest = 0.0f;
	float distance;

	//Loop through the object verts and use pythagoras to find the distance from the centre point
	for (int i = 0; i < m_pObject->numverts; i++)
	{
		x = m_pObject->vertices[i].Pos.x;
		y = m_pObject->vertices[i].Pos.y;
		z = m_pObject->vertices[i].Pos.z;

		//distance = (x1 - x2)²							   + (y1 - y2)²								+ (z1 - z2)²
		distance = (pow(x - m_bounding_sphere_centre_x, 2) + pow(y - m_bounding_sphere_centre_y, 2) + pow(z - m_bounding_sphere_centre_z, 2));
		if (distance > highest)
		{
			highest = distance;
		}
	}

	m_bounding_sphere_radius = sqrt(highest);
}
//Returns a XMVECTOR of the world position of the bounding sphere
XMVECTOR Model::GetBoundingSphereWorldSpacePosition()
{
	XMMATRIX world;

	world = XMMatrixScaling(m_scale, m_scale, m_scale);

	world *= XMMatrixRotationX(m_xAngle);
	world *= XMMatrixRotationY(m_yAngle);
	world *= XMMatrixRotationZ(m_zAngle);
	world *= XMMatrixTranslation(m_x, m_y, m_z);

	XMVECTOR offset = XMVectorSet(m_bounding_sphere_centre_x, m_bounding_sphere_centre_y, m_bounding_sphere_centre_z, 0.0f);
	offset = XMVector3Transform(offset, world);

	return offset;
}
//Returns the bounding sphere radius
float Model::GetBoundingSphereRadius()
{
	float radius = m_bounding_sphere_radius * m_scale;

	return radius;
}
//Checks collision with another given model. Returns true if they've collided.
bool Model::CheckCollision(Model* model)
{
	bool collided = false;
	XMVECTOR thisCentrePos;
	XMVECTOR targetCentrePos;

	if (model == this)
	{
		return false;
	}

	thisCentrePos = GetBoundingSphereWorldSpacePosition();
	targetCentrePos = model->GetBoundingSphereWorldSpacePosition();
	//this obj
	float x1 = XMVectorGetX(thisCentrePos);
	float y1 = XMVectorGetY(thisCentrePos);
	float z1 = XMVectorGetZ(thisCentrePos);
	//target obj
	float x2 = XMVectorGetX(targetCentrePos);
	float y2 = XMVectorGetY(targetCentrePos);
	float z2 = XMVectorGetZ(targetCentrePos);

	//uses pythagoras to work out distance sqrd between the 2 objects
	float distanceSqrd = (pow(x1 - x2, 2) + pow(y1 - y2, 2) + pow(z1 -z2, 2));

	float thisRadius = GetBoundingSphereRadius();
	float targetRadius = model->GetBoundingSphereRadius();

	//if distancesqrd is less than the sum of the two radius' sqrd then they have collided
	if (distanceSqrd < pow(thisRadius + targetRadius, 2)) { collided = true; }

	return collided;
}