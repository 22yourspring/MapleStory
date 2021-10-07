#pragma once
#include "global.h"

class CBlendState;
class CDepthStencilState;

class CRenderMgr 
{
	SINGLE(CRenderMgr);	
private:
	CBlendState*		m_arrBS[(UINT)BLEND_TYPE::END];
	CDepthStencilState* m_arrDS[(UINT)DEPTH_STENCIL_TYPE::END];
		   
public:
	void init();

public:
	CBlendState* GetBlendState(BLEND_TYPE _eType) { return m_arrBS[(UINT)_eType]; }

private:
	void CreateBlendState();
	void CreateDepthStencilState();	
};

