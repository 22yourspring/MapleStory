#include "stdafx.h"
#include "ShoutScript.h"


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

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "EffectScript.h"

SHOUT CShoutScript::SHOUT_SkillState = SHOUT::DISABLE;
SHOUT CShoutScript::SHOUT_PrevSKILLState = SHOUT::DISABLE;

CShoutScript::CShoutScript()
	: ShoutDamage{ 0,0,0,0,0,0 }
	, Critical_Value(0)
{
	SetScriptType((UINT)SCRIPT_TYPE::SHOUTSCRIPT);
}

CShoutScript::~CShoutScript()
{
}

void CShoutScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}



void CShoutScript::OnCollisionEnter(CCollider * _pOther)
{
	if ((SHOUT_SkillState == SHOUT::ENABLE))
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
							if (_pOther->GetGameObject()->GetScript()[i]->ShoutCollision == false)
							{
								_pOther->GetGameObject()->GetScript()[i]->ShoutCollision = true;
							}
						}
					}
				}
			}
		}
	}
}

void CShoutScript::OnCollision(CCollider * _pOther)
{
	if (SHOUT_SkillState == SHOUT::ENABLE)
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
								if ((CScript*)(_pOther->GetGameObject()->GetScript()[i])->ShoutCollision)
								{
									if (3 < Animator2D()->GetCurAnim()->GetCurFrm())
									{

										_pOther->GetGameObject()->GetScript()[i]->ShoutCount += DT;
										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);

										if ((CScript*)(_pOther->GetGameObject()->GetScript()[i])->ShoutCollision)
										{
											((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::SHOUT_HIT);
											((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;


											if (_pOther->GetGameObject()->GetScript()[i]->ShoutCount <= 0.1f)
											{
												return;
											}

											if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
											{
												if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
												{
													CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
													CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

													((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->ShoutEffectCount;

													_pOther->GetGameObject()->AddChild(EffectPtr);
													((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::SHOUT_HIT;
												}
											}


											//									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(AlphaDamage[_pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount]);


											_pOther->GetGameObject()->GetScript()[i]->ShoutCount = 0.f;
											_pOther->GetGameObject()->GetScript()[i]->ShoutEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

											if (SHOUT_HIT_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->ShoutEffectCount)
											{
												_pOther->GetGameObject()->GetScript()[i]->ShoutEffectCount = 0;
												_pOther->GetGameObject()->GetScript()[i]->ShoutCollision = false;
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

void CShoutScript::OnCollisionExit(CCollider * _pOther)
{
	if ((SHOUT_SkillState == SHOUT::DISABLE))
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
							_pOther->GetGameObject()->GetScript()[i]->ShoutCollision = true;
						}
					}

				}
			}
		}
	}
}

void CShoutScript::Skillstate()
{
	switch (SHOUT_SkillState)
	{
	case SHOUT::DISABLE:

		if (CPlayerScript::m_PlayerState != FSM::FLASHJUMP)
		{
			CPlayerScript::m_PlayerState = FSM::STAND;
		}

		break;

	case SHOUT::ENABLE:

		CurDirSet();
		Animator2D()->PlayAnim(L"Shout_Effect");
		break;


	case SHOUT::END:
		break;

	default:
		break;
	}
}

void CShoutScript::SkillPos()
{
	PlayerState = CPlayerScript::GetPlayerState();
	PlayerDir = CPlayerScript::GetPlayerDir();

	if (SHOUT_SkillState == SHOUT::DISABLE)
	{
		Transform()->SetLocalPos(DISABLE_POS);
		return;
	}

	if (PlayerDir == DIR::RIGHT && SHOUT_SkillState != SHOUT::DISABLE)
	{
		vPos = CPlayerScript::PlayerPos;
		vPos.x += 20.f;
		vPos.y += 50.f;
		vPos.z += 100.f;
		Transform()->SetLocalPos(vPos);
		return;
	}
	else if (PlayerDir == DIR::LEFT && SHOUT_SkillState != SHOUT::DISABLE)
	{
		vPos = CPlayerScript::PlayerPos;
		vPos.x -= 20.f;
		vPos.y += 50.f;
		vPos.z += 100.f;
		Transform()->SetLocalPos(vPos);
		return;
	}

}

void CShoutScript::SkillChangeState()
{
	PlayerState = CPlayerScript::GetPlayerState();
	COMBO_COUNT = CPlayerScript::Player_COMBO_COUNT;

	if (KEYTAB(KEY_TYPE::KEY_S) && PlayerState != FSM::BUFF && PlayerState != FSM::SHOUT  && PlayerState != FSM::DEATHFAULT && PlayerState != FSM::SHADOWFLASH_BETA && PlayerState != FSM::SHADOWFLASH_ALPHA && CPlayerScript::isCanControl)
	{
		if (PlayerState == FSM::PRONE)
		{
			return;
		}

		if (PlayerState != FSM::ROPE && PlayerState != FSM::LADDER && PlayerState != FSM::FLASHJUMP)
		{
			if (CPlayerScript::GetComboCount() > 0)
			{
				if (SHOUT_SkillState == SHOUT::DISABLE)
				{
					CPlayerScript::PlayerSkillPos = CPlayerScript::GetPlayerPos();

					for (size_t i = 0; i < SHOUT_HIT_OF_TIME; ++i)
					{
						if (Critical_Calculate())
						{
							ShoutDamage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 4.4f);
						}
						else
							ShoutDamage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 4.4f);
					}

					CPlayerScript::SetComboCount(CPlayerScript::GetComboCount() - 1);

					SHOUT_SkillState = SHOUT::ENABLE;
				}
			}
			return;
		}
	}


	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			SHOUT_SkillState = SHOUT::DISABLE;
		}
	}
	return;
}

void CShoutScript::CurDirSet()
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

bool CShoutScript::Critical_Calculate()
{
	Critical_Value = Well512Random::Instance().GetValue(0, 100);

	if ((CPlayerScript::CRITICAL + CPlayerScript::MAPLESOLDIER) >= Critical_Value)
	{
		return true;
	}
	else
		return false;
}


int CShoutScript::update()
{
	//	CCollisionMgr::GetInst()->CollisionCheck(L"Default", L"MONSTER");

	SkillPos();

	SkillChangeState();

	if (SHOUT_PrevSKILLState != SHOUT_SkillState)
	{
		Skillstate();
	}
	SHOUT_PrevSKILLState = SHOUT_SkillState;

	if (SHOUT_SkillState == SHOUT::ENABLE)
	{
		GetGameObject()->SetActive(true);
	}
	else
	{
		GetGameObject()->SetActive(false);
	}

	return 0;
}