#include "stdafx.h"
#include "LaserBodyScript.h"

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

#include "ObstructScript.h"
#include "ActiveExplosionScript.h"

bool CLaserBodyScript::RedColl = false;
bool CLaserBodyScript::BlueColl = false;

CLaserBodyScript::CLaserBodyScript()
	: LASER_STATE(LASER_FSM::NONE)
	, Laser_Identifier(LASER_TYPE::NONE)
	, LaserIdentifierChecker(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::LASERBODYSCRIPT);
}

CLaserBodyScript::~CLaserBodyScript()
{
}

void CLaserBodyScript::awake()
{
}

void CLaserBodyScript::LaserCheck()
{
	if (GetGameObject()->GetParent() == nullptr)
	{
		return;
	}

	if (LaserIdentifierChecker)
	{
		if (((CLaserScript*)(GetGameObject()->GetParent()->GetScript()[0]))->Laser_Identifier != LASER_TYPE::NONE)
		{
			Laser_Identifier = ((CLaserScript*)(GetGameObject()->GetParent()->GetScript()[0]))->Laser_Identifier;
		}

		LaserIdentifierChecker = false;
	}
}

void CLaserBodyScript::LaserRot()
{
	Transform()->SetLocalRot(GetGameObject()->GetParent()->Transform()->GetLocalRot());
}

void CLaserBodyScript::Skillstate()
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
			Animator2D()->PlayAnim(L"1_LASER_BODY");
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			Animator2D()->PlayAnim(L"2_LASER_BODY");
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

void CLaserBodyScript::SkillPos()
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

void CLaserBodyScript::SkillChangeState()
{
	if (GetGameObject()->GetParent() == nullptr)
	{
		return;
	}

	LASER_STATE = ((CLaserScript*)(GetGameObject()->GetParent()->GetScript()[0]))->LASER_STATE;

	return;

}


void CLaserBodyScript::OnCollisionEnter(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"Player Collider" || _pOther->GetGameObject()->GetName() == L"PLAYER_COLLIDER")
	{
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
		}
	}
}

void CLaserBodyScript::OnCollision(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"Player Collider" || _pOther->GetGameObject()->GetName() == L"PLAYER_COLLIDER")
	{
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			CLaserBodyScript::RedColl = true;

			vPos = Transform()->GetLocalPos();

			if (vPos.x <= CPlayerScript::PlayerPos.x && CLaserScript::RedCollReady)
			{
				vScale = Transform()->GetLocalScale();

				float m_fweight = fabs(vPos.x - CPlayerScript::PlayerPos.x);
				float m_fheight = fabs(vPos.y - CPlayerScript::PlayerPos.y);

				m_fweight *= m_fweight;
				m_fheight *= m_fheight;

				float length = sqrt(m_fweight + m_fheight);

				vScale.y = length * 2.25f;

				Transform()->SetLocalScale(vScale);

				CLaserScript::RedCutting = true;
			}
			else if (vPos.x > CPlayerScript::PlayerPos.x && !CLaserScript::RedCollReady)
			{
				vScale = Transform()->GetLocalScale();

				float m_fweight = fabs(vPos.x - CPlayerScript::PlayerPos.x);
				float m_fheight = fabs(vPos.y - CPlayerScript::PlayerPos.y);

				m_fweight *= m_fweight;
				m_fheight *= m_fheight;

				float length = sqrt(m_fweight + m_fheight);

				vScale.y = length * 2.25f;

				Transform()->SetLocalScale(vScale);

				CLaserScript::RedCutting = true;
			}
			else
			{
				CLaserScript::RedCutting = false;
			}
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			CLaserBodyScript::BlueColl = true;

			vPos = Transform()->GetLocalPos();

			if (CPlayerScript::PlayerPos.x <= vPos.x && CLaserScript::BlueCollReady) // 캐릭보다 BLUE가 오른쪽에 있다면
			{
				vScale = Transform()->GetLocalScale();

				float m_fweight = fabs(vPos.x - CPlayerScript::PlayerPos.x);
				float m_fheight = fabs(vPos.y - CPlayerScript::PlayerPos.y);

				m_fweight *= m_fweight;
				m_fheight *= m_fheight;

				float length = sqrt(m_fweight + m_fheight);

				vScale.y = length * 2.20f;

				Transform()->SetLocalScale(vScale);

				CLaserScript::BlueCutting = true;
			}
			else if (CPlayerScript::PlayerPos.x > vPos.x && !CLaserScript::BlueCollReady)
			{
				vScale = Transform()->GetLocalScale();

				float m_fweight = fabs(vPos.x - CPlayerScript::PlayerPos.x);
				float m_fheight = fabs(vPos.y - CPlayerScript::PlayerPos.y);

				m_fweight *= m_fweight;
				m_fheight *= m_fheight;

				float length = sqrt(m_fweight + m_fheight);

				vScale.y = length * 2.20f;

				Transform()->SetLocalScale(vScale);

				CLaserScript::BlueCutting = true;
			}
			else
			{
				CLaserScript::BlueCutting = false;
			}
		}
	}
	if (CLaserScript::CollisionReady)
	{
		if (((CLaserScript*)(GetGameObject()->GetParent()->GetScript()[0]))->LASER_STATE != LASER_FSM::ACTIVE)
		{
			return;
		}

		if ((SCRIPT_TYPE)(_pOther->GetGameObject()->GetScript()[0]->GetScriptType()) == SCRIPT_TYPE::LASERBODYSCRIPT)
		{
			if (!CObstructScript::RedObstruct && !CObstructScript::BlueObstruct)
			{
				((CLaserScript*)(GetGameObject()->GetParent()->GetScript()[0]))->LASER_STATE = LASER_FSM::DIE;

				CActiveExplosionScript::Active_Explosion = true;
			}
		}
	}
}

void CLaserBodyScript::OnCollisionExit(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"Player Collider" || _pOther->GetGameObject()->GetName() == L"PLAYER_COLLIDER")
	{
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			CLaserBodyScript::RedColl = false;
			CLaserScript::RedCutting = false;

			Transform()->SetLocalScale(GetGameObject()->GetParent()->Transform()->GetLocalScale());
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			CLaserBodyScript::BlueColl = false;
			CLaserScript::BlueCutting = false;

			Transform()->SetLocalScale(GetGameObject()->GetParent()->Transform()->GetLocalScale());
		}
	}
}

int CLaserBodyScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"LEFT_LASER", L"RIGHT_LASER");

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