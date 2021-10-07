#pragma once
#include "Res.h"
#include "Shader.h"

class CTexture;
class CMaterial :
	public CRes
{
private:
	CShader*			m_pShader;
	tShaderParam		m_tParam;
	Ptr<CTexture>		m_arrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0];

	vector<tParamInfo>  m_vecParam;

public:
	virtual void UpdateData();

	void SetShader(CShader* _pShader)
	{
		assert(_pShader);
		m_pShader = _pShader;
		m_vecParam = m_pShader->GetParamInfo();		
	}

	CShader* GetShader() { return m_pShader; }
	void SetData(SHADER_PARAM _eType, void* _pData);

public:
	void Load(const wstring& _strFullPath, const wstring& _strName);

public:
	virtual void FileSave(FILE* _pFile);
	static void FileLoad(FILE* _pFile);

public:
	CMaterial* Clone();
	
public:
	CMaterial();
	virtual ~CMaterial();
};

