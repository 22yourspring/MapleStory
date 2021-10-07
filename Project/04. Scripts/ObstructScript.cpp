#include "stdafx.h"
#include "ObstructScript.h"
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

#include "LaserScript.h"
#include "LaserBodyScript.h"

bool CObstructScript::RedObstruct = false;
bool CObstructScript::BlueObstruct = false;

CObstructScript::CObstructScript()
	: LASER_STATE(LASER_FSM::NONE)
	, Laser_Identifier(LASER_TYPE::NONE)
	, LaserIdentifierChecker(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::OBSTRUCTSCRIPT);
}

CObstructScript::~CObstructScript()
{
}

void CObstructScript::awake()
{
}

void CObstructScript::LaserCheck()
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

void CObstructScript::Skillstate()
{
	switch (LASER_STATE)
	{
	case LASER_FSM::NONE:
		break;
	case LASER_FSM::OBSTRUCT:
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			Animator2D()->PlayAnim(L"1_Obstruct");
		}
		else
		{
			Animator2D()->PlayAnim(L"2_Obstruct");
		}
		break;
	case LASER_FSM::END:
		break;
	default:
		break;
	}
}

void CObstructScript::SkillPos()
{
	vPos = CPlayerScript::PlayerPos;
	if (Laser_Identifier == LASER_TYPE::RED_LEFT)
	{
		vPos.x -= 8.f;
		vPos.y -= 10.f;
		vPos.z -= 300.f;
	}
	else
	{
		vPos.x += 8.f;
		vPos.y -= 10.f;
		vPos.z -= 300.f;
	}

	Transform()->SetLocalPos(vPos);
}

void CObstructScript::SkillChangeState()
{
	if (GetGameObject()->GetParent() == nullptr)
	{
		return;
	}

	
	if (Laser_Identifier == LASER_TYPE::RED_LEFT)
	{
		if (CLaserScript::RedCutting)
		{
			LASER_STATE = LASER_FSM::OBSTRUCT;
			RedObstruct = true;
		}
		else
		{
			LASER_STATE = LASER_FSM::NONE;
			RedObstruct = false;
		}
	}
	else
	{
		if (CLaserScript::BlueCutting)
		{
			LASER_STATE = LASER_FSM::OBSTRUCT;
			BlueObstruct = true;
		}
		else
		{
			LASER_STATE = LASER_FSM::NONE;
			BlueObstruct = false;
		}
	}

	return;

}


int CObstructScript::update()
{
	LaserCheck();

	SkillPos();

	SkillChangeState();

	if (LASER_STATE != PREV_LASER_STATE)
	{
		Skillstate();
	}
	PREV_LASER_STATE = LASER_STATE;

	if (LASER_STATE != LASER_FSM::OBSTRUCT)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}