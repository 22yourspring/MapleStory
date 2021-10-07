#include "stdafx.h"
#include "CraneScript.h"

#include "AnrealEngine/CollisionMgr.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "AnrealEngine/Collider.h"
#include "ResMgr.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/define.h"

#include "AnrealEngine/Script.h"

#include "AreaScript.h"

#include "PlayerScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "PapulatusScript.h"

bool CCraneScript::CraneAction = false;

CCraneScript::CCraneScript()
	: OBJECT_State(CRANE_FSM::IDLE)
	, CurDIR(CRANE_DIR::IDLE)
	, Script_start(true)
	, Phase1Start(false)
	, m_fIdle_Up(0.f)
	, m_fIdle_Down(0.f)
	, FirstTime(true)
	, AccessTime ((float)(Well512Random::Instance().GetValue(0,20)))
{
	SetScriptType((UINT)SCRIPT_TYPE::CRANESCRIPT);
}


CCraneScript::~CCraneScript()
{
}


void CCraneScript::OnCollisionEnter(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
	{
		CurDIR = CRANE_DIR::HOLD;
	}
	else if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		CurDIR = CRANE_DIR::HOLD;
	}
	if (_pOther->GetGameObject()->GetName() == L"PLAYER_CraneColl")
	{
		if (true)
		{

		}
		
		if (CurDIR != CRANE_DIR::UP)
		{
			CurDIR = CRANE_DIR::HOLD;
			OBJECT_State = CRANE_FSM::HOLD;
			BindingPlayer();
		}
	}
}

void CCraneScript::OnCollision(CCollider * _pOther)
{
	/*if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
	{
		if (PrevDir != CRANE_DIR::UP)
		{
			CurDIR = CRANE_DIR::HOLD;
		}
	}
	else if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		if (PrevDir != CRANE_DIR::UP)
		{
			CurDIR = CRANE_DIR::HOLD;
		}
	}
	if (_pOther->GetGameObject()->GetName() == L"PLAYER")
	{
		CurDIR = CRANE_DIR::HOLD;
		OBJECT_State = CRANE_FSM::HOLD;
	}*/
}

void CCraneScript::OnCollisionExit(CCollider * _pOther)
{

}




void CCraneScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

void CCraneScript::Skillstate()
{
	switch (OBJECT_State)
	{
	case CRANE_FSM::IDLE:
		Animator2D()->PlayAnim(L"Idle");
		break;
	case CRANE_FSM::HOLD:
		Animator2D()->PlayAnim(L"Hold");
		break;
	case CRANE_FSM::END:
		break;
	default:
		break;
	}
}

void CCraneScript::SkillPos()
{
	vPos = Transform()->GetLocalPos();

	if (CurDIR == CRANE_DIR::DOWN && OBJECT_State == CRANE_FSM::IDLE)
	{
		vPos.y -= DT * 80.f;
	}
	else if (CurDIR == CRANE_DIR::UP && OBJECT_State == CRANE_FSM::IDLE)
	{
		vPos.y += DT * 80.f;
	}

	if (vPos.y > startPos.y)
	{
		CurDIR = CRANE_DIR::IDLE;
	}
	vPos.z = CPlayerScript::PlayerPos.z + 1.f;

	Transform()->SetLocalPos(vPos);
}

void CCraneScript::SkillChangeState()
{
	if (CPapulatusScript::Phase1Start && Phase1Start && CurDIR != CRANE_DIR::UP && FirstTime)
	{
		CurDIR = CRANE_DIR::DOWN;
		FirstTime = false;
	}

	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish() && OBJECT_State == CRANE_FSM::HOLD)
		{
			OBJECT_State = CRANE_FSM::IDLE;
			CurDIR = CRANE_DIR::UP;

			((CPlayerScript*)(CPlayerScript::PlayerToScript))->SetLadderGravity(true);
			CraneAction = false;
			CPlayerScript::isCanControl = true;
		}
	}
	return;
}


void CCraneScript::saveStartPos()
{
	if (Script_start)
	{
		startPos = Transform()->GetLocalPos();
		Script_start = false;
	}
}

void CCraneScript::IdleUpAcc()
{
	if (CurDIR == CRANE_DIR::IDLE && CPapulatusScript::Phase1Start && Phase1Start && !FirstTime)
	{
		m_fIdle_Up += (DT * Well512Random::Instance().GetFloatValue(0.3f, 0.7f));
		
		if (m_fIdle_Up <= 6.f)
		{
			return;
		}
		m_fIdle_Up = 0.f;
		CurDIR = CRANE_DIR::DOWN;
	}
}

void CCraneScript::IdleDownAcc()
{
	if (CurDIR == CRANE_DIR::HOLD && CPapulatusScript::Phase1Start && Phase1Start && !FirstTime)
	{
		m_fIdle_Down += (float)DT;

		if (m_fIdle_Down <= 2.f)
		{
			return;
		}
		m_fIdle_Down = 0.f;
		CurDIR = CRANE_DIR::UP;
	}
}

void CCraneScript::BindingPlayer()
{
	vPos = Transform()->GetLocalPos();
	vPos.y -= 40.f;
	vPos.z = CPlayerScript::PlayerPos.z;

	CPlayerScript::PlayerObject->Transform()->SetLocalPos(vPos);
	((CPlayerScript*)(CPlayerScript::PlayerToScript)) -> SetLadderGravity(false);
	CraneAction = true;
	CPlayerScript::isCanControl = false;
	CPlayerScript::SetPlayerState(FSM::JUMP);
}

void CCraneScript::F_Time()
{
	if (!Phase1Start && CPapulatusScript::Phase1Start)
	{
		AccessTime += DT;

		if (AccessTime <= 20.f)
		{
			return;
		}
		Phase1Start = true;
	}
}


int CCraneScript::update()
{
	saveStartPos();
	F_Time();

	SkillPos();
	SkillChangeState();

	IdleUpAcc();
	IdleDownAcc();

	if (PREV_OBJECTState != OBJECT_State)
	{
		Skillstate();
	}
	PREV_OBJECTState = OBJECT_State;
	PrevDir = CurDIR;

	return 0;
}