#include "stdafx.h"
#include "ComboDeathfaultScript.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "Collider.h"
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
#include "CameraScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "EffectScript.h"

CComboDeathfaultScript::CComboDeathfaultScript()
	: DeathFaultDamage{ 0, 0, 0, 0, 0, 0, 0 }
	, Critical_Value(0)
{
	SetScriptType((UINT)SCRIPT_TYPE::COMBODEATHFAULTSCRIPT);
}

CComboDeathfaultScript::~CComboDeathfaultScript()
{
}


void CComboDeathfaultScript::OnCollisionEnter(CCollider * _pOther)
{
	if (DEATHFAULT_SkillState == DEATHFAULT_FSM::EFFECT)
	{
		if (_pOther->GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetIdx())
		{
			for (UINT i = 0; i < CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetObjectVec().size(); ++i)
			{
				if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetObjectVec()[i])
				{
					for (size_t i = 0; i < _pOther->GetGameObject()->GetScript().size(); ++i)
					{
						if (_pOther->GetGameObject()->GetScript()[i]->GetScriptType() != (UINT)SCRIPT_TYPE::MOBGRAVITYSCRIPT)
						{
							if (_pOther->GetGameObject()->GetScript()[i]->DeathFaultCollision == false)
							{
								_pOther->GetGameObject()->GetScript()[i]->DeathFaultCollision = true;
							}
						}
					}
				}
			}
		}
	}
}

