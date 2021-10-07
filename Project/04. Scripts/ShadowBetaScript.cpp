#include "stdafx.h"
#include "ShadowBetaScript.h"


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
#include "RagingBlowScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "EffectScript.h"
#include "CraneScript.h"

CShadowBetaScript::CShadowBetaScript()
	: BetaOn (false)
	, m_bAfterEffect(false)
	, OtherCount(0)
{
	SetScriptType((UINT)SCRIPT_TYPE::SHADOWBETASCRIPT);
}

CShadowBetaScript::~CShadowBetaScript()
{
}

bool CShadowBetaScript::Critical_Calculate()
{
	Critical_Value = Well512Random::Instance().GetValue(0, 100);

	if ((CPlayerScript::CRITICAL + CPlayerScript::MAPLESOLDIER) >= Critical_Value)
	{
		return true;
	}
	else
		return false;
}

void CShadowBetaScript::AFTEREFFECT(CCollider * _pOther)
{
	if (SHADOWBETA_SkillState == SHADOWFLASH_FSM::NONE && SHADOWBETA_SkillState != SHADOWFLASH_FSM::ENABLE && SHADOWBETA_SkillState != SHADOWFLASH_FSM::PHASE1 && SHADOWBETA_SkillState != SHADOWFLASH_FSM::PHASE2)
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
								_pOther->GetGameObject()->GetScript()[i]->BetaCount += DT;

								((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);
								((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::BETAPHASE_HIT);
								((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;


								if (_pOther->GetGameObject()->GetScript()[i]->BetaCount <= 0.1f)
								{
									return;
								}

								if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
								{
									if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
									{
										CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
										CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

										((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->BetaEffectCount;

										_pOther->GetGameObject()->AddChild(EffectPtr);
										((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::BETAPHASE_HIT;

									}
								}


								//((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(BetaDamage[_pOther->GetGameObject()->GetScript()[i]->BetaEffectCount]);


								_pOther->GetGameObject()->GetScript()[i]->BetaCount = 0.f;
								_pOther->GetGameObject()->GetScript()[i]->BetaEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

								if (BETAPHASE2_HIT_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->BetaEffectCount)
								{
									_pOther->GetGameObject()->GetScript()[i]->BetaEffectCount = 0;
									m_bAfterEffect = false;

									return;
								}
							}
						}
					}
				}
			}
		}
	}
}

void CShadowBetaScript::OnCollisionEnter(CCollider * _pOther)
{
	if (SHADOWBETA_SkillState == SHADOWFLASH_FSM::PHASE1 || SHADOWBETA_SkillState == SHADOWFLASH_FSM::PHASE1_FINISH || SHADOWBETA_SkillState == SHADOWFLASH_FSM::PHASE2 || SHADOWBETA_SkillState == SHADOWFLASH_FSM::ENABLE)
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
							if (_pOther->GetGameObject()->GetScript()[i]->BetaCollision == false)
							{
								_pOther->GetGameObject()->GetScript()[i]->BetaCollision = true;
							}
						}
					}
				}
			}
		}
	}
}

void CShadowBetaScript::OnCollision(CCollider * _pOther)
{
	if (SHADOWBETA_SkillState != SHADOWFLASH_FSM::END && SHADOWBETA_SkillState != SHADOWFLASH_FSM::NONE && SHADOWBETA_SkillState != SHADOWFLASH_FSM::PHASE2)
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
								_pOther->GetGameObject()->GetScript()[i]->BetaCount += DT;


								if ((CScript*)(_pOther->GetGameObject()->GetScript()[i])->BetaCollision)
								{
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::BETAPHASE_HIT);
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;


									if (_pOther->GetGameObject()->GetScript()[i]->BetaCount <= 0.1f)
									{
										return;
									}

									if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
									{
										if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
										{
											CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
											CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

											((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->BetaEffectCount;

											_pOther->GetGameObject()->AddChild(EffectPtr);
											((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::BETAPHASE_HIT;

										}
									}


									//									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(BetaDamage[_pOther->GetGameObject()->GetScript()[i]->BetaEffectCount]);


									_pOther->GetGameObject()->GetScript()[i]->BetaCount = 0.f;
									_pOther->GetGameObject()->GetScript()[i]->BetaEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

									if (BETAPHASE1_HIT_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->BetaEffectCount)
									{
										_pOther->GetGameObject()->GetScript()[i]->BetaEffectCount = 0;
										_pOther->GetGameObject()->GetScript()[i]->BetaCollision = false;
									}
								}

								////Acc DT
								_pOther->GetGameObject()->GetScript()[i]->BetaTime += DT;
								if (_pOther->GetGameObject()->GetScript()[i]->BetaTime < 4.3f)
								{
									return;
								}
								_pOther->GetGameObject()->GetScript()[i]->BetaCollision = true;
								_pOther->GetGameObject()->GetScript()[i]->BetaTime = 0;
							}
						}
					}
				}
			}
		}
	}
}

