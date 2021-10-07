#include "BlendState.h"

#include "Device.h"

CBlendState::CBlendState()
	: m_pState(nullptr)
	, m_tDesc{}
{
}

CBlendState::~CBlendState()
{
	SAFE_RELEASE(m_pState);
}


void CBlendState::Create(D3D11_BLEND_DESC * _pDesc)
{
	DEVICE->CreateBlendState(_pDesc, &m_pState);
	assert(m_pState);

	m_tDesc = *_pDesc;
}

void CBlendState::SetState()
{
	CONTEXT->OMSetBlendState(m_pState, (float*)&m_vFactor, 0xffffffff);
}

void CBlendState::SetDefaultState()
{
	CONTEXT->OMSetBlendState(nullptr, nullptr, 0xffffffff);
}
