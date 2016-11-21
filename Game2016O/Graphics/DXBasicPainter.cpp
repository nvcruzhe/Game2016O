#include "stdafx.h"
#include "DXBasicPainter.h"


CDXBasicPainter::CDXBasicPainter(CDXManager* pOwner)
{
	m_pManager = pOwner;
	m_pIL = NULL;
	m_pVS = NULL;
	m_pPS = NULL;
	m_pCB = NULL;
	m_pRTV = NULL;
	m_Params.World = m_Params.View = m_Params.Projection = Identity();
	VECTOR4D Zero = { 0, 0, 0, 0 };
	m_Params.Brightness = Zero;
	/*MATERIAL MatDef = {
		{ 1, 1, 1, 1 }, { 1, 1, 1, 1 }, { 1, 1, 1, 1 },
		{ 0, 0, 0, 0 }, { 60, 0, 0, 0 }};
	m_Params.Material = MatDef;
	memset(m_Params.Lights, 0, sizeof(m_Params.Lights));
	LIGHT LightDef = { { LIGHT_ON, LIGHT_DIRECTIONAL, 0, 0 },
	{ 0.1, 0.1, 0.1, 0 }, //Abient
	{ 1, 1, 1, 0 },      //Diffuse
	{ 1, 1, 1, 0 },       //Specular
	{ 1, 0, 0, 0 },      //Attenuation
	{ 0, 0, 0, 1 },      //Position
	{ 0, -1, 0, 0 },//{ -0.5773f, -0.5773f, -0.5773f, 0 },
	{ 1, 0, 0, 0 } };
	m_Params.Lights[0] = LightDef;
	m_pDrawLH = NULL;
	m_pDrawRH = NULL;*/



	MATERIAL MatDef = {
		{ 1,1,1,1 },  // Ambient
		{ 1,1,1,1 },	// Diffuse
		{ 1,1,1,1 },	// Specular
		{ 0 ,0,0,0 },	// Emissive
		{ 100,0,0,0 }  // Power
	};
	m_Params.Material = MatDef;
	memset(m_Params.Lights, 0, sizeof(m_Params.Lights));

	LIGHT LightDef = {
		{ LIGHT_ON, LIGHT_DIRECTIONAL,0,0 },
		{ 0.1,0.1,0.1,0 },
		{ 0.5,0.5,0.5,0.5 },
		{ 1,1,1,0 },
		{ 1,0,0,0 },
		{ 0,0,0,1 },
		{ 0,0,-1,0 },
		{ 1,0,0,0 }
	};

	LIGHT LightDef2 = {
		{ LIGHT_ON, LIGHT_SPOT,0,0 },  // Flags
		{ 0.1,0.1,0.1,0 },				// Ambient
		{ 5,5,5,5 },			// Diffuse
		{ 5,5,5,0 },					// Specular
		{ 1,.05,0.05,0 },					// Attenuation
		{ 0,0,1,1 },					// Position
		{ 0,0,0,0 },					// Direction
		{ 30,0,0,0 }						// Factors
	};

	LIGHT LightDef3 = {
		{ LIGHT_ON, LIGHT_POINT,0,0 },	// Flags
		{ 0.1,0.1,0.1,0 },				// Ambient
		{ 1,1,1,1 },					// Diffuse
		{ 1,1,0.7,0 },					// Specular
		{ 1,0,0,0 },					// Attenuation
		{ 3,3,3,1 },					// Position
		{ -.5773,-.5773,-.5773,0 },		// Direction
		{ 30,0,0,0 }						// Factors
	};

	LIGHT LightDef4 = {
		{ 0, LIGHT_POINT,0,0 },	// Flags
		{ 0.1,0.1,0.1,0 },				// Ambient
		{ 1,1,1,1 },					// Diffuse
		{ 1,1,0.7,0 },					// Specular
		{ 1,0,0,0 },					// Attenuation
		{ 3,3,-3,1 },					// Position
		{ -.5773,-.5773,.5773,0 },		// Direction
		{ 30,0,0,0 }						// Factors
	};

	LIGHT LightDef5 = {
		{ 0, LIGHT_POINT,0,0 },	// Flags
		{ 0.1,0.1,0.1,0 },				// Ambient
		{ 1,1,1,1 },					// Diffuse
		{ 1,1,0.7,0 },					// Specular
		{ 1,0,0,0 },					// Attenuation
		{ 3,-3,3,1 },					// Position
		{ -.5773,.5773,-.5773,0 },		// Direction
		{ 30,0,0,0 }						// Factors
	};

	m_Params.Lights[0] = LightDef;
	m_Params.Lights[1] = LightDef2;
	m_Params.Lights[2] = LightDef3;
	m_Params.Lights[3] = LightDef4;
	m_Params.Lights[4] = LightDef5;
}

