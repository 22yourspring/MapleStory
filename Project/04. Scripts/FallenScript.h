#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CFallenScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vRot;

	int				m_iEffectCount;
	int				m_iScriptCount;

	DIR				SKILL_DIR;

	PAPULATUS_OBJECT	OBJECT_State;
	PAPULATUS_OBJECT	PREV_OBJECTState;

	bool	PlayerCollisionReady;

	bool	Pattern1;

	Vec3	AreaUpPos;
	Vec3	AreaUpScale;

	Vec3 PlayerPos;

	float AddGravity;

	virtual void awake();
	virtual int update();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	void Skillstate();
	void SkillPos();
	void SkillChangeState();



public:
	CLONE(CFallenScript);

public:
	CFallenScript();
	~CFallenScript();
};