#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CPlayerScript;
class CRagingBlowScript;
class CInstinctScript :
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

	UINT			m_iInstinctNumber;

	INSTINCT_FSM	INSTINCT_SkillState;
	INSTINCT_FSM	INSTINCT_PrevSKILLState;
	
	bool			Critical_Calculate();
	int				Critical_Value;
	int				InstinctDamage[6];

	static bool	PrevConfirm;
		
	static bool		PlayerAction;
	static bool		m_bInstinctActive;

	virtual void awake();
	virtual int update();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	void Skillstate();
	void SkillPos();
	void SkillChangeState();
	void CurDirSet();

//	void InstinctActivePos();

public:
	CLONE(CInstinctScript);

public:
	CInstinctScript();
	virtual ~CInstinctScript();
};

