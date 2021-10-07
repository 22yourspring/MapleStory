#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CPlayerScript;
class CComboSubAScript :
	public CScript
{
private:
	Ptr<CPrefab>	m_pBullet;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;


public:
	float		Theta;

	Vec3			vPos;
	Vec3			vRot;
	UINT			COMBO_COUNT;


	COMBO_SUB	m_SkillState;

	virtual void awake();
	virtual int update();

	void Skillstate();
	void SkillPos();
	void SkillChangeState();
	void ThetaUpdate();
public:
	CLONE(CComboSubAScript);

public:
	CComboSubAScript();
	virtual ~CComboSubAScript();
};

