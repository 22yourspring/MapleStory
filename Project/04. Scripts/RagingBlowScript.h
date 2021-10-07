#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CRagingBlowScript :
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
	
	static RAGINGBLOW_FSM	RAGINGBLOW_SkillState;
	static RAGINGBLOW_FSM	RAGINGBLOW_PrevSKILLState;
	static bool RagingBlowPhase2Mob;

	bool			m_bAfterEffect;

	bool			Critical_Calculate();
	int				Critical_Value;
	int				RagingDamage[5];

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
	CLONE(CRagingBlowScript);

public:
	CRagingBlowScript();
	virtual ~CRagingBlowScript();
};

