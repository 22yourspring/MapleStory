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

	tSwapDesc.BufferCount = 1; // ���� ī��Ʈ
	tSwapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT; // ����(�ؽ���) �� �̹��� ��� Ÿ������ �˸�
	tSwapDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;	// ��� ���� �ȼ� ����
	tSwapDesc.BufferDesc.Width = (UINT)_res.x;	// ��� ���� �ػ�
	tSwapDesc.BufferDesc.Height = (UINT)_res.y;	// ��� ���� �ػ�

	// �ֻ���
	tSwapDesc.BufferDesc.RefreshRate.Denominator = 1;
	tSwapDesc.BufferDesc.RefreshRate.Numerator = 60;



	// ����� 
	tSwapDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	tSwapDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	
	// ��� ������ �� â��� 
	tSwapDesc.Windowed = _bWindow;
	tSwapDesc.OutputWindow = m_hWnd;

	// ��Ƽ ���� AA �� ���ؼ� ����
	tSwapDesc.SampleDesc.Count = 8; //4
	tSwapDesc.SampleDesc.Quality = 0;

	// ���� ������ �̹����� �������� ����
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
	// RenderTargetView �����
	// �̹� �����Ǿ� �ִ� Texture �� �����;� �Ѵ�.
	ID3D11Texture2D* pTargetTexture = NULL;
	m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)&pTargetTexture);	
	pTargetTexture->Release(); // GetBuffer �Լ��� ���� ������ ������ ������

	m_pDevice->CreateRenderTargetView(pTargetTexture, NULL, &m_pTargetView);

	// DepthStencilTexture ���� �����
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

	// DepthStencilView �� DepthStencilTexture �� �̿��ؼ� �����
	m_pDevice->CreateDepthStencilView(m_pDepthStencilTex, NULL, &m_pDepthStencilView);
	
	// ��� Ÿ�ٰ� ���� ���ٽ� Ÿ���� �˸���.
	m_pContext->OMSetRenderTargets(1, &m_pTargetView, m_pDepthStencilView);
	


	//========
	// �� ��Ʈ
	//========
	// ������� ��� �̹����� ���۵ɶ�, ��� �� ������ �����Ѵ�.
	// �����δ� ����Ʈ���� �����ؼ� ����̹����� �ϼ��ǰ�, ������� �̹����� ����ȴ�.

	D3D11_VIEWPORT tViewPortDesc = {};

	tViewPortDesc.Width	 = _res.x;
	tViewPortDesc.Height = _res.y;
	tViewPortDesc.MinDepth = 0.f;
	tViewPortDesc.MaxDepth = 1.f;
	tViewPortDesc.TopLeftX = 0;
	tViewPortDesc.TopLeftY = 0;
	
	m_pContext->RSSetViewports(1, &tViewPortDesc);

	// Samplder �ϳ� �����
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
	// ��� �̹��� Ŭ����
	m_pContext->ClearRenderTargetView(m_pTargetView, _pCol);

	// ���� ���ٽ� ���� Ŭ����
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
