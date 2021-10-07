#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CMapleWarriorScript :
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

	static MAPLEWARRIOR		MAPLEWARRIOR_SkillState;
	static MAPLEWARRIOR		MAPLEWARRIOR_PrevSKILLState;

	static bool m_bMAPLEWARRIOR_ON;

	virtual void awake();
	virtual int update();

	void Skillstate();
	void SkillPos();
	void SkillChangeState();
	void CurDirSet();

public:
	CLONE(CMapleWarriorScript);

public:
	CMapleWarriorScript();
	virtual ~CMapleWarriorScript();
};

