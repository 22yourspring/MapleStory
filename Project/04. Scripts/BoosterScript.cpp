#include "stdafx.h"
#include "BoosterScript.h"

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

BOOSTER CBoosterScript::BOOSTER_SkillState = BOOSTER::DISABLE;
BOOSTER CBoosterScript::BOOSTER_PrevSKILLState = BOOSTER::DISABLE;

CBoosterScript::CBoosterScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::BOOSTERSCRIPT);
}

CBoosterScript::~CBoosterScript()
{
}

void CBoosterScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CBoosterScript::update()
{
	SkillPos();

	SkillChangeState();

	if (BOOSTER_PrevSKILLState != BOOSTER_SkillState)
	{
		Skillstate();
	}
	BOOSTER_PrevSKILLState = BOOSTER_SkillState;

	if (BOOSTER_SkillState == BOOSTER::DISABLE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}

void CBoosterScript::Skillstate()
{
	switch (BOOSTER_SkillState)
	{
	case BOOSTER::DISABLE:
		CPlayerScript::m_PlayerState = FSM::STAND;
		break;

	case BOOSTER::ENABLE:
		CurDirSet();
		Animator2D()->PlayAnim(L"1101004.effect.frames");
		break;


	case BOOSTER::END:
		break;

	default:
		break;
	}
}

void CBoosterScript::SkillPos()
{
	PlayerDir = CPlayerScript::GetPlayerDir();


	if (PlayerDir == DIR::RIGHT)
	{
		vPos = CPlayerScript::GetPlayerPos();
		vPos.y += 50.f;
		Transform()->SetLocalPos(vPos);
		return;
	}
	else
	{
		vPos = CPlayerScript::GetPlayerPos();
		vPos.y += 50.f;
		Transform()->SetLocalPos(vPos);
		return;
	}

}

void CBoosterScript::SkillChangeState()
{
	PlayerState = CPlayerScript::GetPlayerState();

	if (KEYTAB(KEY_TYPE::KEY_W) && PlayerState != FSM::BUFF && PlayerState != FSM::SHOUT  && PlayerState != FSM::DEATHFAULT && PlayerState != FSM::SHADOWFLASH_BETA && PlayerState != FSM::SHADOWFLASH_ALPHA && CPlayerScript::isCanControl)
	{
		if (PlayerState == FSM::PRONE)
		{
			return;
		}

		if (PlayerState != FSM::FLASHJUMP)
		{
			if (BOOSTER_SkillState == BOOSTER::DISABLE)
			{
				BOOSTER_SkillState = BOOSTER::ENABLE;
			}
			return;
		}
	}


	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			BOOSTER_SkillState = BOOSTER::DISABLE;
		}
	}
	return;
}

void CBoosterScript::CurDirSet()
{
	switch (PlayerDir)
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
