#include "stdafx.h"
#include "RagingBlowScript.h"


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

#include "InstinctScript.h"

#include "AnrealEngine/Well512Random.h"
#include "EffectScript.h"

bool CRagingBlowScript::RagingBlowPhase2Mob = false;
RAGINGBLOW_FSM CRagingBlowScript::RAGINGBLOW_SkillState = RAGINGBLOW_FSM::NONE;
RAGINGBLOW_FSM CRagingBlowScript::RAGINGBLOW_PrevSKILLState = RAGINGBLOW_FSM::NONE;

DIR	CRagingBlowScript::RagingDir = DIR::LEFT;

CRagingBlowScript::CRagingBlowScript()
	: EnrageActive (false)
	, RagingDamage{0,0,0,0,0}
	, Critical_Value(0)
{
	SetScriptType((UINT)SCRIPT_TYPE::RAGINGBLOWSCRIPT);
}

CRagingBlowScript::~CRagingBlowScript()
{
}

bool CRagingBlowScript::Critical_Calculate()
{
	Critical_Value = Well512Random::Instance().GetValue(0, 100);
	if ((CPlayerScript::CRITICAL + CPlayerScript::MAPLESOLDIER) >= Critical_Value)
	{
		return true;
	}
	else 
		return false;
}

void CRagingBlowScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CRagingBlowScript::update()
{
	SkillPos();

	SkillChangeState();
	
	if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::NONE)
	{
		CurDirSet();
	}

	if (RAGINGBLOW_PrevSKILLState != RAGINGBLOW_SkillState)
	{
		Skillstate();
	}

	RAGINGBLOW_PrevSKILLState = RAGINGBLOW_SkillState;


	GetGameObject()->SetActive(true);
	return 0;
}

