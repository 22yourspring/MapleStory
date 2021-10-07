#pragma once
#include "Entity.h"

class CBlendState :
	public CEntity
{
private:
	ID3D11BlendState*	m_pState;
	D3D11_BLEND_DESC	m_tDesc;
	Vec4				m_vFactor;

public:
	void Create(D3D11_BLEND_DESC* _pDesc);
	void SetState();
	static void SetDefaultState();
	


public:
	int update() { return 0; }
	CLONE_DISABLE(CBlendState);

public:
	CBlendState();
	~CBlendState();
};

