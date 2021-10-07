#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CPlayerScript;
class CActiveExplosionScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vRot;

	static 	bool Active_Explosion;
	bool    Activation_Explosion;
	bool	PREV_Active_Explosion;

	CPlayerScript*	PlayerScriptptr;

	virtual void awake();
	virtual int update();

	void SkillPos();
	void Skillstate();
	void SkillChangeState();
	
	void AnimSection();
public:
	CLONE(CActiveExplosionScript);

public:
	CActiveExplosionScript();
	~CActiveExplosionScript();
};