#pragma once
#include "Res.h"

#include "DirectXTex/DirectXTex.h"

#ifdef _DEBUG
#pragma comment(lib, "DirectXTex_Debug")
#else
#pragma comment(lib, "DirectXTex")
#endif

class CTexture :
	public CRes
{
private:
	ScratchImage				m_image;	
	ID3D11ShaderResourceView*	m_pSRView;
	ID3D11Texture2D*			m_pTex;
	
public:
	ID3D11ShaderResourceView* GetSRV() {return m_pSRView;}

public:
	virtual void Load(const wstring& _strFullPath, const wstring& _strName);
		   
	float GetWidth() { return (float)m_image.GetMetadata().width;	}
	float GetHeight(){ return (float)m_image.GetMetadata().height; }

	void SetRegister(UINT _iShaderType, UINT _iRegister);	
	void SetRegisterAll(UINT _iRegister);

public:
	virtual void FileSave(FILE* _pFile);	
	static void FileLoad(FILE* _pFile);

public:
	CLONE_DISABLE(CTexture);

public:
	CTexture();	
	virtual ~CTexture();
};

