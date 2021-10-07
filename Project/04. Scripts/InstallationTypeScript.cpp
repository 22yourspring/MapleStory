#include "stdafx.h"
#include "InstallationTypeScript.h"

#include "ScriptMgr.h"
#include "AnrealEngine/ResMgr.h"

#include "AnrealEngine/Transform.h"
#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/MeshRender.h"
#include "PlayerScript.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/KeyMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/Layer.h"


CInstallationTypeScript::CInstallationTypeScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::INSTALLATIONTYPESCRIPT);
}


CInstallationTypeScript::~CInstallationTypeScript()
{
}



void CInstallationTypeScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CInstallationTypeScript::update()
{
	FlashJumpSkillPos();
	SkillChangeState();


	if (m_PrevSKILLState != m_SkillState)
	{
		Skillstate();
	}
	m_PrevSKILLState = m_SkillState;

	if (m_SkillState == INSTALLATION_TYPE::NONE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}


	return 0;
}

void CInstallationTypeScript::Skillstate()
{
	switch (m_SkillState)
	{
	case INSTALLATION_TYPE::NONE:
		FlashJumpSkillPos();
		break;

	case INSTALLATION_TYPE::LEAP:
		CurDirSet();
		FlashJumpSkillPos();
		Animator2D()->PlayAnim(L"25001204.effect.frames");
		break;

	case INSTALLATION_TYPE::END:
		break;

	default:
		break;
	}

}


void CInstallationTypeScript::FlashJumpSkillPos()
{
	if (CPlayerScript::GetPlayerState() == FSM::JUMP)
	{
		if (CPlayerScript::GetPlayerDir() == DIR::RIGHT)
		{
			vPos = CPlayerScript::GetPlayerPos();
			vPos.x = CPlayerScript::GetPlayerPos().x - 100.f;
		}
		else if (CPlayerScript::GetPlayerDir() == DIR::LEFT)
		{
			vPos = CPlayerScript::GetPlayerPos();
			vPos.x = CPlayerScript::GetPlayerPos().x + 100.f;
		}
		

	}
	Transform()->SetLocalPos(vPos);
	
}

void CInstallationTypeScript::SkillChangeState()
{
	if (CPlayerScript::GetPlayerState() == FSM::FLASHJUMP)
	{
		m_SkillState = INSTALLATION_TYPE::LEAP;
		return;
	}	
	
	if (m_SkillState == INSTALLATION_TYPE::LEAP)
	{
		if (CPlayerScript::GetPlayerState() != FSM::FLASHJUMP)
		{
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				m_SkillState = INSTALLATION_TYPE::NONE;

			}
		}	
	}

	

}

void CInstallationTypeScript::CurDirSet()
{
	switch (CPlayerScript::GetPlayerDir())
	{
	case DIR::RIGHT:
		vRot = Transform()->GetLocalRot();
		vRot = { 0.f, PI, 0.f };

		Transform()->SetLocalRot(vRot);
		break;

	case DIR::LEFT:
		vRot = Transform()->GetLocalRot();
		vRot = { 0.f, 0.f, 0.f };

		Transform()->SetLocalRot(vRot);
		break;

	default:
		break;
	}
}
