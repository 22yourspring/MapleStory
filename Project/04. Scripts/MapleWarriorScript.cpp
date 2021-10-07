#include "stdafx.h"
#include "MapleWarriorScript.h"

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

bool CMapleWarriorScript::m_bMAPLEWARRIOR_ON = false;
MAPLEWARRIOR CMapleWarriorScript::MAPLEWARRIOR_SkillState = MAPLEWARRIOR::DISABLE;
MAPLEWARRIOR CMapleWarriorScript::MAPLEWARRIOR_PrevSKILLState = MAPLEWARRIOR::DISABLE;

CMapleWarriorScript::CMapleWarriorScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::MAPLEWARRIORSCRIPT);
}

CMapleWarriorScript::~CMapleWarriorScript()
{
}

void CMapleWarriorScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CMapleWarriorScript::update()
{
	SkillPos();

	SkillChangeState();

	if (MAPLEWARRIOR_PrevSKILLState != MAPLEWARRIOR_SkillState)
	{
		Skillstate();
	}
	MAPLEWARRIOR_PrevSKILLState = MAPLEWARRIOR_SkillState;

	if (m_bMAPLEWARRIOR_ON = true)
	{
		CPlayerScript::MAPLESOLDIER = 30;
	}
	else
	{
		CPlayerScript::MAPLESOLDIER = 0;
	}

	if (MAPLEWARRIOR_SkillState == MAPLEWARRIOR::DISABLE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}


	return 0;
}

void CMapleWarriorScript::Skillstate()
{
	switch (MAPLEWARRIOR_SkillState)
	{
	case MAPLEWARRIOR::DISABLE:

		if (CPlayerScript::m_PlayerState != FSM::FLASHJUMP && CPlayerScript::m_PlayerState != FSM::ROPE && CPlayerScript::m_PlayerState != FSM::LADDER)
		{
			CPlayerScript::m_PlayerState = FSM::STAND;
		}

		break;

	case MAPLEWARRIOR::ENABLE:
		CurDirSet();
		Animator2D()->PlayAnim(L"21121000.effect.frames");
		break;


	case MAPLEWARRIOR::END:
		break;

	default:
		break;
	}
}

void CMapleWarriorScript::SkillPos()
{
	PlayerDir = CPlayerScript::GetPlayerDir();


	if (PlayerDir == DIR::RIGHT)
	{
		vPos = CPlayerScript::GetPlayerPos();
		vPos.x -= 9.f;
		vPos.y += 125.f;
		vPos.z += 100.f;
		Transform()->SetLocalPos(vPos);
		return;
	}
	else
	{
		vPos = CPlayerScript::GetPlayerPos();
		vPos.x += 10.f;
		vPos.y += 125.f;
		vPos.z += 100.f;
		Transform()->SetLocalPos(vPos);
		return;
	}

}

void CMapleWarriorScript::SkillChangeState()
{
	PlayerState = CPlayerScript::GetPlayerState();

	if (KEYTAB(KEY_TYPE::KEY_G) && PlayerState != FSM::BUFF && PlayerState != FSM::SHOUT  && PlayerState != FSM::DEATHFAULT && PlayerState != FSM::SHADOWFLASH_BETA && PlayerState != FSM::SHADOWFLASH_ALPHA && CPlayerScript::isCanControl)
	{
		if (PlayerState == FSM::PRONE)
		{
			return;
		}

		if (PlayerState != FSM::FLASHJUMP)
		{
			if (MAPLEWARRIOR_SkillState == MAPLEWARRIOR::DISABLE)
			{
				MAPLEWARRIOR_SkillState = MAPLEWARRIOR::ENABLE;
				
				m_bMAPLEWARRIOR_ON = true;
			}
			return;
		}
	}


	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			MAPLEWARRIOR_SkillState = MAPLEWARRIOR::DISABLE;
		}
	}
	return;
}

void CMapleWarriorScript::CurDirSet()
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
