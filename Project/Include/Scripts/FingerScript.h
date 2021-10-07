#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CFingerScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vScale;

	virtual void awake();
	virtual int update();

	void ProcessPos();

	void state();
public:
	CLONE(CFingerScript);

public:
	CFingerScript();
	~CFingerScript();
};