void CShadowBetaScript::OnCollisionExit(CCollider * _pOther)
{
	if (SHADOWBETA_SkillState == SHADOWFLASH_FSM::NONE)
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
							_pOther->GetGameObject()->GetScript()[i]->BetaCollision = true;
							_pOther->GetGameObject()->GetScript()[i]->BetaCount = 0.f;

							if (OtherPtr[OtherCount] != nullptr)
							{
								while (OtherPtr[OtherCount] == nullptr)
								{
									if (300 <= OtherCount)
									{
										OtherCount = 0;
									}
									OtherCount += 1;

									if (OtherPtr[OtherCount] == nullptr)
									{
										OtherPtr[OtherCount] = _pOther;
										if (OtherPtr[OtherCount + 1] == nullptr)
										{
											OtherCount += 1;
										}
										break;
									}
								}
							}
							else
							{
								OtherPtr[OtherCount] = _pOther;
								if (OtherPtr[OtherCount + 1] == nullptr)
								{
									OtherCount += 1;
								}
							}
							m_bAfterEffect = true;
						}
					}
				}
			}
		}
	}
}

void CShadowBetaScript::EFFECT_PROCESSING()
{
	if (m_bAfterEffect)
	{
		for (size_t i = 0; i < 300; ++i)
		{
			if (OtherPtr[i] != nullptr)
			{
				AFTEREFFECT(OtherPtr[i]);
			}
		}
	}

	if (m_bAfterEffect == false)
	{
		for (size_t i = 0; i < 300; ++i)
		{
			if (OtherPtr[i] != nullptr)
			{
				OtherPtr[i] = nullptr;
			}
		}
	}
}


void CShadowBetaScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CShadowBetaScript::update()
{
	SkillPos();

	SkillChangeState();

	if (SHADOWBETA_PrevSKILLState != SHADOWBETA_SkillState)
	{
		Skillstate();
	}
	SHADOWBETA_PrevSKILLState = SHADOWBETA_SkillState;

	EFFECT_PROCESSING();

	if (SHADOWBETA_SkillState == SHADOWFLASH_FSM::NONE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}


	return 0;
}

void CShadowBetaScript::Skillstate()
{
	switch (SHADOWBETA_SkillState)
	{
	case SHADOWFLASH_FSM::NONE:
		break;

	case SHADOWFLASH_FSM::ENABLE:
		SkillPos();
		Animator2D()->PlayAnim(L"400011100.special.frames");
		break;

	case SHADOWFLASH_FSM::PHASE1:
		Animator2D()->PlayAnim(L"400011100.tile.frames");
		break;

	case SHADOWFLASH_FSM::PHASE1_FINISH:
		Animator2D()->PlayAnim(L"400011100.finish.frames");
		break;

	case SHADOWFLASH_FSM::PHASE2:
		Animator2D()->PlayAnim(L"400011101.effect.frames");
		break;

	default:
		break;
	}
}

void CShadowBetaScript::SkillPos()
{
	if (SHADOWBETA_SkillState == SHADOWFLASH_FSM::NONE)
	{
		Transform()->SetLocalPos(DISABLE_POS);
	}
}

