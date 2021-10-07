#include "stdafx.h"
#include "ParticleScript.h"
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

#include "CameraScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

CParticleScript::CParticleScript()
	: PARTICLE_STATE(0)
	, PREV_PARTICLE_STATE(1)
	, PARTICLE_Identifier(0)
	, PARTICLEIdentifierChecker(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::PARTICLESCRIPT);
}

CParticleScript::~CParticleScript()
{
}


void CParticleScript::awake()
{
}

void CParticleScript::PARTICLECheck()
{
	if (PARTICLEIdentifierChecker)
	{
		if (GetGameObject()->GetName() == L"PARTICLE_1")
		{
			PARTICLE_Identifier = 0;
		}
		else if (GetGameObject()->GetName() == L"PARTICLE_2")
		{
			PARTICLE_Identifier = 1;
		}
		PARTICLEIdentifierChecker = false;
	}
}


void CParticleScript::Skillstate()
{
	if (PARTICLE_Identifier == 0)
	{
		Animator2D()->PlayAnim(L"toyCastleDungeon.img.ani.0.frames");
	}
	else if (PARTICLE_Identifier == 1)
	{
		Animator2D()->PlayAnim(L"toyCastleDungeon.img.ani.1.frames");
	}
}

void CParticleScript::SkillPos()
{
	CameraPos = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"CAM_COLL")->GetObjectVec()[0]->Transform()->GetLocalPos();

	vPos = Transform()->GetLocalPos();
	if (PARTICLE_Identifier == 0)
	{
		vPos.y += DT * 2.5f;
		vPos.x = CameraPos.x * 0.17f;
	}
	else
	{
		vPos.y += DT * 7.4f;
		vPos.x = CameraPos.x * 0.23f;
	}

	Transform()->SetLocalPos(vPos);
	return;
}


int CParticleScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"PARTICLE_SYSTEM", L"AREAUP");

	PARTICLECheck();
	SkillPos();

	if (PARTICLE_STATE != PREV_PARTICLE_STATE)
	{
		Skillstate();
	}
	PREV_PARTICLE_STATE = PARTICLE_STATE;

	return 0;
}

void CParticleScript::OnCollisionEnter(CCollider * _pOther)
{
}

void CParticleScript::OnCollision(CCollider * _pOther)
{
}

void CParticleScript::OnCollisionExit(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"AREAUP" || _pOther->GetGameObject()->GetName() == L"Area Up")
	{
		vPos = Transform()->GetLocalPos();
		vPos.y = -500.f;
		Transform()->SetLocalPos(vPos);
	}
}
