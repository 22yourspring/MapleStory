#pragma once
#include "Component.h"
#include "Ptr.h"

class CMesh;
class CMaterial;
class CMeshRender :
	public CComponent
{
private:
	Ptr<CMesh>		m_pMesh;
	Ptr<CMaterial>	m_pMtrl;

public:
	void SetMesh(CMesh* _pMesh) { m_pMesh = _pMesh; }
	void SetMaterial(CMaterial* _pMtrl) { m_pMtrl = _pMtrl; }

	CMesh* GetMesh() { return m_pMesh; }
	CMaterial* GetSharedMaterial() { return m_pMtrl; }
	CMaterial* GetCloneMaterial();

	virtual int update();
	void render();

public:
	CLONE(CMeshRender);
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CMeshRender();
	virtual ~CMeshRender();
};

