#include "ShaderMgr.h"

#include "Shader.h"
#include "RenderMgr.h"

CShaderMgr::CShaderMgr()
{
}

CShaderMgr::~CShaderMgr()
{
	Safe_Delete_Map(m_mapShader);
}

void CShaderMgr::init()
{
	CShader* pShader = NULL;

	//==============
	// Color Shader
	//==============
	pShader = new CShader;
	pShader->CreateVertexShader(L"std.fx", "VS_Color", "vs_5_0");
	pShader->CreatePixelShader(L"std.fx", "PS_Color", "ps_5_0");
	AddShader(L"ColorShader", pShader);

	//================
	// Collider Shader
	//================
	pShader = new CShader;
	pShader->CreateVertexShader(L"std.fx", "VS_Collider", "vs_5_0");
	pShader->CreatePixelShader(L"std.fx", "PS_Collider", "ps_5_0");

	pShader->AddShaderParam(L"Collision", SHADER_PARAM::INT_0);

	AddShader(L"ColliderShader", pShader);


	//==============
	// Std 2D Shader
	//==============
	pShader = new CShader;
	pShader->CreateVertexShader(L"std.fx", "VS_Std2D", "vs_5_0");
	pShader->CreatePixelShader(L"std.fx", "PS_Std2D", "ps_5_0");

	// Shader param Description
	pShader->AddShaderParam(L"Diffuse Texture", SHADER_PARAM::TEX_0);	

	// Blend State Setting
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));

	AddShader(L"Std2DShader", pShader);




	//==============
	// EFFECT Shader
	//==============
	pShader = new CShader;
	pShader->CreateVertexShader(L"std.fx", "VS_Std2D", "vs_5_0");
	pShader->CreatePixelShader(L"std.fx", "PS_Std2D", "ps_5_0");

	// Shader param Description
	pShader->AddShaderParam(L"Diffuse Texture", SHADER_PARAM::TEX_0);	

	// Blend State Setting
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::EFFECT));

	AddShader(L"Std_EFFECT_Shader", pShader);




	//==============
	// FADE Shader
	//==============
	pShader = new CShader;
	pShader->CreateVertexShader(L"std.fx", "VS_FADE", "vs_5_0");
	pShader->CreatePixelShader(L"std.fx", "PS_FADE", "ps_5_0");

	// Shader param Description
	pShader->AddShaderParam(L"Diffuse Texture", SHADER_PARAM::TEX_0);

	// Blend State Setting
	pShader->SetBlendState(CRenderMgr::GetInst()->GetBlendState(BLEND_TYPE::ALPHABLEND));

	AddShader(L"FADE_Shader", pShader);
}

void CShaderMgr::AddShader(const wstring & _strKey, CShader * _pShader)
{
	assert(m_mapShader.end() == m_mapShader.find(_strKey));	
	_pShader->SetName(_strKey);
	m_mapShader.insert(make_pair(_strKey, _pShader));
}

CShader * CShaderMgr::FindShader(const wstring & _strKey)
{
	map<wstring, CShader*>::iterator iter = m_mapShader.find(_strKey);	
	assert(m_mapShader.end() != iter);
	return iter->second;
}
