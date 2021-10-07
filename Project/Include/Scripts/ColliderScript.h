#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CColliderScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3	vPos;


	virtual void awake();
	virtual int update();

	void SkillPos();

public:
	CLONE(CColliderScript);

public:
	CColliderScript();
	~CColliderScript();
};