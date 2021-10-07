#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"
#include "AnrealEngine/TimeMgr.h"

class CMobGravityScript :
	public CScript
{
private:
	Ptr<CPrefab>	m_pBullet;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

	bool	m_bOnGravity;
	int		LayerCount;

	float 	m_fGravityAcc = 98.f * 5.f;
	float	m_fGravity = m_fGravityAcc;
	float	m_fMaxGravity = 100.f;

public:
	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	CGameObject* CurTerrain;

	Vec3	m_TerrainvPos;	//TERRAIN
	Vec3	m_TerrainvRot;
	Vec3	m_TerrainvScale;
	Vec3	m_ObjectPos;	//Å¸°Ù

	FSM		PlayerState;

	bool	IsColl() { return m_bOnGravity; }
	void	SetGravity(bool _Gravity) { m_bOnGravity = _Gravity; }
	void	SetGravityAcc() { m_bOnGravity = false; m_fGravity = m_fGravityAcc; };

	float m_fSlope;
	float m_fY;
	float fData;

	virtual void awake();
	virtual int update();

public:
	CLONE(CMobGravityScript);

public:
	CMobGravityScript();
	~CMobGravityScript();
};