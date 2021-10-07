#include "stdafx.h"
#include "LaserPointScript.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "AnrealEngine/Collider.h"
#include "ResMgr.h"

#include "AnrealEngine/CollisionMgr.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/define.h"

#include "AnrealEngine/Script.h"

#include "PlayerScript.h"
#include "PapulatusScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "LaserBodyScript.h"

CLaserPointScript::CLaserPointScript()
	: LASER_STATE(LASER_FSM::NONE)
	, Laser_Identifier(LASER_TYPE::NONE)
	, LaserIdentifierChecker(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::LASERPOINTSCRIPT);
}

CLaserPointScript::~CLaserPointScript()
{
}

void CLaserPointScript::awake()
{
}

void CLaserPointScript::LaserCheck()
{
	if (GetGameObject()->GetParent() == nullptr)
	{
		return;
	}

	if (LaserIdentifierChecker)
	{
		if (((CLaserBodyScript*)(GetGameObject()->GetParent()->GetScript()[0]))->Laser_Identifier != LASER_TYPE::NONE)
		{
			Laser_Identifier = ((CLaserBodyScript*)(GetGameObject()->GetParent()->GetScript()[0]))->Laser_Identifier;
		}

		LaserIdentifierChecker = false;
	}
}

void CLaserPointScript::LaserRot()
{
	Transform()->SetLocalRot(GetGameObject()->GetParent()->Transform()->GetLocalRot());
}

void CLaserPointScript::Skillstate()
{
	switch (LASER_STATE)
	{
	case LASER_FSM::NONE:
		break;

	case LASER_FSM::SUMMON:
		break;

	case LASER_FSM::ACTIVE:
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			Animator2D()->PlayAnim(L"1_LASER_POINT");
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			Animator2D()->PlayAnim(L"2_LASER_POINT");
		}
		break;

	case LASER_FSM::DIE:
		break;

	case LASER_FSM::END:
		break;

	default:
		break;
	}
}

void CLaserPointScript::SkillPos()
{
	if (LASER_STATE == LASER_FSM::NONE)
	{
		//	Transform()->SetLocalPos(DISABLE_POS);
		return;
	}
	else
	{
		vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();
		vPos.z -= 1;
		Transform()->SetLocalPos(vPos);
	}
}

void CLaserPointScript::SkillChangeState()
{
	if (GetGameObject()->GetParent() == nullptr)
	{
		return;
	}

	LASER_STATE = ((CLaserBodyScript*)(GetGameObject()->GetParent()->GetScript()[0]))->LASER_STATE;

	return;

}


int CLaserPointScript::update()
{
	//	CCollisionMgr::GetInst()->CollisionCheck(L"", L"PLAYER");
	LaserCheck();

	SkillPos();
	LaserRot();

	SkillChangeState();

	if (LASER_STATE != PREV_LASER_STATE)
	{
		Skillstate();
	}
	PREV_LASER_STATE = LASER_STATE;

	if (LASER_STATE != LASER_FSM::ACTIVE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}