#include "stdafx.h"
#include "InstinctActiveScript.h"
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
#include "InstinctScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

INSTINCT_FSM CInstinctActiveScript::INSTINCTACTIVE_SkillState = INSTINCT_FSM::DISABLE;
INSTINCT_FSM CInstinctActiveScript::INSTINCTACTIVE_PrevSKILLState = INSTINCT_FSM::DISABLE;

CInstinctActiveScript::CInstinctActiveScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::INSTINCTACTIVESCRIPT);
}

CInstinctActiveScript::~CInstinctActiveScript()
{
}

void CInstinctActiveScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CInstinctActiveScript::update()
{
	SkillPos();

	SkillChangeState();

	if (INSTINCTACTIVE_PrevSKILLState != INSTINCTACTIVE_SkillState)
	{
		Skillstate();
	}
	INSTINCTACTIVE_PrevSKILLState = INSTINCTACTIVE_SkillState;

	if (INSTINCTACTIVE_SkillState == INSTINCT_FSM::DISABLE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}

void CInstinctActiveScript::Skillstate()
{
	switch (INSTINCTACTIVE_SkillState)
	{
	case INSTINCT_FSM::DISABLE:

		if (CPlayerScript::m_PlayerState != FSM::FLASHJUMP && CPlayerScript::m_PlayerState != FSM::ROPE && CPlayerScript::m_PlayerState != FSM::LADDER)
		{
			CPlayerScript::m_PlayerState = FSM::STAND;
		}

		break;

	case INSTINCT_FSM::ENABLE:
		Animator2D()->PlayAnim(L"400011073.effect.frames");
		break;


	case INSTINCT_FSM::END:
		break;

	default:
		break;
	}
}

void CInstinctActiveScript::SkillPos()
{
	vPos = CPlayerScript::GetPlayerPos();
	vPos.y += 50.f;
	vPos.z -= 300.f;
	Transform()->SetLocalPos(vPos);
}

void CInstinctActiveScript::SkillChangeState()
{
	PlayerState = CPlayerScript::GetPlayerState();

	if (KEYTAB(KEY_TYPE::KEY_F) && PlayerState != FSM::BUFF && PlayerState != FSM::SHOUT  && PlayerState != FSM::DEATHFAULT && PlayerState != FSM::SHADOWFLASH_BETA && PlayerState != FSM::SHADOWFLASH_ALPHA && CPlayerScript::isCanControl)
	{
		if (PlayerState == FSM::PRONE)
		{
			return;
		}

		if (PlayerState != FSM::FLASHJUMP)
		{
			if (INSTINCTACTIVE_SkillState == INSTINCT_FSM::DISABLE)
			{
				INSTINCTACTIVE_SkillState = INSTINCT_FSM::ENABLE;
				CInstinctScript::m_bInstinctActive = true;
				CInstinctScript::PrevConfirm = false;
				return;
			}
		}
	}


	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			INSTINCTACTIVE_SkillState = INSTINCT_FSM::DISABLE;
		}
	}
	return;
}

