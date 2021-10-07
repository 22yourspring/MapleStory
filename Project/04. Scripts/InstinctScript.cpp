#include "stdafx.h"
#include "InstinctScript.h"


#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "AnrealEngine/Collider.h"
#include "ResMgr.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"

#include "AnrealEngine/CollisionMgr.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/define.h"

#include "AnrealEngine/Script.h"

#include "PlayerScript.h"
#include "RagingBlowScript.h"
#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "EffectScript.h"

bool CInstinctScript::PlayerAction = false;
bool CInstinctScript::m_bInstinctActive = false;
bool CInstinctScript::PrevConfirm = false;

CInstinctScript::CInstinctScript()
	: m_iInstinctNumber(0)
	, InstinctDamage{}
	, Critical_Value(0)
{
	SetScriptType((UINT)SCRIPT_TYPE::INSTINCTSCRIPT);
}

CInstinctScript::~CInstinctScript()
{
}

void CInstinctScript::OnCollisionEnter(CCollider * _pOther)
{
	if (INSTINCT_SkillState == INSTINCT_FSM::ATTACK)
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
								//	if (((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->GetSkillType() != ((UINT)SKILL_TYPE::RAGINGBLOW_PHASE1) || ((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->GetSkillType() != ((UINT)SKILL_TYPE::RAGINGBLOW_PHASE2))

								if (_pOther->GetGameObject()->GetScript()[i]->InstinctCollision == false)
								{
									_pOther->GetGameObject()->GetScript()[i]->InstinctCollision = true;
								}


								//	if (Animator2D()->GetCurAnim()->GetCurFrm() <= 5)
								{
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::COMBO_INSTINCT);
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;
								}
							}
						}
					}
				}
			}
		}
	}
}

