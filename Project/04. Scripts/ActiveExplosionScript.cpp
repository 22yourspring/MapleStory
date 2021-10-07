#include "stdafx.h"
#include "ActiveExplosionScript.h"

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

#include "PapulatusScript.h"
#include "InstantDeathAreaScript.h"
#include "LaserBodyScript.h"
#include "EffectScript.h"

bool CActiveExplosionScript::Active_Explosion = false;

CActiveExplosionScript::CActiveExplosionScript()
	: PlayerScriptptr(nullptr)
	, Activation_Explosion(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::ACTIVEEXPLOSIONSCRIPT);
}

CActiveExplosionScript::~CActiveExplosionScript()
{
}

void CActiveExplosionScript::awake()
{
}

void CActiveExplosionScript::SkillPos()
{
	vPos = { 0.f, 0.f, 0.f };
	Transform()->SetLocalPos(vPos);
}

void CActiveExplosionScript::Skillstate()
{
	if (Activation_Explosion)
	{
		Animator2D()->PlayAnim(L"Active_Explosion");
	}
	else
	{
		Animator2D()->PlayAnim(L"blink");
	}
}


void CActiveExplosionScript::SkillChangeState()
{
	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Active_Explosion)
		{
			Activation_Explosion = true;
		}

		if (Animator2D()->GetCurAnim()->IsFinish() && Active_Explosion)
		{
			Animator2D()->GetCurAnim()->SetFrmIdx(0);
			Active_Explosion = false;
			Activation_Explosion = false;
		}
	}
}

void CActiveExplosionScript::AnimSection()
{
	if (CPlayerScript::PlayerObject == nullptr)
	{
		return;
	}
	if (PlayerScriptptr == nullptr)
	{
		for (size_t i = 0; i < CPlayerScript::PlayerObject->GetScript().size(); ++i)
		{
			if (CPlayerScript::PlayerObject->GetScript()[i]->GetScriptType() == (UINT)SCRIPT_TYPE::PLAYERSCRIPT)
			{
				PlayerScriptptr = (CPlayerScript*)(CPlayerScript::PlayerObject->GetScript()[i]);
			}
		}
	}

	if (Animator2D()->GetCurAnim()->GetCurFrm() == 16)
	{
		((CScript*)PlayerScriptptr)->SetSkillType((UINT)SKILL_TYPE::PAPULATUS_LASER_OVER);
		
		if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
		{
			CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
			CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();
			CPlayerScript::PlayerObject->AddChild(EffectPtr);
			((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PAPULATUS_LASER_OVER;

		}
	}
}


int CActiveExplosionScript::update()
{
	SkillPos();
	AnimSection();
	SkillChangeState();

	if (Activation_Explosion != PREV_Active_Explosion)
	{
		Skillstate();
	}
	PREV_Active_Explosion = Activation_Explosion;

	if (!Activation_Explosion)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}