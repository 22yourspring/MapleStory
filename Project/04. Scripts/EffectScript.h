#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CPlayerScript;
class CEffectScript :
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

	int				EffectSequence;

	DIR				SKILL_DIR;

	CPlayerScript*	PS;

	SKILL_TYPE	EFFECT_SkillState;
	SKILL_TYPE	EFFECTPREV_SkillState;
	
	Vec3 PlayerPos;

	virtual void awake();
	virtual int update();

	void Skillstate();
	void SkillPos();
	void SkillChangeState();
	void CurDirSet();

	bool LeftFromPlayer();



public:
	CLONE(CEffectScript);

public:
	CEffectScript();
	~CEffectScript();
};