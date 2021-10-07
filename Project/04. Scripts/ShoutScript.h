#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CPlayerScript;
class CShoutScript :
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

	static SHOUT	SHOUT_SkillState;
	static SHOUT	SHOUT_PrevSKILLState;


	virtual void awake();
	virtual int update();

	bool			Critical_Calculate();
	int				ShoutDamage[6];
	int				Critical_Value;

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	void Skillstate();
	void SkillPos();
	void SkillChangeState();
	void CurDirSet();

public:
	CLONE(CShoutScript);

public:
	CShoutScript();
	virtual ~CShoutScript();
};

