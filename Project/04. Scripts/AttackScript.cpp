#include "stdafx.h"
#include "AttackScript.h"


#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "ResMgr.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"

#include "AnrealEngine/CollisionMgr.h"
#include "AnrealEngine/Collider.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/define.h"

#include "AnrealEngine/Script.h"

#include "PlayerScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "AnrealEngine/Well512Random.h"


CAttackScript::CAttackScript()
	: EnrageActive(false)
	, AttackDamage(0)
	, Critical_Value(0)
	, ATTACK_SkillState (ATTACK_FSM::NONE)
	, ATTACK_PrevSKILLState (ATTACK_FSM::NONE)

{
	SetScriptType((UINT)SCRIPT_TYPE::ATTACKSCRIPT);
}

CAttackScript::~CAttackScript()
{
}

bool CAttackScript::Critical_Calculate()
{
	Critical_Value = Well512Random::Instance().GetValue(0, 100);
	if ((CPlayerScript::CRITICAL + CPlayerScript::MAPLESOLDIER) >= Critical_Value)
	{
		return true;
	}
	else
		return false;
}

void CAttackScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CAttackScript::update()
{
	SkillPos();

	SkillChangeState();

	if (ATTACK_SkillState == ATTACK_FSM::NONE)
	{
		CurDirSet();
	}

	if (ATTACK_PrevSKILLState != ATTACK_SkillState)
	{
		Skillstate();
	}

	ATTACK_PrevSKILLState = ATTACK_SkillState;

	return 0;
}

void CAttackScript::OnCollisionEnter(CCollider * _pOther)
{
	if (ATTACK_SkillState == ATTACK_FSM::ENABLE)
	{
		if (_pOther->GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetIdx())
		{
			for (UINT i = 0; i < CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetObjectVec().size(); ++i)
			{
				if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetObjectVec()[i])
				{
					if (_pOther->GetGameObject()->Confirm_Life())
					{
						for (size_t i = 0; i < _pOther->GetGameObject()->GetScript().size(); ++i)
						{
							if (_pOther->GetGameObject()->GetScript()[i]->GetScriptType() != (UINT)SCRIPT_TYPE::MOBGRAVITYSCRIPT)
							{
								if (CPlayerScript::GetPlayerState() == FSM::ATTACK)	// 떄리는 중간에 들어오는 몬스터 피격 제외
								{
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;
								//	((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(AttackDamage);

									if (CPlayerScript::COMBO_ON)
									{
										if (CPlayerScript::GetComboCount() < CPlayerScript::COMBO_MAXCOUNT)
										{
											CPlayerScript::SetComboCount(CPlayerScript::GetComboCount() + 1);
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

void CAttackScript::OnCollision(CCollider * _pOther)
{
}

void CAttackScript::OnCollisionExit(CCollider * _pOther)
{

}

void CAttackScript::Skillstate()
{
	switch (ATTACK_SkillState)
	{
	case ATTACK_FSM::NONE:
		break;
	case ATTACK_FSM::ENABLE:
		break;
	case ATTACK_FSM::END:
		break;
	default:
		break;
	}
}

void CAttackScript::SkillPos()
{
	PlayerState = CPlayerScript::GetPlayerState();


	PlayerDir = CPlayerScript::GetPlayerDir();

	if (ATTACK_SkillState == ATTACK_FSM::NONE)
	{
		Transform()->SetLocalPos(DISABLE_POS);
	}

	if (PlayerDir == DIR::RIGHT)
	{
		if (ATTACK_SkillState == ATTACK_FSM::ENABLE)
		{
			vPos = CPlayerScript::GetPlayerPos();
			vPos.x += 50.f;
			vPos.z -= 10.f;
			Transform()->SetLocalPos(vPos);

			return;
		}
	}
	else if (PlayerDir == DIR::LEFT)
	{
		if (ATTACK_SkillState == ATTACK_FSM::ENABLE)
		{
			vPos = CPlayerScript::GetPlayerPos();
			vPos.x -= 50.f;
			vPos.z -= 10.f;

			Transform()->SetLocalPos(vPos);

			return;
		}
	}
	/*CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"EFFECT")->GetObjectVec()[0]
	CSceneMgr::GetInst()->GetLayer(L"EFFECT")->GetObjectVec()[0]->AddChild()*/
}

void CAttackScript::SkillChangeState()
{
	PlayerState = CPlayerScript::GetPlayerState();
	COMBO_COUNT = CPlayerScript::GetComboCount();


	if (KEYTAB(KEY_TYPE::KEY_CTRL))
	{
		if (PlayerState == FSM::PRONE || PlayerState == FSM::ROPE || PlayerState == FSM::LADDER || PlayerState == FSM::SHOUT || PlayerState == FSM::SHADOWFLASH_ALPHA || PlayerState == FSM::SHADOWFLASH_BETA || PlayerState == FSM::DEATHFAULT || PlayerState == FSM::BUFF)
		{
			return;
		}

		
		if ((CPlayerScript::m_PlayerState == FSM::STAND) || (CPlayerScript::m_PlayerState == FSM::WALK) || (CPlayerScript::m_PlayerState == FSM::JUMP) || (CPlayerScript::m_PlayerState == FSM::STAND_FLASH) || (CPlayerScript::m_PlayerState == FSM::FLASHJUMP))
		{
			ATTACK_SkillState = ATTACK_FSM::ENABLE;

			CPlayerScript::PlayerSkillPos = CPlayerScript::GetPlayerPos();

			if (Critical_Calculate())
			{
				AttackDamage = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f));
			}
			else
				AttackDamage = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT));
		}
		return;
	}

	if (CPlayerScript::GetPlayerState() != FSM::ATTACK)
	{
		ATTACK_SkillState = ATTACK_FSM::NONE;
	}
	return;
}

void CAttackScript::CurDirSet()
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
