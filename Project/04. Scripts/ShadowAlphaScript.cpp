#include "stdafx.h"
#include "ShadowAlphaScript.h"


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

CShadowAlphaScript::CShadowAlphaScript()
	: AlphaON(false)
	, AlphaDamage{}
	, Alpha2Damage{}
	, m_bAfterEffect(false)
	, OtherCount(0)
{
	SetScriptType((UINT)SCRIPT_TYPE::SHADOWALPHASCRIPT);
}

CShadowAlphaScript::~CShadowAlphaScript()
{
}

bool CShadowAlphaScript::Critical_Calculate()
{
	Critical_Value = Well512Random::Instance().GetValue(0, 100);

	if ((CPlayerScript::CRITICAL + CPlayerScript::MAPLESOLDIER) >= Critical_Value)
	{
		return true;
	}
	else
		return false;
}


void CShadowAlphaScript::AFTEREFFECT(CCollider * _pOther)
{
	if (SHADOWALPHA_SkillState == SHADOWFLASH_FSM::NONE && SHADOWALPHA_SkillState != SHADOWFLASH_FSM::ENABLE && SHADOWALPHA_SkillState != SHADOWFLASH_FSM::PHASE1 && SHADOWALPHA_SkillState != SHADOWFLASH_FSM::PHASE2)
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
								_pOther->GetGameObject()->GetScript()[i]->AlphaCount += DT;

								((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);
								((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::ALPHAPHASE_HIT);
								((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;


								if (_pOther->GetGameObject()->GetScript()[i]->AlphaCount <= 0.1f)
								{
									return;
								}

								if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
								{
									if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
									{
										CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
										CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

										((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount;

										_pOther->GetGameObject()->AddChild(EffectPtr);
										((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::ALPHAPHASE_HIT;

									}
								}


								//((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(AlphaDamage[_pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount]);


								_pOther->GetGameObject()->GetScript()[i]->AlphaCount = 0.f;
								_pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

								if (ALPHAPHASE2_HIT_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount)
								{
									_pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount = 0;
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

void CShadowAlphaScript::OnCollisionEnter(CCollider * _pOther)
{
	if (SHADOWALPHA_SkillState == SHADOWFLASH_FSM::PHASE1 || SHADOWALPHA_SkillState == SHADOWFLASH_FSM::PHASE1_FINISH || SHADOWALPHA_SkillState == SHADOWFLASH_FSM::PHASE2 || SHADOWALPHA_SkillState == SHADOWFLASH_FSM::ENABLE)
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
							if (_pOther->GetGameObject()->GetScript()[i]->AlphaCollision == false)
							{
								_pOther->GetGameObject()->GetScript()[i]->AlphaCollision = true;
							}
						}
					}
				}
			}
		}
	}
}

void CShadowAlphaScript::OnCollision(CCollider * _pOther)
{
	if (SHADOWALPHA_SkillState != SHADOWFLASH_FSM::END && SHADOWALPHA_SkillState != SHADOWFLASH_FSM::NONE && SHADOWALPHA_SkillState != SHADOWFLASH_FSM::PHASE2)
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
								if (m_bAfterEffect == false)
								{
									_pOther->GetGameObject()->GetScript()[i]->AlphaCount += DT;

									if ((CScript*)(_pOther->GetGameObject()->GetScript()[i])->AlphaCollision)
									{
										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);
										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::ALPHAPHASE_HIT);
										((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;


										if (_pOther->GetGameObject()->GetScript()[i]->AlphaCount <= 0.1f)
										{
											return;
										}

										if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
										{
											if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
											{
												CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
												CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

												((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount;

												_pOther->GetGameObject()->AddChild(EffectPtr);
												((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::ALPHAPHASE_HIT;
											}
										}


										//									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(AlphaDamage[_pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount]);


										_pOther->GetGameObject()->GetScript()[i]->AlphaCount = 0.f;
										_pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

										if (ALPHAPHASE1_HIT_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount)
										{
											_pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount = 0;
											_pOther->GetGameObject()->GetScript()[i]->AlphaCollision = false;
										}
									}

									////Acc DT
									_pOther->GetGameObject()->GetScript()[i]->AlphaTime += DT;
									if (_pOther->GetGameObject()->GetScript()[i]->AlphaTime < 4.3f)
									{
										return;
									}
									_pOther->GetGameObject()->GetScript()[i]->AlphaCollision = true;
									_pOther->GetGameObject()->GetScript()[i]->AlphaTime = 0;
								}
							}
						}
					}
				}
			}
		}
	}
}

void CShadowAlphaScript::OnCollisionExit(CCollider * _pOther)
{
	if (SHADOWALPHA_SkillState == SHADOWFLASH_FSM::NONE)
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
							_pOther->GetGameObject()->GetScript()[i]->AlphaCollision = true;
							_pOther->GetGameObject()->GetScript()[i]->AlphaCount = 0.f;

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
										if (OtherPtr[OtherCount + 1 ] == nullptr)
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

void CShadowAlphaScript::EFFECT_PROCESSING()
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

void CShadowAlphaScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CShadowAlphaScript::update()
{
//	CCollisionMgr::GetInst()->CollisionCheck(L"Default", L"MONSTER");

	SkillPos(); // Delete

	SkillChangeState();

	if (SHADOWALPHA_PrevSKILLState != SHADOWALPHA_SkillState)
	{
		Skillstate();
	}

	SHADOWALPHA_PrevSKILLState = SHADOWALPHA_SkillState;

	EFFECT_PROCESSING();
	
	if (SHADOWALPHA_SkillState == SHADOWFLASH_FSM::NONE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}

void CShadowAlphaScript::Skillstate()
{
	switch (SHADOWALPHA_SkillState)
	{
	case SHADOWFLASH_FSM::NONE:
		break;

	case SHADOWFLASH_FSM::ENABLE:
		SkillPos();
		Animator2D()->PlayAnim(L"400011098.special.frames");
		break;

	case SHADOWFLASH_FSM::PHASE1:
		Animator2D()->PlayAnim(L"400011098.tile.frames");
		break;

	case SHADOWFLASH_FSM::PHASE1_FINISH:
		Animator2D()->PlayAnim(L"400011098.finish.frames");
		break;

	case SHADOWFLASH_FSM::PHASE2:
		Animator2D()->PlayAnim(L"400011099.effect.frames");
		break;

	default:
		break;
	}
}

void CShadowAlphaScript::SkillPos()
{
	if (SHADOWALPHA_SkillState == SHADOWFLASH_FSM::NONE)
	{
		Transform()->SetLocalPos(DISABLE_POS);
	}
}

void CShadowAlphaScript::SkillChangeState()
{
	//Player
	PlayerState = CPlayerScript::GetPlayerState();

	if (PlayerState != FSM::JUMP && PlayerState != FSM::FLASHJUMP && PlayerState != FSM::LADDER && PlayerState != FSM::ROPE && CPlayerScript::TerrainIn)
	{
		if (KEYTAB(KEY_TYPE::KEY_A) && PlayerState != FSM::BUFF && PlayerState != FSM::SHOUT  && PlayerState != FSM::DEATHFAULT && PlayerState != FSM::SHADOWFLASH_BETA)
		{
			if (PlayerState == FSM::PRONE)
			{
				return;
			}

			//

			for (size_t i = 0; i < ALPHAPHASE1_HIT_OF_TIME; ++i)
			{
				if (Critical_Calculate())
				{
					AlphaDamage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 11.f);
				}
				else
					AlphaDamage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 11.f);
			}

			//

			if (SHADOWALPHA_SkillState == SHADOWFLASH_FSM::NONE)
			{
				CPlayerScript::PlayerSkillPos = CPlayerScript::GetPlayerPos();

				SHADOWALPHA_SkillState = SHADOWFLASH_FSM::ENABLE;
				Alpha_ActivePos();
				AlphaON = true;

				if (AlphaON)
				{
					CPlayerScript::SetPlayerState(FSM::SHADOWFLASH_ALPHA);
				}
			}

			else if (SHADOWALPHA_SkillState == SHADOWFLASH_FSM::PHASE1)
			{
				//
				for (size_t i = 0; i < ALPHAPHASE2_HIT_OF_TIME; ++i)
				{
					if (Critical_Calculate())
					{
						Alpha2Damage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 8.8f);
					}
					else
						Alpha2Damage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 8.8f);
				}
				//

				CPlayerScript::PlayerSkillPos = vPos;

				SHADOWALPHA_SkillState = SHADOWFLASH_FSM::PHASE2;							
				SetPlayerMove();
				AlphaON = false;

				if (Animator2D()->GetCurAnim()->GetName() == L"400011099.effect.frames")
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
			if (Animator2D()->GetCurAnim()->GetName() == L"400011098.special.frames")
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
			if (SHADOWALPHA_PrevSKILLState == SHADOWFLASH_FSM::ENABLE)
			{
				SHADOWALPHA_SkillState = SHADOWFLASH_FSM::PHASE1;
			}
			else if (SHADOWALPHA_SkillState == SHADOWFLASH_FSM::PHASE2)
			{
				SHADOWALPHA_SkillState = SHADOWFLASH_FSM::NONE;

				if (!AlphaON && !CCraneScript::CraneAction)
				{
					CPlayerScript::SetPlayerState(FSM::STAND);
				}
			}
		}
	}
	return;
}



