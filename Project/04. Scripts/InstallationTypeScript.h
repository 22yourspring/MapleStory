#pragma once

#include "AnrealEngine/Script.h"

class CPlayerScript;
class CInstallationTypeScript :
	public CScript
{
private:
	Ptr<CPrefab>	m_pBullet;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;


public:
	virtual void awake();
	virtual int update();

	void FlashJumpSkillPos();
	void SkillChangeState();
	void CurDirSet();
	void Skillstate();

	Vec3 vPos;
	Vec3 vRot;

	INSTALLATION_TYPE	m_SkillState;
	INSTALLATION_TYPE	m_PrevSKILLState;



public:
	CLONE(CInstallationTypeScript);

public:
	CInstallationTypeScript();
	virtual ~CInstallationTypeScript();
};