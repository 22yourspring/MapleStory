#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CPlayerScript;
class CComboSubScript :
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
	CPlayerScript*	PS;

	BUFF_FRONT	m_SkillState;
	BUFF_FRONT	m_PrevSKILLState;

	virtual void awake();
	virtual int update();

	void Skillstate();
	void SkillPos();
	void SkillChangeState();

public:
	CLONE(CComboSubScript);

public:
	CComboSubScript();
	virtual ~CComboSubScript();
};

