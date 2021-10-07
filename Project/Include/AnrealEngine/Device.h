#pragma once

#include "global.h"

class CConstBuffer;

class CDevice
{
	SINGLE(CDevice);

private:
	HWND						m_hWnd;		// 타겟 윈도우 핸들
	ID3D11Device*				m_pDevice;	// GPU 메모리, 할당
	ID3D11DeviceContext*		m_pContext;	// Render
	
	UINT						m_iQualityLv; // 멀티샘플 품질 수준

	IDXGISwapChain*				m_pSwapChain;	// Swap Chain
	ID3D11RenderTargetView*		m_pTargetView;	// 출력 타겟 뷰

	ID3D11Texture2D*			m_pDepthStencilTex;	// 깊이 스텐실 버퍼
	ID3D11DepthStencilView*		m_pDepthStencilView;// 깊이 스텐실 뷰
	
	tResolution					m_res;		// 현재 해상도
	bool						m_bWindow;	// 창모드 

	map<wstring, CConstBuffer*>	m_mapCBuff;
	ID3D11SamplerState*			m_pSampler;

	ID3D11RasterizerState*      m_arrRSState[(UINT)RASTERIZER_TYPE::END];

public:
	int init(HWND _hWnd, const tResolution& _res, bool _bWindow);
	void ClearTarget(float* _pCol);
	void Present(){	m_pSwapChain->Present(0, 0);}

	ID3D11Device* GetDevice() { return m_pDevice; }
	ID3D11DeviceContext* GetContext() { return m_pContext; }

	void SetRasteriazeState(RASTERIZER_TYPE _eType)
	{
		CONTEXT->RSSetState(m_arrRSState[(UINT)_eType]);		
	}

public:
	void CreateCBuffer(const wstring _strKey, UINT _iSize, UINT _iRegister);
	CConstBuffer* FindCBuffer(const wstring& _strKey);
	const tResolution& GetResolution() {return m_res;}
};

