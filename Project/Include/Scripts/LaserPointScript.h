#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CLaserPointScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vRot;

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

public:
	CLONE(CLaserPointScript);

public:
	CLaserPointScript();
	~CLaserPointScript();
};