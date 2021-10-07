#pragma once
#include "Entity.h"

struct tParamInfo
{
	wstring			strParamName;
	SHADER_PARAM	eType;	
};

class CBlendState;
class CDepthStencilState;

class CShader :
	public CEntity
{
	ID3DBlob*				m_pVSBlob;
	ID3DBlob*				m_pHSBlob;
	ID3DBlob*				m_pDSBlob;
	ID3DBlob*				m_pGSBlob;
	ID3DBlob*				m_pCSBlob;	
	ID3DBlob*				m_pPSBlob;

	ID3DBlob*				m_pErrBlob;
	
	ID3D11VertexShader*		m_pVS;
	ID3D11HullShader*		m_pHS;
	ID3D11DomainShader*		m_pDS;
	ID3D11GeometryShader*	m_pGS;
	ID3D11ComputeShader*	m_pCS;
	ID3D11PixelShader*		m_pPS;

	vector<tParamInfo>		m_vecParam;
	CBlendState*			m_pBlendState;
	CDepthStencilState*		m_pDepthStencilState;

public:
	ID3DBlob* GetVSBlob() { return m_pVSBlob; }
	const vector<tParamInfo>& GetParamInfo() { return m_vecParam; };


public:
	virtual int update() { return 0; }
	virtual void UpdateData();

	int CreateVertexShader(const wstring& _strFileName, const string& _strFuncName, const string& _strVersion);
	int CreatePixelShader(const wstring& _strFileName, const string& _strFuncName, const string& _strVersion);

	void AddShaderParam(const wstring& _strParamName, SHADER_PARAM _eType)
	{
		m_vecParam.push_back(tParamInfo{ _strParamName , _eType});
	}
	void SetBlendState(CBlendState* _pBlendState) { m_pBlendState = _pBlendState; }	

public:
	CLONE(CShader);

public:
	CShader();
	~CShader();
};