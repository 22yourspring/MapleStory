#pragma once

#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CPlayerScript;
class CComboSub5Script :
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

	CPlayerScript*	PS;

	BUFF_FRONT	m_SkillState;

	virtual void awake();
	virtual int update();

	void Skillstate();
	void SkillPos();
	void SkillChangeState();
	void ThetaUpdate();
public:
	CLONE(CComboSub5Script);

public:
	CComboSub5Script();
	virtual ~CComboSub5Script();
};

