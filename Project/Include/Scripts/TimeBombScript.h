#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CTimeBombScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			startPos;
	DIR				SKILL_DIR;

	bool		OBJECT_State;
	bool		PREV_OBJECTState;

	bool		iscollisionBombs;

	float TimeSec;

	virtual void awake();
	virtual int update();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	void Skillstate();
	void SkillChangeState();


	bool TakeTimeBombs;

	float ActiveAcc;

public:
	CLONE(CTimeBombScript);

public:
	CTimeBombScript();
	~CTimeBombScript();
};