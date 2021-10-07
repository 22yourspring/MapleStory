#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CLaserBodyScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vRot;
	Vec3			vScale;

	static bool RedColl;
	static bool BlueColl;

	LASER_FSM	LASER_STATE;
	LASER_FSM	PREV_LASER_STATE;

	bool	LaserIdentifierChecker;
	LASER_TYPE	Laser_Identifier;

	virtual void awake();
	virtual int update();


	void LaserCheck();

	void LaserRot();

	void Skillstate();
	void SkillPos();
	void SkillChangeState();


	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

public:
	CLONE(CLaserBodyScript);

public:
	CLaserBodyScript();
	~CLaserBodyScript();
};