#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CParticleScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vRot;
	Vec3			CameraPos;

	int	PARTICLE_STATE;
	int	PREV_PARTICLE_STATE;

	bool	PARTICLEIdentifierChecker;
	int		PARTICLE_Identifier;

	virtual void awake();
	virtual int update();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	void PARTICLECheck();

	void Skillstate();
	void SkillPos();

public:
	CLONE(CParticleScript);

public:
	CParticleScript();
	~CParticleScript();
};