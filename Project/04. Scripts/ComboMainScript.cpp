#include "stdafx.h"
#include "ComboMainScript.h"


#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "ResMgr.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"

#include "AnrealEngine/CollisionMgr.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/define.h"

#include "AnrealEngine/Script.h"

#include "PlayerScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

CComboMainScript::CComboMainScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::COMBOMAINSCRIPT);
}

CComboMainScript::~CComboMainScript()
{
}

void CComboMainScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CComboMainScript::update()
{
	SkillPos();

	SkillChangeState();
	
	Skillstate();

	if (m_SkillState == COMBO_ATTACK::DISABLE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}

void CComboMainScript::Skillstate()
{
	switch (m_SkillState)
	{
	case COMBO_ATTACK::DISABLE:
		break;

	case COMBO_ATTACK::PHASE1:

		Animator2D()->PlayAnim(L"COMBO_STATE");
		break;
	
	case COMBO_ATTACK::PHASE2:
		
		Animator2D()->PlayAnim(L"ADVANCED_STATE");
		break;

	case COMBO_ATTACK::END:
		break;
		
	default:
		break;
	}
}

void CComboMainScript::SkillPos()
{
	if (CPlayerScript::PlayerObject == nullptr)
	{
		return;
	}

	for (UINT i = 0; i < CPlayerScript::PlayerObject->GetScript().size(); ++i)
	{
		vPos = CPlayerScript::GetPlayerPos();
		vPos.y += 4.f;
		vPos.z += 100.f;
		Transform()->SetLocalPos(vPos);
	}
}

void CComboMainScript::SkillChangeState()
{
	COMBO_COUNT = CPlayerScript::Player_COMBO_COUNT;


	if (CPlayerScript::COMBO_ON)
	{
		if (CPlayerScript::GetComboCount() <= 0)
		{
			m_SkillState = COMBO_ATTACK::DISABLE;
		}

		if (CPlayerScript::GetComboCount() >= 1 && CPlayerScript::GetComboCount() <= 5)
		{
			m_SkillState = COMBO_ATTACK::PHASE1;
			return;
		}

		if (CPlayerScript::GetComboCount() >= 6 && CPlayerScript::GetComboCount() <= 12)
		{
			m_SkillState = COMBO_ATTACK::PHASE2;
			return;
		}
	}


	if (CPlayerScript::GetComboCount() == 0)
	{
		m_SkillState = COMBO_ATTACK::DISABLE;
	}
}
