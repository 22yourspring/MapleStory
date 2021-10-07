#include "RenderMgr.h"

#include "BlendState.h"
#include "DepthStencilState.h"

CRenderMgr::CRenderMgr()
	: m_arrBS{}
	, m_arrDS{}
{
	
}

CRenderMgr::~CRenderMgr()
{
	Safe_Delete_Array(m_arrBS);
	Safe_Delete_Array(m_arrDS);
}

void CRenderMgr::init()
{
	// Blend State 생성
	CreateBlendState();

	// 
	CreateDepthStencilState();
}

void CRenderMgr::CreateBlendState()
{
	D3D11_BLEND_DESC tDesc = {};
	
	tDesc.AlphaToCoverageEnable = true; //false
	tDesc.IndependentBlendEnable = false;
	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD; // 혼합 방식
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_SRC_ALPHA; // Source 계수
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_INV_SRC_ALPHA;// Dest 계수
	
	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	m_arrBS[(UINT)BLEND_TYPE::ALPHABLEND] = new CBlendState;
	m_arrBS[(UINT)BLEND_TYPE::ALPHABLEND]->Create(&tDesc);	






	tDesc.AlphaToCoverageEnable = false;
	tDesc.IndependentBlendEnable = false;
	tDesc.RenderTarget[0].BlendEnable = true;
	tDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD; // 혼합 방식
	tDesc.RenderTarget[0].SrcBlend = D3D11_BLEND::D3D11_BLEND_ONE;		// Source 계수
	tDesc.RenderTarget[0].DestBlend = D3D11_BLEND::D3D11_BLEND_ONE;		// Dest 계수
	
	tDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP::D3D11_BLEND_OP_ADD;
	tDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND::D3D11_BLEND_ONE;
	tDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND::D3D11_BLEND_ZERO;
	tDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE::D3D11_COLOR_WRITE_ENABLE_ALL;

	m_arrBS[(UINT)BLEND_TYPE::EFFECT] = new CBlendState;
	m_arrBS[(UINT)BLEND_TYPE::EFFECT]->Create(&tDesc);
}





void CRenderMgr::CreateDepthStencilState()
{
	//D3D11_DEPTH_STENCIL_DESC::DepthEnable = false;
}