#pragma once
#include "Entity.h"
#include "Ptr.h"

class CMesh;
class CTexture;
class CSound;
class CPrefab;
class CMaterial;
class CRes :
	public CEntity
{
private:
	UINT		m_iRef;	// 리소스 참조 회수

protected:
	wstring		m_strPath; // 리소스의 상대 경로(저장 위치)

public:
	void AddRef() { ++m_iRef; };
	void SubRef() { --m_iRef; };

private:
	void SetPath(const wstring& _strPath) { m_strPath = _strPath; }

public:
	virtual int update() { return 0; }

public:
	// File 로 부터 로딩
	virtual void Load(const wstring& _strFullPath, const wstring& _strName) = 0;

	virtual void FileSave(FILE* _pFile) = 0;

public:
	//CLONE_DISABLE(CRes);

	virtual CRes* Clone() 
	{ 
		assert(nullptr); 
		return nullptr; 
	}

public:
	CRes();
	virtual ~CRes();
	

	friend class Ptr<CMesh>;
	friend class Ptr<CMaterial>;
	friend class Ptr<CSound>;
	friend class Ptr<CTexture>;
	friend class Ptr<CPrefab>;

	friend class CResMgr;
};

