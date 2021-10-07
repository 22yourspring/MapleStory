#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"
#include "AnrealEngine/TimeMgr.h"

class CPlayerScript;
class CWallScript :
	public CScript
{
private:
	Ptr<CPrefab>	m_pBullet;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;


public:
	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);


	Vec3	m_ObjectPos;	//Å¸°Ù

	Vec3	m_TerrainvPos;	//TERRAIN
	Vec3	m_TerrainvRot;
	Vec3	m_TerrainvScale;


	virtual void awake();
	virtual int update();

public:
	CLONE(CWallScript);

public:
	CWallScript();
	~CWallScript();
};