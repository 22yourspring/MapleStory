#pragma once
#include "Entity.h"

class CConstBuffer :
	public CEntity
{
private:
	ID3D11Buffer*		m_pCB;
	UINT				m_iMaxSize;
	UINT				m_iRegister;
	
public:
	virtual int update() { return 0; }
	static CConstBuffer* Create(const wstring& _strName, UINT _iSize, UINT _iRegister);

public:
	void UpdateData(void* _pSrc, UINT _iSize);
	void UpdateRegister(UINT _iRegisterType);
	void UpdateRegisterAll();
	void ClearData();

private:
	CLONE(CConstBuffer);

public:
	CConstBuffer();
	~CConstBuffer();
};

