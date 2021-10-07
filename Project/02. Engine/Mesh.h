#pragma once
#include "Res.h"

class CShader;

class CMesh :
	public CRes
{
	ID3D11Buffer*			m_pVB;		// Vertex buffer
	ID3D11Buffer*			m_pIB;		// Index buffer
	ID3D11InputLayout*		m_pLayout;	// InputLayout
	
	UINT					m_iVtxSize;
	UINT					m_iVtxCount;
	UINT					m_iIdxSize;
	UINT					m_iIdxCount;
	D3D_PRIMITIVE_TOPOLOGY  m_eTopology;

	D3D11_BUFFER_DESC		m_tVtxDesc; 
	D3D11_BUFFER_DESC		m_tIdxDesc; 

	void*					m_pVtxMem;	
	void*					m_pIdxMem;	

	vector<D3D11_INPUT_ELEMENT_DESC>	m_vecLayout;
	ID3DBlob*							m_pVtxBlob;

public:

	void render();

public:
	static CMesh* Create(void* _pVtx, UINT _iVtxSize, UINT _iVtxCount, D3D11_USAGE _eVtxUsage
		, void* _pIdx, UINT _iIdxSize, UINT _iIdxCount, D3D11_USAGE _eIdxUsage
		, D3D_PRIMITIVE_TOPOLOGY _eTopology = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
public:
	virtual void Load(const wstring& _strFullPath, const wstring& _strName)
	{
		
	}

	virtual void UpdateData();
	void SetShader(CShader* _pShader);

	virtual void FileSave(FILE* _pFile);
	static void FileLoad(FILE* _pFile);
	
	CLONE_DISABLE(CMesh);
public:
	CMesh();
	virtual ~CMesh();
};

