#include "stdafx.h"
#include "BeShotEffectScript.h"

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

#include "PlayerScript.h"
#include "PapulatusScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"


CBeShotEffectScript::CBeShotEffectScript()
	: EFFECT_SkillState(SKILL_TYPE::NONE)
	, m_iScriptCount(0)
	, m_iEffectCount(-1)
	, blinkcheck(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::BESHOTEFFECTSCRIPT);
}


CBeShotEffectScript::~CBeShotEffectScript()
{
}



void CBeShotEffectScript::awake()
{
}

void CBeShotEffectScript::Skillstate()
{
	switch (EFFECT_SkillState)
	{
	case SKILL_TYPE::NONE:
		break;

	case SKILL_TYPE::GATEKEEPER_ATTACK:
		Animator2D()->PlayAnim(L"8160000.img.attack1.info.hit");
		break;

	case SKILL_TYPE::PAPULATUS_BLINK:
		Animator2D()->PlayAnim(L"BlinkHere");
		break;

	case SKILL_TYPE::END:
		break;

	default:
		break;
	}
}

void CBeShotEffectScript::SkillPos()
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		if (EFFECT_SkillState == SKILL_TYPE::NONE)
		{
			Transform()->SetLocalPos(DISABLE_POS);
			return;
		}
		else
		{
			vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();
			
			if (EFFECT_SkillState == SKILL_TYPE::GATEKEEPER_ATTACK)
			{
				if (CPlayerScript::GetPlayerDir() == DIR::LEFT)
				{
					vPos.x -= 10.f;
				}
				else
				{
					vPos.x += 15.f;
				}

				vPos.y += 40.f;
				vPos.z = 400.f; 
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_BLINK)
			{
				vPos.x -= 0.f;
				vPos.y += 0.f;
				vPos.z -= 200.f;
			}
			else
			{
				vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();
			}
		}
		Transform()->SetLocalPos(vPos);
	}
}

void CBeShotEffectScript::SkillChangeState()
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		for (size_t i = 0; i < GetGameObject()->GetParent()->GetScript().size(); ++i)
		{
			if (GetGameObject()->GetParent()->GetScript()[i]->GetScriptType() != (UINT)SCRIPT_TYPE::MOBGRAVITYSCRIPT)
			{
				if (EFFECTPREV_SkillState != EFFECT_SkillState || EFFECT_SkillState == SKILL_TYPE::NONE || EFFECT_SkillState == SKILL_TYPE::COMBO_INSTINCT)
				{
					EFFECT_SkillState = (SKILL_TYPE)(GetGameObject()->GetParent()->GetScript()[i]->GetSkillType());
					m_iScriptCount = (int)i;

					return;
				}
			}
		}

		if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_BLINK && blinkcheck)
		{
			if (Animator2D()->GetCurAnim()->GetCurFrm() == 15)
			{
				CPapulatusScript::BlinkPlayer();
				blinkcheck = false;
			}
		}

		if (Animator2D()->GetCurAnim() != nullptr)
		{
			if (Animator2D()->GetCurAnim()->IsFinish() && EFFECT_SkillState != SKILL_TYPE::NONE)
			{
				EFFECT_SkillState = SKILL_TYPE::NONE;

				GetGameObject()->GetParent()->GetScript()[m_iScriptCount]->SetSkillType((UINT)SKILL_TYPE::NONE);

				blinkcheck = true;
			}
		}
		return;

	}
}


bool CBeShotEffectScript::LeftFromPlayer()
{
	if (vPos.x > CPlayerScript::GetPlayerPos().x)
	{
		return false;
	}
	else
	{
		return true;
	}
}

int CBeShotEffectScript::update()
{
	SkillPos();
	SkillChangeState();

	if (EFFECTPREV_SkillState != EFFECT_SkillState)
	{
		Skillstate();
	}
	EFFECTPREV_SkillState = EFFECT_SkillState;

	if (EFFECT_SkillState == SKILL_TYPE::NONE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}