void CComboDeathfaultScript::OnCollision(CCollider * _pOther)
{
	if (DEATHFAULT_SkillState == DEATHFAULT_FSM::SCREEN)
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
								if ((CScript*)(_pOther->GetGameObject()->GetScript()[i])->DeathFaultCollision)
								{
									if (10 < Animator2D()->GetCurAnim()->GetCurFrm())
									{

										_pOther->GetGameObject()->GetScript()[i]->DeathFaultCount += DT;
										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);

										if ((CScript*)(_pOther->GetGameObject()->GetScript()[i])->DeathFaultCollision)
										{
											((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::COMBO_DEATH_FAULT_HIT);
											((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;


											if (_pOther->GetGameObject()->GetScript()[i]->DeathFaultCount <= 0.1f)
											{
												return;
											}

											if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
											{
												if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
												{
													CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
													CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

													((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->DeathFaultEffectCount;

													_pOther->GetGameObject()->AddChild(EffectPtr);
													
													if (_pOther->GetGameObject()->GetScript()[i]->DeathFaultEffectCount < DEATHFAULT_OF_TIME - 1)
													{
														((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::COMBO_DEATH_FAULT_HIT;
													}
													else
													{
														switch (Well512Random::Instance().GetValue(0, 3))
														{
														case 0:
															((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_0;
															break;
														case 1:
															((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_1;
															break;
														case 2:
															((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_2;
															break;
														case 3:
															((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_3;
															break;
															/*	case 4:
																	((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::COMBO_DEATH_FAULT_HIT;
																	break;
															*/
														default:
															break;
														}
													}
													
												}
											}


											//									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(AlphaDamage[_pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount]);


											_pOther->GetGameObject()->GetScript()[i]->DeathFaultCount = 0.f;
											_pOther->GetGameObject()->GetScript()[i]->DeathFaultEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

											if (DEATHFAULT_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->DeathFaultEffectCount)
											{
												_pOther->GetGameObject()->GetScript()[i]->DeathFaultEffectCount = 0;
												_pOther->GetGameObject()->GetScript()[i]->DeathFaultCollision = false;
											}
										}


										//((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(ShoutDamage[0]);
										//((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(ShoutDamage[1]);
										//((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(ShoutDamage[2]);
										//((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(ShoutDamage[3]);
										//((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(ShoutDamage[4]);
										//((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(ShoutDamage[5]);
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

void CComboDeathfaultScript::OnCollisionExit(CCollider * _pOther)
{
	if (DEATHFAULT_SkillState == DEATHFAULT_FSM::NONE)
	{
		if (_pOther->GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetIdx())
		{
			for (UINT i = 0; i < CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetObjectVec().size(); ++i)
			{
				if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetObjectVec()[i])
				{
					for (size_t i = 0; i < _pOther->GetGameObject()->GetScript().size(); ++i)
					{
						if (_pOther->GetGameObject()->GetScript()[i]->GetScriptType() != (UINT)SCRIPT_TYPE::MOBGRAVITYSCRIPT)
						{
							_pOther->GetGameObject()->GetScript()[i]->DeathFaultCollision = true;
						}
					}
				}
			}
		}
	}
}


bool CComboDeathfaultScript::Critical_Calculate()
{
	Critical_Value = Well512Random::Instance().GetValue(0, 100);

	if ((CPlayerScript::CRITICAL + CPlayerScript::MAPLESOLDIER) >= Critical_Value)
	{
		return true;
	}
	else
		return false;
}


void CComboDeathfaultScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}


void CComboDeathfaultScript::Skillstate()
{
	switch (DEATHFAULT_SkillState)
	{
	case DEATHFAULT_FSM::NONE:

		Animator2D()->PlayAnim(L"blink");

		if (CPlayerScript::m_PlayerState != FSM::FLASHJUMP)
		{
			CPlayerScript::m_PlayerState = FSM::STAND;
		}
		break;

	case DEATHFAULT_FSM::EFFECT:

		CurDirSet();
		Animator2D()->PlayAnim(L"400011027.effect.frames");
		break;

	case DEATHFAULT_FSM::SCREEN:

		CurDirSet();
		Animator2D()->PlayAnim(L"400011027.screen.frames");
		break;

	case DEATHFAULT_FSM::END:
		break;

	default:
		break;
	}
}

void CComboDeathfaultScript::SkillPos()
{

	if (DEATHFAULT_SkillState == DEATHFAULT_FSM::NONE)
	{
		Transform()->SetLocalPos(DISABLE_POS);
		return;
	}

	if (DEATHFAULT_SkillState == DEATHFAULT_FSM::SCREEN)
	{
		findCS();

		Transform()->SetLocalPos(CS->GetCameraPos());
		return;

	}
	else
	{
		PlayerDir = CPlayerScript::GetPlayerDir();

		if (PlayerDir == DIR::RIGHT)
		{
			vPos = CPlayerScript::PlayerPos;
			vPos.x += 55.f;
			Transform()->SetLocalPos(vPos);
			return;
		}
		else if (PlayerDir == DIR::LEFT)
		{
			vPos = CPlayerScript::PlayerPos;
			vPos.x -= 55.f;
			Transform()->SetLocalPos(vPos);
			return;
		}
	}
}

void CComboDeathfaultScript::SkillChangeState()
{
	PlayerState = CPlayerScript::GetPlayerState();
	COMBO_COUNT = CPlayerScript::Player_COMBO_COUNT;


	if (KEYTAB(KEY_TYPE::KEY_D) && PlayerState != FSM::BUFF && PlayerState != FSM::SHOUT  && PlayerState != FSM::DEATHFAULT && PlayerState != FSM::SHADOWFLASH_BETA && PlayerState != FSM::SHADOWFLASH_ALPHA && CPlayerScript::isCanControl)
	{
		if (PlayerState == FSM::PRONE)
		{
			return;
		}

		if (PlayerState != FSM::ROPE && PlayerState != FSM::LADDER && PlayerState != FSM::FLASHJUMP)
		{
			if (CPlayerScript::GetComboCount() >= 5)
			{
				if (DEATHFAULT_SkillState == DEATHFAULT_FSM::NONE)
				{
					for (size_t i = 0; i < DEATHFAULT_OF_TIME; ++i)
					{
						if (Critical_Calculate())
						{
							DeathFaultDamage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 17.60f);
						}
						else
							DeathFaultDamage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 17.60f);
					}

					CPlayerScript::PlayerSkillPos = CPlayerScript::GetPlayerPos();

					CPlayerScript::SetComboCount(CPlayerScript::GetComboCount() - 5);

					DEATHFAULT_SkillState = DEATHFAULT_FSM::EFFECT;

					CPlayerScript::SetPlayerState(FSM::DEATHFAULT);
				}
			}
			return;
		}
	}

	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			if (DEATHFAULT_PrevSKILLState == DEATHFAULT_FSM::EFFECT)
			{
				DEATHFAULT_SkillState = DEATHFAULT_FSM::SCREEN;
			}
			else if (DEATHFAULT_SkillState == DEATHFAULT_FSM::SCREEN)
			{
				DEATHFAULT_SkillState = DEATHFAULT_FSM::NONE;
			}

		}
	}

	return;
}

void CComboDeathfaultScript::CurDirSet()
{
	PlayerDir = CPlayerScript::GetPlayerDir();

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

void CComboDeathfaultScript::findCS()
{
	if (CS == nullptr)
	{
		for (size_t i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Camera")->GetObjectVec().size(); ++i)
		{
			if ((SCRIPT_TYPE)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Camera")->GetObjectVec()[i]->GetScript()[0]->GetScriptType()) == SCRIPT_TYPE::CAMERASCRIPT)
			{
				CS = (CCameraScript*)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"Camera")->GetObjectVec()[i]->GetScript()[0]);
				return;
			}
		}
	}
}


int CComboDeathfaultScript::update()
{
	//	CCollisionMgr::GetInst()->CollisionCheck(L"Default", L"MONSTER");

	SkillPos();

	SkillChangeState();

	if (DEATHFAULT_PrevSKILLState != DEATHFAULT_SkillState)
	{
		Skillstate();
	}
	DEATHFAULT_PrevSKILLState = DEATHFAULT_SkillState;

	GetGameObject()->SetActive(true);
	return 0;
}