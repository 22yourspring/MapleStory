#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"


enum class ECLIPSE_ENABLE
{
	NONE,
	ENABLE,
	DISABLE,
	END,
};

class CCameraScript;
class CPlayerScript;
class CSoulEclipseScript :
	public CScript
{
private:
	Ptr<CPrefab>	m_pBullet;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;


public:
	bool			Critical_Calculate();
	int				EclipseDamage[6];
	int				Eclipse2Damage[15];

	int				Critical_Value;

	int OtherCount;
	CCollider* OtherPtr[300];

	void AFTEREFFECT(CCollider * _pOther);
	bool m_bAfterEffect;

	void EFFECT_PROCESSING();

	Vec3			vPos;
	Vec3			vRot;

	FSM				PlayerState;
	CCameraScript*	CS;

	ECLIPSE_ENABLE	ECLIPSE_ACTIVE;

	static SOULECLIPSE_FSM		MOON_State;
	static SOULECLIPSE_FSM		EFFECT_State;
	static SOULECLIPSE_FSM		PARTICLE_State;
	static SOULECLIPSE_FSM		SUNSET_State;
	static SOULECLIPSE_FSM		OUTLINE_State;

	static SOULECLIPSE_FSM		PrevMOON_State;
	static SOULECLIPSE_FSM		PrevEFFECT_State;
	static SOULECLIPSE_FSM		PrevPARTICLE_State;
	static SOULECLIPSE_FSM		PrevSUNSET_State;
	static SOULECLIPSE_FSM		PrevOUTLINE_State;

	static bool	Eclipse2Phase;
	static bool	EclipseParticle;
	static bool Eclipse2Can;
	static bool Explosion;
	static bool Eclipse;

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	virtual void awake();
	virtual int update();

	void MOONSkillstate();
	void EFFECTSkillstate();
	void PARTICLESkillstate();
	void SUNSETSkillstate();
	void OUTLINESkillstate();

	void SkillPos();
	void SkillChangeState();
	

	void findCS();

	SOULECLIPSE_FSM GetMOON_State() { return MOON_State; }
	SOULECLIPSE_FSM GetEFFECT_State() { return EFFECT_State; }
	SOULECLIPSE_FSM GetPARTICLE_State() { return PARTICLE_State; }
	SOULECLIPSE_FSM GetSUNSET_State() { return SUNSET_State; }
	SOULECLIPSE_FSM	GetOUTLINE_State() { return OUTLINE_State; }


public:
	CLONE(CSoulEclipseScript);

public:
	CSoulEclipseScript();
	virtual ~CSoulEclipseScript();
};