void CShadowAlphaScript::Alpha_ActivePos()
{
	vPos.x = CPlayerScript::GetPlayerPos().x + 8;
	vPos.y = CPlayerScript::GetPlayerPos().y + 170;
	vPos.z = CPlayerScript::GetPlayerPos().z + 300;

	Transform()->SetLocalPos(vPos);

}

void CShadowAlphaScript::SetPlayerMove()
{
	vPos = Transform()->GetLocalPos();
	vPos.x -= 8.f;
	vPos.y -= 160.f;

	/*for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->GetScript().size(); ++i)
	{
		if ((SCRIPT_TYPE)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->GetScript()[i]->GetScriptType()) == SCRIPT_TYPE::PLAYERSCRIPT)
		{
			PS = (CPlayerScript*)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->GetScript()[i]);
			CPlayerScript::SetPlayerState(FSM::SHADOWFLASH_ALPHA);
			vPos.z = CPlayerScript::PlayerPos().z;
	
			CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->SetLocalPos(vPos);
			PS->GetGameObject()->Transform()->SetLocalPos(vPos);
		}
	}*/


	CPlayerScript::SetPlayerState(FSM::SHADOWFLASH_ALPHA);
	vPos.z = CPlayerScript::PlayerPos.z;
	CPlayerScript::SetPlayerPos(vPos);
}