void CShadowBetaScript::SkillChangeState()
{
	//Player
	PlayerState = CPlayerScript::GetPlayerState();
	
	if (PlayerState != FSM::JUMP && PlayerState != FSM::FLASHJUMP && PlayerState != FSM::LADDER && PlayerState != FSM::ROPE && CPlayerScript::TerrainIn)
	{
		if (KEYTAB(KEY_TYPE::KEY_B) && PlayerState != FSM::BUFF && PlayerState != FSM::SHOUT  && PlayerState != FSM::DEATHFAULT && PlayerState != FSM::SHADOWFLASH_BETA)
		{
			if (PlayerState == FSM::PRONE)
			{
				return;
			}

			//

			for (size_t i = 0; i < BETAPHASE1_HIT_OF_TIME; ++i)
			{
				if (Critical_Calculate())
				{
					BetaDamage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 13.20f);
				}
				else
					BetaDamage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 13.20f);
			}

			//

			if (SHADOWBETA_SkillState == SHADOWFLASH_FSM::NONE)
			{
				CPlayerScript::PlayerSkillPos = CPlayerScript::GetPlayerPos();

				SHADOWBETA_SkillState = SHADOWFLASH_FSM::ENABLE;
				Beta_ActivePos();
				BetaOn = true;

				if (BetaOn)
				{
					CPlayerScript::SetPlayerState(FSM::SHADOWFLASH_BETA);
				}
			}

			else if (SHADOWBETA_SkillState == SHADOWFLASH_FSM::PHASE1)
			{
				//

				for (size_t i = 0; i < BETAPHASE2_HIT_OF_TIME; ++i)
				{
					if (Critical_Calculate())
					{
						Beta2Damage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 16.50f);
					}
					else
						Beta2Damage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 16.50f);
				}

				//

				CPlayerScript::PlayerSkillPos = vPos;

				SHADOWBETA_SkillState = SHADOWFLASH_FSM::PHASE2;
				SetPlayerMove();
				BetaOn = false;

				if (Animator2D()->GetCurAnim()->GetName() == L"400011101.effect.frames")
				{
					if (Animator2D()->GetCurAnim()->IsFinish())
					{
						CPlayerScript::SetPlayerState(FSM::STAND);
					}
				}
			}
		}

		if (Animator2D()->GetCurAnim() != nullptr)
		{
			if (Animator2D()->GetCurAnim()->GetName() == L"400011100.special.frames")
			{
				if (Animator2D()->GetCurAnim()->IsFinish())
				{
					CPlayerScript::SetPlayerState(FSM::STAND);
				}
			}
		}
	}


	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish()) //현재 스킬 오브젝트의 현재 프레임이 끝이거나 0 일 경우 NONE으로
		{
			if (SHADOWBETA_PrevSKILLState == SHADOWFLASH_FSM::ENABLE)
			{
				SHADOWBETA_SkillState = SHADOWFLASH_FSM::PHASE1;
			}
			else if (SHADOWBETA_SkillState == SHADOWFLASH_FSM::PHASE2)
			{
				SHADOWBETA_SkillState = SHADOWFLASH_FSM::NONE;

				if (!BetaOn && !CCraneScript::CraneAction)
				{
					CPlayerScript::SetPlayerState(FSM::STAND);
				}
			}
		}
	}
	return;
}



void CShadowBetaScript::Beta_ActivePos()
{
	vPos = CPlayerScript::GetPlayerPos();
	vPos.y = CPlayerScript::GetPlayerPos().y + 234;
	vPos.z = CPlayerScript::GetPlayerPos().z + 310;

	Transform()->SetLocalPos(vPos);

}

void CShadowBetaScript::SetPlayerMove()
{
	vPos = Transform()->GetLocalPos();
	vPos.y -= 224.f;

	/*for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->GetScript().size(); ++i)
	{
		if ((SCRIPT_TYPE)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->GetScript()[i]->GetScriptType()) == SCRIPT_TYPE::PLAYERSCRIPT)
		{
			PS = (CPlayerScript*)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->GetScript()[i]);
			PS->SetPlayerState(FSM::SHADOWFLASH_BETA);
			vPos.z = PS->GetGameObject()->Transform()->GetLocalPos().z;
			PS->GetGameObject()->Transform()->SetLocalPos(vPos);
		}
	}*/

	CPlayerScript::SetPlayerState(FSM::SHADOWFLASH_BETA);
	vPos.z = CPlayerScript::PlayerPos.z;
	CPlayerScript::SetPlayerPos(vPos);

}
