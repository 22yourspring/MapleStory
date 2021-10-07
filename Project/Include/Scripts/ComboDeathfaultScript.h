#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CPlayerScript;
class CCameraScript;
class CComboDeathfaultScript :
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

	CCameraScript*	CS;

	DEATHFAULT_FSM	DEATHFAULT_SkillState;
	DEATHFAULT_FSM	DEATHFAULT_PrevSKILLState;

	bool			Critical_Calculate();
	int				DeathFaultDamage[7];
	int				Critical_Value;

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	virtual void awake();
	virtual int update();

	void Skillstate();
	void SkillPos();
	void SkillChangeState();
	void CurDirSet();

	void findCS();

public:
	CLONE(CComboDeathfaultScript);

public:
	CComboDeathfaultScript();
	virtual ~CComboDeathfaultScript();
};

