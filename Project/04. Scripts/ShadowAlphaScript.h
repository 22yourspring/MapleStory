#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CPlayerScript;
class CShadowAlphaScript :
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
	
	SHADOWFLASH_FSM		SHADOWALPHA_SkillState;
	SHADOWFLASH_FSM		SHADOWALPHA_PrevSKILLState;

	bool AlphaON;


	bool			Critical_Calculate();
	int				AlphaDamage[6];
	int				Alpha2Damage[15];

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

	SHADOWFLASH_FSM GetSkillState() { return SHADOWALPHA_SkillState; }

	void Alpha_ActivePos();
	void SetPlayerMove();

public:
	CLONE(CShadowAlphaScript);

public:
	CShadowAlphaScript();
	virtual ~CShadowAlphaScript();
};

