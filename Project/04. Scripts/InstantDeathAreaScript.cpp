#include "stdafx.h"
#include "InstantDeathAreaScript.h"

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
#include "EffectScript.h"

float CInstantDeathAreaScript::AreaOffAcc = 0.f;
bool CInstantDeathAreaScript::AreaingTime = false;
bool CInstantDeathAreaScript::PapulatusSignal = true;
bool CInstantDeathAreaScript::isAlive = false;
bool CInstantDeathAreaScript::EffectAction = true;

CInstantDeathAreaScript::CInstantDeathAreaScript()
	: EFFECT_SkillState(INSTANT_DEATH::NONE)
	, EndResetTime(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::INSTANTDEATHAREASCRIPT);
}

CInstantDeathAreaScript::~CInstantDeathAreaScript()
{
}



void CInstantDeathAreaScript::awake()
{
}

void CInstantDeathAreaScript::CountEffect()
{
	if (!EffectAction)
	{
		static float AreaCountAcc = 0.f;

		AreaCountAcc += DT * 0.01f;

		if (AreaCountAcc < 0.07f)
		{
			return;
		}
		AreaCountAcc = 0.f;

		EffectAction = true;
	}
}

void CInstantDeathAreaScript::Skillstate()
{
	switch (EFFECT_SkillState)
	{
	case INSTANT_DEATH::NONE:
		Animator2D()->PlayAnim(L"blink");
		break;

	case INSTANT_DEATH::SUMMON:
		Animator2D()->PlayAnim(L"Summon.img.91.frames");
		break;

	case INSTANT_DEATH::AREAING:
		Animator2D()->PlayAnim(L"8500009.img.stand.frames");
		break;

	case INSTANT_DEATH::DIE:
		Animator2D()->PlayAnim(L"8500009.img.die1.frames");
		break;

	case INSTANT_DEATH::END:
		break;

	default:
		break;
	}
}

void CInstantDeathAreaScript::SkillChangeState()
{
	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish() && EFFECT_SkillState == INSTANT_DEATH::SUMMON)
		{
			EFFECT_SkillState = INSTANT_DEATH::AREAING;
			AreaingTime = false;
			PapulatusSignal = false;
		}
		else if (EFFECT_SkillState == INSTANT_DEATH::AREAING && AreaingTime)
		{
			PapulatusSignal = false;
			EFFECT_SkillState = INSTANT_DEATH::DIE;
		}
		else if (Animator2D()->GetCurAnim()->IsFinish() && EFFECT_SkillState == INSTANT_DEATH::DIE)
		{
			EFFECT_SkillState = INSTANT_DEATH::NONE;
			AreaingTime = false;
			CLaserScript::Fir_FirLaser = true;

			//	PapulatusSignal = true;
		}
	}
	if (CPapulatusScript::CrackOfCheck && EFFECT_SkillState == INSTANT_DEATH::NONE)
	{
		EFFECT_SkillState = INSTANT_DEATH::SUMMON;
	}
	return;

}

void CInstantDeathAreaScript::CountCheck()
{
	if (!AreaingTime)
	{
		AreaOffAcc += DT;

		if (AreaOffAcc <= 60.f)
		{
			return;
		}
		EndResetTime = true;
		AreaingTime = true;
		AreaOffAcc = 0.f;
	}
}

void CInstantDeathAreaScript::PapulatusSignalCount()
{
	if (!PapulatusSignal && !AreaingTime && EndResetTime)
	{
		//		CLaserScript::Fir_FirLaser = true;
		static float SignalAcc = 0.f;

		SignalAcc += DT;

		if (SignalAcc <= 30.f)
		{
			return;
		}
		SignalAcc = 0.f;

		PapulatusSignal = true;
		EndResetTime = false;
	}
}

void CInstantDeathAreaScript::OnCollisionEnter(CCollider * _pOther)
{
}

void CInstantDeathAreaScript::OnCollision(CCollider * _pOther)
{
	if (EFFECT_SkillState == INSTANT_DEATH::AREAING)
	{
		if (_pOther->GetGameObject() == CPlayerScript::PlayerObject && EffectAction)
		{
			//	CPlayerScript::PlayerToScript->SubHP(); // 해당 부분 플레이어 업데이트에서 제대로 동작하는지 디버깅 확인 해야함. 그리고 이펙트 해당 장판 데미지 이펙트 출력 확인

			((CScript*)((CPlayerScript::PlayerObject)->GetScript()[0]))->SetSkillType((UINT)SKILL_TYPE::PAPULATUS_CRACK_OF_TIME);

			CPlayerScript::EffectPlayerPos = CPlayerScript::PlayerPos;
			CPlayerScript::EffectPlayerPos.y = CPlayerScript::EffectYvalue;

			if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
			{
				if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
				{
					CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
					CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

					CPlayerScript::PlayerObject->AddChild(EffectPtr);
					((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PAPULATUS_CRACK_OF_TIME;

					EffectAction = false;
				}
			}
		}
	}
}

void CInstantDeathAreaScript::OnCollisionExit(CCollider * _pOther)
{
}

int CInstantDeathAreaScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"SECTOR", L"PLAYER");

	SkillChangeState();

	CountCheck();
	PapulatusSignalCount();
	CountEffect();

	if (EFFECTPREV_SkillState != EFFECT_SkillState)
	{
		Skillstate();
	}
	EFFECTPREV_SkillState = EFFECT_SkillState;

	if (EFFECT_SkillState == INSTANT_DEATH::NONE)
	{
		isAlive = false;
	}
	else
	{
		isAlive = true;
	}
	return 0;
}