void CInstinctScript::OnCollision(CCollider * _pOther)
{
	if (INSTINCT_SkillState == INSTINCT_FSM::ATTACK)
	{
		if (_pOther->GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetIdx())
		{
			for (UINT i = 0; i < CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetObjectVec().size(); ++i)
			{
				if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetObjectVec()[i])
				{
					if (_pOther->GetGameObject()->Confirm_Life())
					{
						for (UINT i = 0; i < _pOther->GetGameObject()->GetScript().size(); ++i)
						{
							if (_pOther->GetGameObject()->GetScript()[i]->GetScriptType() != (UINT)SCRIPT_TYPE::MOBGRAVITYSCRIPT)
							{

								_pOther->GetGameObject()->GetScript()[i]->InstinctCount += DT;

								if ((CScript*)(_pOther->GetGameObject()->GetScript()[i])->InstinctCollision)
								{
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::COMBO_INSTINCT);
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;


									if (_pOther->GetGameObject()->GetScript()[i]->InstinctCount <= 0.07f)
									{
										return;
									}

									if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
									{
										if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
										{
											CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
											CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

											((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->InstinctEffectCount;

											_pOther->GetGameObject()->AddChild(EffectPtr);
											((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::COMBO_INSTINCT;
										}
									}


									//									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(AlphaDamage[_pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount]);


									_pOther->GetGameObject()->GetScript()[i]->InstinctCount = 0.f;
									_pOther->GetGameObject()->GetScript()[i]->InstinctEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

									if (INSTINCT_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->InstinctEffectCount)
									{
										_pOther->GetGameObject()->GetScript()[i]->InstinctEffectCount = 0;
										_pOther->GetGameObject()->GetScript()[i]->InstinctCollision = false;
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

void CInstinctScript::OnCollisionExit(CCollider * _pOther)
{
}


bool CInstinctScript::Critical_Calculate()
{
	Critical_Value = Well512Random::Instance().GetValue(0, 100);
	if ((CPlayerScript::CRITICAL + CPlayerScript::MAPLESOLDIER) >= Critical_Value)
	{
		return true;
	}
	else
		return false;
}

void CInstinctScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}


void CInstinctScript::Skillstate()
{
	switch (INSTINCT_SkillState)
	{
	case INSTINCT_FSM::DISABLE:
		Animator2D()->PlayAnim(L"blink");
		break;

	case INSTINCT_FSM::ATTACK:
		CurDirSet();
		SkillPos();

		switch (m_iInstinctNumber % 3)
		{
		case 0:	Animator2D()->PlayAnim(L"400011074.effect.frames");
			++m_iInstinctNumber;
			if (m_iInstinctNumber >= 100)
			{
				m_iInstinctNumber = 0;

				m_bInstinctActive = false;
			}
			break;
		case 1:	Animator2D()->PlayAnim(L"400011075.effect.frames");
			++m_iInstinctNumber;
			break;

		case 2:	Animator2D()->PlayAnim(L"400011076.effect.frames");
			++m_iInstinctNumber;
			break;

		default:
			break;
		}

		break;

		//case INSTINCT_FSM::ENABLE:
		//	Animator2D()->PlayAnim(L"400011073.effect.frames");
		//	break;

	case INSTINCT_FSM::END:
		break;

	default:
		break;
	}
}

void CInstinctScript::SkillPos()
{
	PlayerDir = CRagingBlowScript::RagingDir;

	if (PlayerDir == DIR::RIGHT)
	{
		if (INSTINCT_SkillState == INSTINCT_FSM::DISABLE && PrevConfirm)
		{
			INSTINCT_SkillState = INSTINCT_FSM::ATTACK;

			vPos = CPlayerScript::GetPlayerPos();
			vPos.x += 200.f;
			vPos.z -= 300.f;
			Transform()->SetLocalPos(vPos);

			return;
		}
	}
	else if (PlayerDir == DIR::LEFT)
	{
		if (INSTINCT_SkillState == INSTINCT_FSM::DISABLE && PrevConfirm)
		{
			INSTINCT_SkillState = INSTINCT_FSM::ATTACK;

			vPos = CPlayerScript::GetPlayerPos();
			vPos.x -= 200.f;
			vPos.z -= 300.f;
			Transform()->SetLocalPos(vPos);

			return;
		}
	}

	if (INSTINCT_SkillState == INSTINCT_FSM::NONE)
	{
		Transform()->SetLocalPos(DISABLE_POS);
		GetGameObject()->SetActive(false);
	}
}

void CInstinctScript::SkillChangeState()
{
	PlayerState = CPlayerScript::GetPlayerState();


	if (PlayerAction)
	{
		if (PlayerState == FSM::PRONE || PlayerState == FSM::ROPE || PlayerState == FSM::LADDER || PlayerState == FSM::SHOUT || PlayerState == FSM::SHADOWFLASH_ALPHA || PlayerState == FSM::SHADOWFLASH_BETA || PlayerState == FSM::DEATHFAULT || PlayerState == FSM::BUFF)
		{
			return;
		}

		if (m_bInstinctActive)
		{
			//

			CPlayerScript::PlayerSkillPos = CPlayerScript::GetPlayerPos();

			for (size_t i = 0; i < INSTINCT_OF_TIME; ++i)
			{
				if (Critical_Calculate())
				{
					InstinctDamage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 4.4f);
				}
				else
					InstinctDamage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 4.4f);
			}

			//
			INSTINCT_SkillState = INSTINCT_FSM::DISABLE;
			PlayerAction = false;

		}
		return;
	}

	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish()) //현재 스킬 오브젝트의 현재 프레임이 끝이거나 0 일 경우 NONE으로
		{
			INSTINCT_SkillState = INSTINCT_FSM::NONE;
		}
	}
	return;
}

void CInstinctScript::CurDirSet()
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
//
//void CInstinctScript::InstinctActivePos()
//{	
////	InstinctActive = true;
//
//	vPos = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->GetLocalPos();
//	vPos.y += 50.f;
//	vPos.z -= 300.f;
//	Transform()->SetLocalPos(vPos);
//
//}


int CInstinctScript::update()
{
	//	CCollisionMgr::GetInst()->CollisionCheck(L"Default", L"MONSTER");

	SkillPos();

	SkillChangeState();

	if (INSTINCT_PrevSKILLState != INSTINCT_SkillState)
	{
		if (INSTINCT_SkillState != INSTINCT_FSM::NONE)
		{
			GetGameObject()->SetActive(true);
		}
		Skillstate();
	}
	INSTINCT_PrevSKILLState = INSTINCT_SkillState;

	return 0;
}