void CRagingBlowScript::OnCollisionEnter(CCollider * _pOther)
{
	if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::PHASE1)
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
								if (Animator2D()->GetCurAnim()->GetCurFrm() <= 3)	// 떄리는 중간에 들어오는 몬스터 피격 제외
								{
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);

									if (CPlayerScript::COMBO_ON)
									{
										if (CPlayerScript::GetComboCount() < CPlayerScript::COMBO_MAXCOUNT)
										{
											CPlayerScript::SetComboCount(CPlayerScript::GetComboCount() + 1);
										}
									}

									if (_pOther->GetGameObject()->GetScript()[i]->RagingCollision == false)
									{
										_pOther->GetGameObject()->GetScript()[i]->RagingCollision = true;
									}
								}
							}
						}
					}
				}
			}
		}
	}
	else if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::PHASE2 && CRagingBlowScript::RagingBlowPhase2Mob)
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
								if (Animator2D()->GetCurAnim()->GetCurFrm() <= 3)
								{
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);

									if (CPlayerScript::COMBO_ON)
									{
										if (CPlayerScript::GetComboCount() < CPlayerScript::COMBO_MAXCOUNT)
										{
											CPlayerScript::SetComboCount(CPlayerScript::GetComboCount() + 1);
										}
									}

									if (_pOther->GetGameObject()->GetScript()[i]->RagingCollision == false)
									{
										_pOther->GetGameObject()->GetScript()[i]->RagingCollision = true;
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

void CRagingBlowScript::OnCollision(CCollider * _pOther)
{
	if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::PHASE1)
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
								{
									_pOther->GetGameObject()->GetScript()[i]->RagingCount += DT;

									if ((CScript*)(_pOther->GetGameObject()->GetScript()[i])->RagingCollision)
									{
										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::RAGINGBLOW_PHASE1);
										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;


										if (_pOther->GetGameObject()->GetScript()[i]->RagingCount <= 0.08f)
										{
											return;
										}

										if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
										{
											if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
											{
												CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
												CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

												((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->RagingEffectCount;

												_pOther->GetGameObject()->AddChild(EffectPtr);
												((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::RAGINGBLOW_PHASE1;
											}
										}


										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(RagingDamage[_pOther->GetGameObject()->GetScript()[i]->RagingEffectCount]);

										_pOther->GetGameObject()->GetScript()[i]->RagingCount = 0.f;
										_pOther->GetGameObject()->GetScript()[i]->RagingEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

										if (RAGINGBLOW_PHASE_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->RagingEffectCount)
										{
											_pOther->GetGameObject()->GetScript()[i]->RagingEffectCount = 0;
											_pOther->GetGameObject()->GetScript()[i]->RagingCollision = false;
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
	else if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::PHASE2 && CRagingBlowScript::RagingBlowPhase2Mob)
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
								{
									_pOther->GetGameObject()->GetScript()[i]->RagingCount += DT;

									if ((CScript*)(_pOther->GetGameObject()->GetScript()[i])->RagingCollision)
									{
										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::RAGINGBLOW_PHASE2);
										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;


										if (_pOther->GetGameObject()->GetScript()[i]->RagingCount <= 0.08f)
										{
											return;
										}

										if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
										{
											if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
											{
												CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
												CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

												((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->RagingEffectCount;

												_pOther->GetGameObject()->AddChild(EffectPtr);
												((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::RAGINGBLOW_PHASE2;
											}
										}


										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(RagingDamage[_pOther->GetGameObject()->GetScript()[i]->RagingEffectCount]);


										_pOther->GetGameObject()->GetScript()[i]->RagingCount = 0.f;
										_pOther->GetGameObject()->GetScript()[i]->RagingEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

										if (ALPHAPHASE2_HIT_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->RagingEffectCount)
										{
											_pOther->GetGameObject()->GetScript()[i]->RagingEffectCount = 0;
											_pOther->GetGameObject()->GetScript()[i]->RagingCollision = false;
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

void CRagingBlowScript::OnCollisionExit(CCollider * _pOther)
{

}

void CRagingBlowScript::Skillstate()
{
	switch (RAGINGBLOW_SkillState)
	{
	case RAGINGBLOW_FSM::NONE:
		Animator2D()->PlayAnim(L"blink");
		break;
		
	case RAGINGBLOW_FSM::PHASE1:
		CurDirSet();
		Animator2D()->PlayAnim(L"1121008.effect.0.frames");
		break;

	case RAGINGBLOW_FSM::PHASE2:
		CurDirSet();
		Animator2D()->PlayAnim(L"1120017.effect.0.framesTEMP");
		break;

	case RAGINGBLOW_FSM::ENRAGE_ACTIVE:
		CurDirSet();
		Animator2D()->PlayAnim(L"Enrage");
		EnrageActive = true;
		break;


	case RAGINGBLOW_FSM::END:
		break;

	default:
		break;
	}
}

void CRagingBlowScript::SkillPos()
{
	PlayerState = CPlayerScript::GetPlayerState();


	PlayerDir = CPlayerScript::GetPlayerDir();
	
	if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::NONE)
	{
		Transform()->SetLocalPos(DISABLE_POS);
	}
	
	if (PlayerDir == DIR::RIGHT)
	{
		if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::PHASE1)
		{
			vPos = CPlayerScript::GetPlayerPos();
			vPos.x += 10.f;
			vPos.y += 40.f;
			vPos.z -= 10.f;
			Transform()->SetLocalPos(vPos);

			return;
		}
		else if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::PHASE2)
		{
			vPos = CPlayerScript::GetPlayerPos();
			vPos.x += 20.f;
			vPos.y += 5.f;
			vPos.z -= 10.f;

			Transform()->SetLocalPos(vPos);

			return;
		}
	}
	else if (PlayerDir == DIR::LEFT)
	{
		if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::PHASE1)
		{
			vPos = CPlayerScript::GetPlayerPos();
			vPos.x -= 10.f;
			vPos.y += 40.f;
			vPos.z -= 10.f;

			Transform()->SetLocalPos(vPos);

			return;
		}
		else if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::PHASE2)
		{
			vPos = CPlayerScript::GetPlayerPos();
			vPos.x -= 20.f;
			vPos.y += 5.f;
			vPos.z -= 10.f;

			Transform()->SetLocalPos(vPos);

			return;
		}
	}

	if (RAGINGBLOW_SkillState == RAGINGBLOW_FSM::ENRAGE_ACTIVE)
	{
		vPos = CPlayerScript::GetPlayerPos();
		vPos.y += 50.f;
		vPos.z -= 10.f;

		Transform()->SetLocalPos(vPos);

		return;
	}
	/*CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"EFFECT")->GetObjectVec()[0]
	CSceneMgr::GetInst()->GetLayer(L"EFFECT")->GetObjectVec()[0]->AddChild()*/
}

void CRagingBlowScript::SkillChangeState()
{
	PlayerState = CPlayerScript::GetPlayerState();
	COMBO_COUNT = CPlayerScript::GetComboCount();


	if (KEYTAB(KEY_TYPE::KEY_LSHIFT))
	{
		if (!CPlayerScript::isCanControl)
		{
			return;
		}

		if (PlayerState == FSM::PRONE || PlayerState == FSM::ROPE || PlayerState == FSM::LADDER || PlayerState == FSM::SHOUT || PlayerState == FSM::SHADOWFLASH_ALPHA || PlayerState == FSM::SHADOWFLASH_BETA || PlayerState == FSM::DEATHFAULT || PlayerState == FSM::BUFF)
		{
			return;
		}

		if (!EnrageActive)
		{
			if ((CPlayerScript::m_PlayerState == FSM::STAND) || (CPlayerScript::m_PlayerState == FSM::WALK) || (CPlayerScript::m_PlayerState == FSM::JUMP) || (CPlayerScript::m_PlayerState == FSM::STAND_FLASH) || (CPlayerScript::m_PlayerState == FSM::FLASHJUMP))
			{
				RAGINGBLOW_SkillState = RAGINGBLOW_FSM::PHASE1;

				CPlayerScript::PlayerSkillPos = CPlayerScript::GetPlayerPos();

				for (size_t i = 0; i < RAGINGBLOW_PHASE_OF_TIME; ++i)
				{
					if (Critical_Calculate())
					{
						RagingDamage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 2.85f);
					}
					else
						RagingDamage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 2.85f);
				}
				RagingDamage[4] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 2.85f);

				RagingDir = PlayerDir;

				CInstinctScript::PrevConfirm = true;
			}
		}
		else if(EnrageActive && RAGINGBLOW_SkillState == RAGINGBLOW_FSM::NONE)
		{
			if ((CPlayerScript::m_PlayerState == FSM::STAND) || (CPlayerScript::m_PlayerState == FSM::WALK) || (CPlayerScript::m_PlayerState == FSM::JUMP) || (CPlayerScript::m_PlayerState == FSM::STAND_FLASH) || (CPlayerScript::m_PlayerState == FSM::FLASHJUMP))
			{
				RAGINGBLOW_SkillState = RAGINGBLOW_FSM::PHASE2;

				CPlayerScript::PlayerSkillPos = CPlayerScript::GetPlayerPos();

				CRagingBlowScript::RagingBlowPhase2Mob = true;

				for (size_t i = 0; i < RAGINGBLOW_PHASE_OF_TIME; ++i)
				{
					if (Critical_Calculate())
					{
						RagingDamage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 2.85f);
					}
					else
						RagingDamage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 2.85f);
				}
				RagingDamage[3] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 2.85f);
				RagingDamage[4] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 2.85f);

				RagingDir = PlayerDir;

				CInstinctScript::PrevConfirm = true;
			}
		}
		
	
		return;
	}

	if (KEYTAB(KEY_TYPE::KEY_2) && PlayerState != FSM::BUFF && PlayerState != FSM::SHOUT  && PlayerState != FSM::DEATHFAULT && PlayerState != FSM::SHADOWFLASH_BETA && PlayerState != FSM::SHADOWFLASH_ALPHA)
	{
		if (PlayerState == FSM::PRONE)
		{
			return;
		}

		if (CPlayerScript::GetComboCount() > 0)
		{
			if (Animator2D()->GetCurAnim()->GetName() == L"blink")
			{
				CPlayerScript::SetComboCount(CPlayerScript::GetComboCount() - 1);
				RAGINGBLOW_SkillState = RAGINGBLOW_FSM::ENRAGE_ACTIVE;
			}
		}
		return;
	}


	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			if (RAGINGBLOW_SkillState != RAGINGBLOW_FSM::NONE)
			{
				if (CInstinctScript::m_bInstinctActive)
				{
					CInstinctScript::PlayerAction = true;
				}
			}
			RAGINGBLOW_SkillState = RAGINGBLOW_FSM::NONE;
		}
	}
	return;
}

void CRagingBlowScript::CurDirSet()
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
