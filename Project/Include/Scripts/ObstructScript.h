#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CObstructScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vRot;

	bool	FirstAccess; // 최초 1번 실행

	LASER_FSM	LASER_STATE;
	LASER_FSM	PREV_LASER_STATE;

	bool	LaserIdentifierChecker;
	LASER_TYPE	Laser_Identifier;

	static bool RedObstruct;
	static bool BlueObstruct;

	virtual void awake();
	virtual int update();

	void LaserCheck();
	void Skillstate();
	void SkillPos();
	void SkillChangeState();

public:
	CLONE(CObstructScript);

public:
	CObstructScript();
	~CObstructScript();
};