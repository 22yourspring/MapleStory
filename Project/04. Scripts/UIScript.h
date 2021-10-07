#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"


class CUIScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3	vPos;
	Vec3	vScale;

	virtual void awake();
	virtual int update();

	bool	HasItem;

	void ProcessPos();
	void SetVisible();

public:
	CLONE(CUIScript);

public:
	CUIScript();
	~CUIScript();
};