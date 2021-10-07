#include "stdafx.h"
#include "TimeBombScript.h"

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
#include "EffectScript.h"
#include "PapulatusScript.h"

CTimeBombScript::CTimeBombScript()
	: OBJECT_State(false)
	, ActiveAcc(0.f)
	, TimeSec(0.f)
	, TakeTimeBombs(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::TIMEBOMBSCRIPT);
}


CTimeBombScript::~CTimeBombScript()
{
}


void CTimeBombScript::OnCollisionEnter(CCollider * _pOther)
{

}

void CTimeBombScript::OnCollision(CCollider * _pOther)
{
	if (_pOther->GetGameObject() == CPlayerScript::PlayerObject)
	{
		if (OBJECT_State && Animator2D()->GetCurAnim()->GetCurFrm() == (Animator2D()->GetCurAnim()->GetFrmVec().size() - 2))
		{
			((CScript*)((CPlayerScript::PlayerObject)->GetScript()[0]))->SetSkillType((UINT)SKILL_TYPE::PAPULATUS_TIME_BOMB);

			CPlayerScript::EffectPlayerPos = CPlayerScript::GetPlayerPos();
			CPlayerScript::EffectPlayerPos.y = CPlayerScript::EffectYvalue;

			if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
			{
				if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
				{
					CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
					CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

					CPlayerScript::PlayerObject->AddChild(EffectPtr);
					((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PAPULATUS_TIME_BOMB;

				}
			}
		}
	}
}

void CTimeBombScript::OnCollisionExit(CCollider * _pOther)
{

}


void CTimeBombScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

void CTimeBombScript::Skillstate()
{
	if (OBJECT_State)
	{
		Animator2D()->PlayAnim(L"8500021.img.attack1.info.areaWarning.frames");
	}
	else
	{
		Animator2D()->PlayAnim(L"blink");
	}
}

void CTimeBombScript::SkillChangeState()
{
	if (!TakeTimeBombs)
	{
		TakeTimeBombs = CPapulatusScript::TimeBombsOn;
		int Devided_TimeSec = Well512Random::Instance().GetValue(0, 4);

		switch (Devided_TimeSec)
		{
		case 0:
			TimeSec = 0.1f * DT;
			break;
		case 1:
			TimeSec = 0.2f * DT;
			break;
		case 2:
			TimeSec = 0.3f * DT;
			break;
		case 3:
			TimeSec = 0.4f * DT;
			break;
		case 4:
			TimeSec = 0.5f * DT;
			break;
		default:
			break;
		}
	}

	if (TakeTimeBombs)
	{
		if (!CPapulatusScript::TimeBombsOn)
		{
//			OBJECT_State = false;

			TakeTimeBombs = false;
			ActiveAcc = 0.f;
			TimeSec = 0.f;
			return;

		}
		ActiveAcc += TimeSec;

		if (ActiveAcc <= 0.2f)
		{
			return;
		}
		OBJECT_State = true;

		TakeTimeBombs = false;
		ActiveAcc = 0.f;
		TimeSec =0.f;
	}


	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish() && OBJECT_State)
		{
			OBJECT_State = false;
		}
	}
	return;
}

int CTimeBombScript::update()
{
	SkillChangeState();

	if (PREV_OBJECTState != OBJECT_State)
	{
		Skillstate();
	}
	PREV_OBJECTState = OBJECT_State;


	if (!OBJECT_State)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}