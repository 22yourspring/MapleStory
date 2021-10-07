#pragma once
#include "Res.h"

class CGameObject;

class CPrefab :
	public CRes
{
private:
	CGameObject*	m_pProto;

public:
	CGameObject* Instantiate();
	CGameObject* GetOriginObj() const { return m_pProto; }

public:
	virtual void Load(const wstring& _strFullPath, const wstring& _strName)
	{
		
	}

private:
	virtual void FileSave(FILE* _pFile) {}

	CLONE_DISABLE(CPrefab);
	
public:
	CPrefab();
	CPrefab(CGameObject* _pObj);
	virtual ~CPrefab();
};

