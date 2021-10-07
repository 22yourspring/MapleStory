#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CInstinctScript;
class CInstinctActiveScript :
	public CScript
{
private:
	Ptr<CPrefab>	m_pBullet;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;


public:

	Vec3			vPos;
	Vec3			vRot;
	DIR				PlayerDir;
	FSM				PlayerState;

	static INSTINCT_FSM	INSTINCTACTIVE_SkillState;
	static INSTINCT_FSM	INSTINCTACTIVE_PrevSKILLState;



	virtual void awake();
	virtual int update();

	void Skillstate();
	void SkillPos();
	void SkillChangeState();

public:
	CLONE(CInstinctActiveScript);

public:
	CInstinctActiveScript();
	virtual ~CInstinctActiveScript();
};

