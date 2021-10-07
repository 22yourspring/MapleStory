#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CPlayerScript;
class CShadowBetaScript :
	public CScript
{
private:
	Ptr<CPrefab>	m_pBullet;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;


public:

	Vec3			vPos;
	Vec3			vRot;

	FSM				PlayerState;

	SHADOWFLASH_FSM		SHADOWBETA_SkillState;
	SHADOWFLASH_FSM		SHADOWBETA_PrevSKILLState;

	bool BetaOn;


	bool			Critical_Calculate();
	int				BetaDamage[5];
	int				Beta2Damage[12];

	int				Critical_Value;

	int OtherCount;
	CCollider* OtherPtr[300];

	void AFTEREFFECT(CCollider * _pOther);
	bool m_bAfterEffect;
	void EFFECT_PROCESSING();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	virtual void awake();
	virtual int update();

	void Skillstate();
	void SkillPos();
	void SkillChangeState();
	
	SHADOWFLASH_FSM GetSkillState() { return SHADOWBETA_SkillState; }

	void Beta_ActivePos();
	void SetPlayerMove();

public:
	CLONE(CShadowBetaScript);

public:
	CShadowBetaScript();
	virtual ~CShadowBetaScript();
};