void CDXBasicPainter::Uninitialize()
{
	SAFE_RELEASE(m_pIL);
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pPS);
	SAFE_RELEASE(m_pCB);
	SAFE_RELEASE(m_pDrawLH);
	SAFE_RELEASE(m_pDrawRH);
}

D3D11_INPUT_ELEMENT_DESC CDXBasicPainter::VERTEX::InputLayout[] =
{ 
	{"POSITION",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 }, 
	{"NORMAL",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0,16,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"NORMAL",1,DXGI_FORMAT_R32G32B32A32_FLOAT,0,32,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"NORMAL",2,DXGI_FORMAT_R32G32B32A32_FLOAT,0,48,D3D11_INPUT_PER_VERTEX_DATA,0},
	{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 64, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	{"TEXCOORD", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 80, D3D11_INPUT_PER_VERTEX_DATA, 0 }
};

bool CDXBasicPainter::Initialize()
{
	Uninitialize();
	ID3D10Blob* pVSCode = NULL;
	m_pVS=m_pManager->CompileVertexShader(
		L"..\\Shaders\\BasicShader.hlsl", "VSMain",&pVSCode);
	if (!m_pVS)
	{
		SAFE_RELEASE(pVSCode);
		return false;
	}
	HRESULT hr=
	m_pManager->GetDevice()->CreateInputLayout(
		VERTEX::InputLayout,
		sizeof(VERTEX::InputLayout) / sizeof(D3D11_INPUT_ELEMENT_DESC),
		pVSCode->GetBufferPointer(), pVSCode->GetBufferSize(), &m_pIL);
	SAFE_RELEASE(pVSCode);
	if (FAILED(hr))
	{
		SAFE_RELEASE(m_pVS);
		return false;
	}
	m_pPS = m_pManager->CompilePixelShader(
		L"..\\Shaders\\BasicShader.hlsl", "PSMain");
	if (!m_pPS)
	{
		SAFE_RELEASE(m_pVS);
		SAFE_RELEASE(m_pIL);
		return false;
	}
	m_pVSShadow=m_pManager->CompileVertexShader(L"..\\Shaders\\BasicShader.hlsl","VSShadow", &pVSCode);
	SAFE_RELEASE(pVSCode);
	m_pPSShadow=m_pManager->CompilePixelShader(L"..\\Shaders\\BasicShader.hlsl", "PSShadow");
	D3D11_BUFFER_DESC dbd;
	memset(&dbd, 0, sizeof(dbd));
	dbd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	dbd.ByteWidth = 16 * ((sizeof(PARAMS) + 15) / 16);
	dbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	dbd.Usage = D3D11_USAGE_DYNAMIC;
	m_pManager->GetDevice()->CreateBuffer(&dbd, 0, &m_pCB);
	//Creaci�n de los Estados de Rasterizador para dibujo en mano izquierda y mano derecha
	D3D11_RASTERIZER_DESC drd;
	memset(&drd, 0, sizeof(drd));
	drd.AntialiasedLineEnable = false;
	drd.CullMode = D3D11_CULL_BACK;
	drd.DepthBias = 0.0f;
	drd.DepthBiasClamp = 0.0f;
	drd.FillMode = D3D11_FILL_SOLID;
	drd.FrontCounterClockwise = FALSE;
	drd.MultisampleEnable = FALSE;
	drd.ScissorEnable = FALSE;
	drd.SlopeScaledDepthBias = 0.0f;
	m_pManager->GetDevice()->CreateRasterizerState(&drd, &m_pDrawLH);
	drd.CullMode = D3D11_CULL_FRONT;
	m_pManager->GetDevice()->CreateRasterizerState(&drd, &m_pDrawRH);
	//Creaci�n de estados de profundidad/stencil Dibujar / Marcar / Dibujar en Marcado
	D3D11_DEPTH_STENCIL_DESC ddsd;
	ddsd.DepthEnable = TRUE;
	ddsd.StencilEnable = TRUE;
	ddsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	ddsd.DepthFunc = D3D11_COMPARISON_LESS;
	ddsd.StencilReadMask = D3D11_DEFAULT_STENCIL_READ_MASK;
	ddsd.StencilWriteMask = D3D11_DEFAULT_STENCIL_WRITE_MASK;
	ddsd.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	ddsd.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	ddsd.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	ddsd.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	ddsd.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	ddsd.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;
	ddsd.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	ddsd.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	m_pManager->GetDevice()->CreateDepthStencilState(&ddsd, &m_pDSSMark);
	ddsd.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	ddsd.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	m_pManager->GetDevice()->CreateDepthStencilState(&ddsd, &m_pDSSDrawOnMark);
	ddsd.StencilEnable = false;
	m_pManager->GetDevice()->CreateDepthStencilState(&ddsd, &m_pDSSDraw);

	//Recursos para sombras
	ID3D11Texture2D* pMemory = NULL;
	D3D11_TEXTURE2D_DESC dtd;
	memset(&dtd, 0, sizeof(dtd));
	dtd.ArraySize = 1;
	dtd.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	dtd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dtd.MipLevels = 1;
	dtd.SampleDesc.Count = 1;
	dtd.Usage = D3D11_USAGE_DEFAULT;
	dtd.Width = SHADOW_MAP_RESOLUTION;
	dtd.Height = SHADOW_MAP_RESOLUTION;
	m_pManager->GetDevice()->CreateTexture2D(&dtd, NULL, &pMemory);
	m_pManager->GetDevice()->CreateDepthStencilView(
		pMemory, NULL, &m_pDSVShadowMap);
	SAFE_RELEASE(pMemory);
	dtd.Format = DXGI_FORMAT_R16_UNORM;
	dtd.BindFlags =
		D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE;
	m_pManager->GetDevice()->CreateTexture2D(&dtd, NULL, &pMemory);
	m_pManager->GetDevice()->CreateShaderResourceView(
		pMemory, NULL, &m_pSRVShadowMap);
	m_pManager->GetDevice()->CreateRenderTargetView(
		pMemory, NULL, &m_pRTVShadowMap);
	SAFE_RELEASE(pMemory);
	return true;
}
CDXBasicPainter::~CDXBasicPainter()
{
	Uninitialize();
}

void CDXBasicPainter::ClearShadow()
{
	m_pManager->GetContext()->ClearDepthStencilView(
		m_pDSVShadowMap, D3D11_CLEAR_DEPTH, 1.0f,0);
	VECTOR4D One = { 1, 1, 1, 1 };
	m_pManager->GetContext()->ClearRenderTargetView(
		m_pRTVShadowMap, (float*)&One);
}

void CDXBasicPainter::DrawIndexed(VERTEX* pVertices, unsigned long nVertices,
	unsigned long* pIndices, unsigned long nIndices,bool bShadow)
{
	//1.- Crear los buffer de v�rtices e indices en el GPU.
	ID3D11Buffer  *pVB = NULL, *pIB = NULL;
	D3D11_BUFFER_DESC dbd;
	memset(&dbd, 0, sizeof(dbd));
	dbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	dbd.ByteWidth = sizeof(VERTEX)*nVertices;
	dbd.CPUAccessFlags = 0;
	/*
	D3D11_USAGE_DEFAULT GPU:R/W CPU:None
	D3D11_USAGE_DYNAMIC GPU:R   CPU:W
	D3D11_USAGE_IMMUTABLE GPU:R CPU:W once
	D3D11_USAGE_STAGING   GPU:None CPU:W/R
	*/
	dbd.Usage = D3D11_USAGE_IMMUTABLE;
	D3D11_SUBRESOURCE_DATA dsd;
	dsd.pSysMem = pVertices;
	dsd.SysMemPitch = 0;
	dsd.SysMemSlicePitch = 0;
	m_pManager->GetDevice()->CreateBuffer(
		&dbd, &dsd, &pVB);
	dsd.pSysMem = pIndices;
	dbd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	dbd.ByteWidth = sizeof(unsigned long)*nIndices;
	m_pManager->GetDevice()->CreateBuffer(
		&dbd, &dsd, &pIB);
	//2.- Instalar el VS , PS , IL
	m_pManager->GetContext()->IASetInputLayout(m_pIL);
	m_pManager->GetContext()->VSSetShader(bShadow?m_pVSShadow:m_pVS, 0, 0);
	m_pManager->GetContext()->PSSetShader(bShadow?m_pPSShadow:m_pPS, 0, 0);
	//3.- Definir el puerto de visi�n y la topologia
	// a dibujar
	D3D11_VIEWPORT ViewPort;
	ID3D11Texture2D* pBackBuffer=NULL;
	D3D11_TEXTURE2D_DESC dtd;
	if (bShadow)
		m_pRTVShadowMap->GetResource((ID3D11Resource**)&pBackBuffer);
	else
		m_pRTV->GetResource((ID3D11Resource**)&pBackBuffer);
	pBackBuffer->GetDesc(&dtd);
	ViewPort.Width = (float)dtd.Width;
	ViewPort.Height = (float)dtd.Height;
	ViewPort.TopLeftX = 0;
	ViewPort.TopLeftY = 0;
	ViewPort.MaxDepth = 1.0f;
	ViewPort.MinDepth = 0.0f;
	m_pManager->GetContext()->RSSetViewports(1, &ViewPort);
	m_pManager->GetContext()->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//4.- Configurar la salida
	m_pManager->GetContext()->OMSetRenderTargets(1,
		bShadow?&m_pRTVShadowMap:&m_pRTV,
		bShadow?m_pDSVShadowMap:m_pDSV);
	if (!bShadow)
		m_pManager->GetContext()->
		PSSetShaderResources(4, 1, &m_pSRVShadowMap);
	SAFE_RELEASE(pBackBuffer);
	//5.- Dibujar
	unsigned int Offset = 0;
	unsigned int Stride = sizeof(VERTEX);
	m_pManager->GetContext()->IASetVertexBuffers(0, 1, &pVB, &Stride, &Offset);
	m_pManager->GetContext()->IASetIndexBuffer(pIB, DXGI_FORMAT_R32_UINT, 0);
	D3D11_MAPPED_SUBRESOURCE ms;
	m_pManager->GetContext()->Map(m_pCB, 0, D3D11_MAP_WRITE_DISCARD,0, &ms);
	PARAMS Temp = m_Params;
	Temp.World = Transpose(m_Params.World);
	Temp.View = Transpose(m_Params.View);
	Temp.Projection = Transpose(m_Params.Projection);
	Temp.LightView = Transpose(m_Params.LightView);
	Temp.LightProjection = Transpose(m_Params.LightProjection);
	for (int i = 0; i < 8; i++)
	{
		Temp.Lights[i].Position = m_Params.Lights[i].Position*m_Params.View;
		Temp.Lights[i].Direction = m_Params.Lights[i].Direction*m_Params.View;
	}
	memcpy(ms.pData, &Temp, sizeof(PARAMS));
	m_pManager->GetContext()->Unmap(m_pCB, 0);
	m_pManager->GetContext()->VSSetConstantBuffers(0, 1, &m_pCB);
	m_pManager->GetContext()->PSSetConstantBuffers(0, 1, &m_pCB);
	m_pManager->GetContext()->DrawIndexed(nIndices, 0, 0);
	SAFE_RELEASE(pIB);
	SAFE_RELEASE(pVB);
}
