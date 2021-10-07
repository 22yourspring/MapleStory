#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CInstantDeathAreaScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vRot;
	
	INSTANT_DEATH	EFFECT_SkillState;
	INSTANT_DEATH	EFFECTPREV_SkillState;
	
	static bool AreaingTime;
	static bool PapulatusSignal;
	static float AreaOffAcc;

	static bool EffectAction;
	
	virtual void awake();
	virtual int update();

	bool EndResetTime;
	void CountEffect();

	void Skillstate();
	void SkillChangeState();

	static bool isAlive;

	void CountCheck();
	void PapulatusSignalCount();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

public:
	CLONE(CInstantDeathAreaScript);

public:
	CInstantDeathAreaScript();
	~CInstantDeathAreaScript();
};