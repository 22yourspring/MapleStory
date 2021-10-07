#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"


class CPlayerScript;
class CInstinctActive :
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
	UINT			COMBO_COUNT;

	CPlayerScript*	PS;
	INSTINCT_FSM	m_SkillState;
	INSTINCT_FSM	m_PrevSKILLState;



	virtual void awake();
	virtual int update();

	void Skillstate();
	void SkillPos();
	void SkillChangeState();
	void CurDirSet();

public:
	CLONE(CInstinctActive);

public:
	CInstinctActive();
	virtual ~CInstinctActive();
};

