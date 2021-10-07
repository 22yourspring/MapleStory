#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"
#include "AnrealEngine/TimeMgr.h"

class CPlayerScript;
class CGravityScript :
	public CScript
{
private:
	Ptr<CPrefab>	m_pBullet;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

	static bool	m_bOnGravity;
	int		LayerCount;

	static float 	m_fGravityAcc;
	static float	m_fGravity;

	float	m_fMaxGravity = 100.f;

public:
	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	Vec3	m_TerrainvPos;	//TERRAIN
	Vec3	m_TerrainvRot;
	Vec3	m_TerrainvScale;
	Vec3	m_ObjectPos;	//Å¸°Ù

	FSM		PlayerState;

	CPlayerScript* PS;

	static bool	OverTerrain;


	static float T_Up;
	static float T_Down;
	static float P_Up;
	static float P_Down;
	static float P_Scale;
	static float P_Pos;
	static float P_CalOffSet;
	static float T_Left;
	static float T_Right;

//	bool	IsColl() { return m_bOnGravity; }
	static void	SetGravity(bool _Gravity) {	m_bOnGravity = _Gravity	;}
	static void	SetGravityAcc() { m_bOnGravity = false; m_fGravity = m_fGravityAcc; };

	float m_fSlope;
	float m_fY;
	float fData;

	virtual void awake();
	virtual int update();

public:
	CLONE(CGravityScript);

public:
	CGravityScript();
	~CGravityScript();
};