#include "Device.h"

#include "ConstBuffer.h"

CDevice::CDevice()
	: m_hWnd(NULL)
	, m_pDevice(NULL)
	, m_pContext(NULL)
	, m_iQualityLv(0)
	, m_pSwapChain(NULL)
	, m_pTargetView(NULL)
	, m_pDepthStencilTex(NULL)
	, m_pDepthStencilView(NULL)
	, m_res{}
	, m_bWindow(true)
{
}


CDevice::~CDevice()
{
	SAFE_RELEASE(m_pDevice);
	SAFE_RELEASE(m_pContext);
	SAFE_RELEASE(m_pSwapChain);
	SAFE_RELEASE(m_pTargetView);
	SAFE_RELEASE(m_pDepthStencilTex);
	SAFE_RELEASE(m_pDepthStencilView);

	Safe_Delete_Map(m_mapCBuff);	

	SAFE_RELEASE(m_pSampler);

	Safe_Release_Array(m_arrRSState);
}

int CDevice::init(HWND _hWnd, const tResolution & _res, bool _bWindow)
{
	m_bWindow = _bWindow;
	m_res = _res;
	m_hWnd = _hWnd;

	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3D11_CREATE_DEVICE_DEBUG;	
#endif
	
	D3D_FEATURE_LEVEL eLv;
	if (FAILED(D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, iFlag,
		0, 0, D3D11_SDK_VERSION, &m_pDevice, &eLv, &m_pContext)))
	{
		return RET_FAILED;
	}
		
	m_pDevice->CheckMultisampleQualityLevels(DXGI_FORMAT_R8G8B8A8_UNORM, 4, &m_iQualityLv);

	if (m_iQualityLv == 0)
		return RET_FAILED;

	DXGI_SWAP_CHAIN_DESC tSwapDesc = {};

	tSwapDesc.BufferCount = 1; // 버퍼 카운트
	tSwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // 버퍼(텍스쳐) 가 이미지 출력 타겟임을 알림
	tSwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// 출력 버퍼 픽셀 포맷
	tSwapDesc.BufferDesc.Width = (UINT)_res.x;	// 출력 버퍼 해상도
	tSwapDesc.BufferDesc.Height = (UINT)_res.y;	// 출력 버퍼 해상도

	// 주사율
	tSwapDesc.BufferDesc.RefreshRate.Denominator = 1;
	tSwapDesc.BufferDesc.RefreshRate.Numerator = 60;



	// 모니터 
	tSwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tSwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	// 출력 윈도우 및 창모드 
	tSwapDesc.Windowed = _bWindow;
	tSwapDesc.OutputWindow = m_hWnd;

	// 멀티 샘플 AA 를 위해서 셋팅
	tSwapDesc.SampleDesc.Count = 8; //4
	tSwapDesc.SampleDesc.Quality = 0;

	// 이전 프레임 이미지를 저장하지 않음
	tSwapDesc.SwapEffect = DXGI_SWAP_EFFECT::DXGI_SWAP_EFFECT_DISCARD;
	
	IDXGIDevice* pDevice = NULL;
	IDXGIAdapter* pAdapter = NULL;
	IDXGIFactory* pFactory = NULL;

	m_pDevice->QueryInterface(__uuidof(IDXGIDevice), (void**)&pDevice);
	pDevice->GetAdapter(&pAdapter);
	pAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&pFactory);
	
	pFactory->CreateSwapChain(m_pDevice, &tSwapDesc, &m_pSwapChain);

	pDevice->Release();
	pAdapter->Release();
	pFactory->Release();

	//=============
	// Create View
	//=============
	// RenderTargetView 만들기
	// 이미 생성되어 있는 Texture 를 가져와야 한다.
	ID3D11Texture2D* pTargetTexture = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pTargetTexture);	
	pTargetTexture->Release(); // GetBuffer 함수에 의해 증가한 참조를 제거함

	m_pDevice->CreateRenderTargetView(pTargetTexture, NULL, &m_pTargetView);

	// DepthStencilTexture 먼저 만들기
	D3D11_TEXTURE2D_DESC tTexDesc = {};

	tTexDesc.Width = (UINT)_res.x;
	tTexDesc.Height = (UINT)_res.y;
	tTexDesc.MipLevels = 1;
	tTexDesc.ArraySize = 1;
	tTexDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	
	tTexDesc.SampleDesc.Count = 8; //4
	tTexDesc.SampleDesc.Quality = 0;
	

	tTexDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	tTexDesc.Usage = D3D11_USAGE_DEFAULT;
	

	m_pDevice->CreateTexture2D(&tTexDesc, 0, &m_pDepthStencilTex);

	// DepthStencilView 를 DepthStencilTexture 를 이용해서 만들기
	m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, NULL, &m_pDepthStencilView);
	
	// 출력 타겟과 깊이 스텐실 타겟을 알린다.
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthStencilView);
	


	//========
	// 뷰 포트
	//========
	// 윈도우로 출력 이미지가 전송될때, 출력 될 영역을 지정한다.
	// 실제로는 뷰포트까지 감안해서 출력이미지가 완성되고, 윈도우로 이미지가 복사된다.

	D3D11_VIEWPORT tViewPortDesc = {};

	tViewPortDesc.Width	 = _res.x;
	tViewPortDesc.Height = _res.y;
	tViewPortDesc.MinDepth = 0.f;
	tViewPortDesc.MaxDepth = 1.f;
	tViewPortDesc.TopLeftX = 0;
	tViewPortDesc.TopLeftY = 0;
	
	m_pContext->RSSetViewports(1, &tViewPortDesc);

	// Samplder 하나 만들기
	D3D11_SAMPLER_DESC tSamDesc = {};
	tSamDesc.AddressU = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressV = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;
	tSamDesc.AddressW = D3D11_TEXTURE_ADDRESS_MODE::D3D11_TEXTURE_ADDRESS_WRAP;

	tSamDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	DEVICE->CreateSamplerState(&tSamDesc, &m_pSampler);	
	CONTEXT->PSSetSamplers(0, 1, &m_pSampler);
	
	//==================
	// Rasterizer State
	//==================

	m_arrRSState[(UINT)RASTERIZER_TYPE::CULL_BACK] = nullptr;
	
	D3D11_RASTERIZER_DESC tRSDesc = {};
	tRSDesc.CullMode = D3D11_CULL_FRONT;
	tRSDesc.FillMode = D3D11_FILL_SOLID;

	DEVICE->CreateRasterizerState(&tRSDesc, &m_arrRSState[(UINT)RASTERIZER_TYPE::CULL_FRONT]);

	tRSDesc = {};
	tRSDesc.CullMode = D3D11_CULL_NONE;
	tRSDesc.FillMode = D3D11_FILL_SOLID;

	DEVICE->CreateRasterizerState(&tRSDesc, &m_arrRSState[(UINT)RASTERIZER_TYPE::CULL_NONE]);

	tRSDesc = {};
	tRSDesc.CullMode = D3D11_CULL_NONE;
	tRSDesc.FillMode = D3D11_FILL_WIREFRAME;

	DEVICE->CreateRasterizerState(&tRSDesc, &m_arrRSState[(UINT)RASTERIZER_TYPE::WIREFRAME]);

	return RET_SUCCESS;
}

void CDevice::ClearTarget(float * _pCol)
{
	// 출력 이미지 클리어
	m_pContext->ClearRenderTargetView(m_pTargetView, _pCol);

	// 깊이 스텐실 정보 클리어
	m_pContext->ClearDepthStencilView(m_pDepthStencilView
		, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL
		, 1.f, 0);
}

void CDevice::CreateCBuffer(const wstring _strKey, UINT _iSize, UINT _iRegister)
{
	CConstBuffer* pBuffer = CConstBuffer::Create(_strKey, _iSize, _iRegister);

	m_mapCBuff.insert(make_pair(_strKey, pBuffer));
}

CConstBuffer * CDevice::FindCBuffer(const wstring & _strKey)
{
	map<wstring, CConstBuffer*>::iterator iter =  m_mapCBuff.find(_strKey);
	if (iter == m_mapCBuff.end())
	{
		return NULL;
	}
	return iter->second;
}
