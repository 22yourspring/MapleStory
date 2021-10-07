#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

//#include "AnrealEngine/Well512Random.h"

class CAttackScript :
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

	static	DIR		RagingDir;

	UINT			COMBO_COUNT;

	FSM				PlayerState;

	ATTACK_FSM	ATTACK_SkillState;
	ATTACK_FSM	ATTACK_PrevSKILLState;

	bool			m_bAfterEffect;

	bool			Critical_Calculate();
	int				Critical_Value;
	int				AttackDamage;

	bool EnrageActive;

	virtual void awake();
	virtual int update();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	void Skillstate();
	void SkillPos();
	void SkillChangeState();
	void CurDirSet();

public:
	CLONE(CAttackScript);

public:
	CAttackScript();
	virtual ~CAttackScript();
};

