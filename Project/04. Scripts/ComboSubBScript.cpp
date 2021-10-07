#include "stdafx.h"
#include "ComboSubBScript.h"


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

CComboSubBScript::CComboSubBScript() :
	Theta (288.f * _DEGREE)
{
	SetScriptType((UINT)SCRIPT_TYPE::COMBOSUBBSCRIPT);
}

CComboSubBScript::~CComboSubBScript()
{
}

void CComboSubBScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CComboSubBScript::update()
{
	SkillPos();

	SkillChangeState();

	ThetaUpdate();

	Skillstate();

	if (m_SkillState == COMBO_SUB::DISABLE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}

void CComboSubBScript::Skillstate()
{
	switch (m_SkillState)
	{
	case COMBO_SUB::DISABLE:
		break;

	case COMBO_SUB::PHASE1:

		Animator2D()->PlayAnim(L"COMBO_B");
		break;

	case COMBO_SUB::PHASE2:

		Animator2D()->PlayAnim(L"ADVANCED_COMBO_B");
		break;

	case COMBO_SUB::END:
		break;

	default:
		break;
	}
}

void CComboSubBScript::SkillPos()
{
	if (CPlayerScript::PlayerObject == nullptr)
	{
		return;
	}

	for (UINT i = 0; i < CPlayerScript::PlayerObject->GetScript().size(); ++i)
	{
		vPos = CPlayerScript::GetPlayerPos();
		Vec3 Sub = { cos(Theta), sin(Theta), 0.f };

		Sub *= 50.f;
		Sub += vPos;
		Sub.z = CPlayerScript::GetPlayerPos().z - 99.f;

		Transform()->SetLocalPos(Sub);
	}
}

void CComboSubBScript::SkillChangeState()
{
	COMBO_COUNT = CPlayerScript::Player_COMBO_COUNT;

	
	if (COMBO_COUNT < 2)
	{
		m_SkillState = COMBO_SUB::DISABLE;
		return;
	}

	if (2 <= COMBO_COUNT && COMBO_COUNT <= 6)
	{
		m_SkillState = COMBO_SUB::PHASE1;
		return;
	}

	if (COMBO_COUNT >= 7)
	{
		m_SkillState = COMBO_SUB::PHASE2;
		return;
	}
}

void CComboSubBScript::ThetaUpdate()
{
	if (Theta == 360.f)
	{
		Theta = 0.f;
	}
	Theta += 2.5f * DT;
}
