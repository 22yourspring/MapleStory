#include "stdafx.h"
#include "SoulEclipseScript.h"

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

#include "CameraScript.h"
#include "PlayerScript.h"
#include "RagingBlowScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "EffectScript.h"

bool CSoulEclipseScript::Eclipse2Phase = false;		// EFFECT OUT을 구분하기 위해서
bool CSoulEclipseScript::EclipseParticle = false;	// Particle 없애는 시점을 구분하기 위해서
bool CSoulEclipseScript::Eclipse2Can = true;		// 생성되는 시점 구분
bool CSoulEclipseScript::Explosion = false;			// SUNSET REMOVE 시점
bool CSoulEclipseScript::Eclipse = true;


SOULECLIPSE_FSM	CSoulEclipseScript::MOON_State = SOULECLIPSE_FSM::NONE;
SOULECLIPSE_FSM	CSoulEclipseScript::EFFECT_State = SOULECLIPSE_FSM::NONE;
SOULECLIPSE_FSM	CSoulEclipseScript::PARTICLE_State = SOULECLIPSE_FSM::NONE;
SOULECLIPSE_FSM	CSoulEclipseScript::SUNSET_State = SOULECLIPSE_FSM::NONE;
SOULECLIPSE_FSM	CSoulEclipseScript::OUTLINE_State = SOULECLIPSE_FSM::NONE;

SOULECLIPSE_FSM	CSoulEclipseScript::PrevMOON_State = SOULECLIPSE_FSM::NONE;
SOULECLIPSE_FSM	CSoulEclipseScript::PrevEFFECT_State = SOULECLIPSE_FSM::NONE;
SOULECLIPSE_FSM	CSoulEclipseScript::PrevPARTICLE_State = SOULECLIPSE_FSM::NONE;
SOULECLIPSE_FSM	CSoulEclipseScript::PrevSUNSET_State = SOULECLIPSE_FSM::NONE;
SOULECLIPSE_FSM	CSoulEclipseScript::PrevOUTLINE_State = SOULECLIPSE_FSM::NONE;



CSoulEclipseScript::CSoulEclipseScript()
	: m_bAfterEffect(false)
	, OtherCount(0)
{
	SetScriptType((UINT)SCRIPT_TYPE::SOULECLIPSESCRIPT);
}

CSoulEclipseScript::~CSoulEclipseScript()
{
}

bool CSoulEclipseScript::Critical_Calculate()
{
	Critical_Value = Well512Random::Instance().GetValue(0, 100);

	if (CPlayerScript::CRITICAL >= Critical_Value)
	{
		return true;
	}
	else
		return false;
}


void CSoulEclipseScript::AFTEREFFECT(CCollider * _pOther)
{
	if (MOON_State == SOULECLIPSE_FSM::NONE && MOON_State != SOULECLIPSE_FSM::MOONAPPEAR && MOON_State != SOULECLIPSE_FSM::MOONPHASE1 && MOON_State != SOULECLIPSE_FSM::MOONPHASE2)
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
								_pOther->GetGameObject()->GetScript()[i]->EclipseCount += DT;

								((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);
								((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::SOUL_ECLIPSE_PHASE);
								((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;

								if (_pOther->GetGameObject()->GetScript()[i]->EclipseCount <= 0.12f)
								{
									return;
								}

								if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
								{
									if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
									{
										CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
										CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

										((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->EclipseEffectCount;

										_pOther->GetGameObject()->AddChild(EffectPtr);
										((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::SOUL_ECLIPSE_PHASE;

									}
								}


								//((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(AlphaDamage[_pOther->GetGameObject()->GetScript()[i]->AlphaEffectCount]);

								_pOther->GetGameObject()->GetScript()[i]->EclipseCount = 0.f;
								_pOther->GetGameObject()->GetScript()[i]->EclipseEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

								if (SOUL_ECLIPSE_PHASE2_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->EclipseEffectCount)
								{
									_pOther->GetGameObject()->GetScript()[i]->EclipseEffectCount = 0;
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

void CSoulEclipseScript::EFFECT_PROCESSING()
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

void CSoulEclipseScript::OnCollisionEnter(CCollider * _pOther)
{
	if (MOON_State == SOULECLIPSE_FSM::MOONPHASE1 || MOON_State == SOULECLIPSE_FSM::MOONPHASE2 || MOON_State == SOULECLIPSE_FSM::MOONAPPEAR)
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
							if (_pOther->GetGameObject()->GetScript()[i]->EclipseCollision == false)
							{
								_pOther->GetGameObject()->GetScript()[i]->EclipseCollision = true;
							}
						}
					}
				}
			}
		}
	}
}

void CSoulEclipseScript::OnCollision(CCollider * _pOther)
{
	if (MOON_State != SOULECLIPSE_FSM::END && MOON_State != SOULECLIPSE_FSM::NONE && MOON_State != SOULECLIPSE_FSM::MOONPHASE2)
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
								_pOther->GetGameObject()->GetScript()[i]->EclipseCount += DT;


								if ((CScript*)(_pOther->GetGameObject()->GetScript()[i])->EclipseCollision)
								{
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetPushed(true);
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SetSkillType((UINT)SKILL_TYPE::SOUL_ECLIPSE_PHASE);
									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->m_bAggro = true;


									if (_pOther->GetGameObject()->GetScript()[i]->EclipseCount <= 0.12f)
									{
										return;
									}

									if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
									{
										if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
										{
											CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
											CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

											((CEffectScript*)(EffectPtr->GetScript()[0]))->EffectSequence = _pOther->GetGameObject()->GetScript()[i]->EclipseEffectCount;

											_pOther->GetGameObject()->AddChild(EffectPtr);
											((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::SOUL_ECLIPSE_PHASE;

										}
									}


									//데미지 계산 해야함

									//									((CScript*)(_pOther->GetGameObject()->GetScript()[i]))->SubHP(EclipseDamage[_pOther->GetGameObject()->GetScript()[i]->EclipseEffectCount]);


									_pOther->GetGameObject()->GetScript()[i]->EclipseCount = 0.f;
									_pOther->GetGameObject()->GetScript()[i]->EclipseEffectCount += 1;				// 최초 1회 재생 후 안되는 이유 검수해서 다시 보기 여기까지 했음.

									if (SOUL_ECLIPSE_PHASE1_OF_TIME <= _pOther->GetGameObject()->GetScript()[i]->EclipseEffectCount)
									{
										_pOther->GetGameObject()->GetScript()[i]->EclipseEffectCount = 0;
										_pOther->GetGameObject()->GetScript()[i]->EclipseCollision = false;
									}
								}

								////Acc DT
								_pOther->GetGameObject()->GetScript()[i]->EclipseTime += DT;
								if (_pOther->GetGameObject()->GetScript()[i]->EclipseTime < 1.f)
								{
									return;
								}
								_pOther->GetGameObject()->GetScript()[i]->EclipseCollision = true;
								_pOther->GetGameObject()->GetScript()[i]->EclipseTime = 0;
							}
						}
					}
				}
			}
		}
	}
}

void CSoulEclipseScript::OnCollisionExit(CCollider * _pOther)
{
	if (MOON_State == SOULECLIPSE_FSM::NONE)
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
							_pOther->GetGameObject()->GetScript()[i]->EclipseCollision = true;
							_pOther->GetGameObject()->GetScript()[i]->EclipseCount = 0.f;

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

void CSoulEclipseScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CSoulEclipseScript::update()
{
	SkillPos();

	SkillChangeState();

	if (MOON_State != PrevMOON_State)
	{
		MOONSkillstate();
	}
	PrevMOON_State = MOON_State;

	if (EFFECT_State != PrevEFFECT_State)
	{
		EFFECTSkillstate();
	}
	PrevEFFECT_State = EFFECT_State;

	if (PARTICLE_State != PrevPARTICLE_State)
	{
		PARTICLESkillstate();
	}
	PrevPARTICLE_State = PARTICLE_State;

	if (SUNSET_State != PrevSUNSET_State)
	{
		SUNSETSkillstate();
	}
	PrevSUNSET_State = SUNSET_State;

	if (OUTLINE_State != PrevOUTLINE_State)
	{
		OUTLINESkillstate();
	}
	PrevOUTLINE_State = OUTLINE_State;

	if (MOON_State == SOULECLIPSE_FSM::MOONAPPEAR || MOON_State == SOULECLIPSE_FSM::MOONPHASE1 || MOON_State == SOULECLIPSE_FSM::MOONPHASE2)
	{
		CPlayerScript::PlayerSkillPos = CPlayerScript::GetPlayerPos();
	}

	EFFECT_PROCESSING();

	return 0;
}

void CSoulEclipseScript::MOONSkillstate()
{
	switch (MOON_State)
	{
	case SOULECLIPSE_FSM::NONE:
		Animator2D()->PlayAnim(L"blink");
		break;

	case SOULECLIPSE_FSM::MOONAPPEAR:
		Animator2D()->PlayAnim(L"MOONAPPEAR");
		break;

	case SOULECLIPSE_FSM::MOONPHASE1:
		Animator2D()->PlayAnim(L"MOONPHASE1");
		break;

	case SOULECLIPSE_FSM::MOONPHASE2:
		Animator2D()->PlayAnim(L"MOONPHASE2");

		break;

	case SOULECLIPSE_FSM::EFFECT_IN:
		break;

	case SOULECLIPSE_FSM::EFFECT_OUT:
		break;

	case SOULECLIPSE_FSM::PARTICLE:
		break;

	case SOULECLIPSE_FSM::PARTICLE_EXPLOTION:
		break;

	case SOULECLIPSE_FSM::SUNSET:
		break;

	case SOULECLIPSE_FSM::OUTLINE:
		break;

	case SOULECLIPSE_FSM::END:
		break;

	default:
		break;
	}
}

void CSoulEclipseScript::EFFECTSkillstate()
{
	switch (EFFECT_State)
	{
	case SOULECLIPSE_FSM::NONE:
		Animator2D()->PlayAnim(L"blink");
		break;

	case SOULECLIPSE_FSM::MOONAPPEAR:
		break;

	case SOULECLIPSE_FSM::MOONPHASE1:
		break;

	case SOULECLIPSE_FSM::MOONPHASE2:
		break;

	case SOULECLIPSE_FSM::EFFECT_IN:
		Animator2D()->PlayAnim(L"EFFECT_IN");
		ECLIPSE_ACTIVE = ECLIPSE_ENABLE::ENABLE;
		break;

	case SOULECLIPSE_FSM::EFFECT_OUT:
		Animator2D()->PlayAnim(L"EFFECT_OUT");
		ECLIPSE_ACTIVE = ECLIPSE_ENABLE::DISABLE;
		break;

	case SOULECLIPSE_FSM::PARTICLE:
		break;

	case SOULECLIPSE_FSM::PARTICLE_EXPLOTION:
		break;

	case SOULECLIPSE_FSM::SUNSET:
		break;

	case SOULECLIPSE_FSM::OUTLINE:
		break;

	case SOULECLIPSE_FSM::END:
		break;

	default:
		break;
	}
}

void CSoulEclipseScript::PARTICLESkillstate()
{
	switch (PARTICLE_State)
	{
	case SOULECLIPSE_FSM::NONE:
		Animator2D()->PlayAnim(L"blink");
		break;

	case SOULECLIPSE_FSM::MOONAPPEAR:
		break;

	case SOULECLIPSE_FSM::MOONPHASE1:
		break;

	case SOULECLIPSE_FSM::MOONPHASE2:
		break;

	case SOULECLIPSE_FSM::EFFECT_IN:
		break;

	case SOULECLIPSE_FSM::EFFECT_OUT:
		break;

	case SOULECLIPSE_FSM::PARTICLE:
		Animator2D()->PlayAnim(L"PARTICLE");
		break;

	case SOULECLIPSE_FSM::PARTICLE_EXPLOTION:
		Animator2D()->PlayAnim(L"PARTICLE_EXPLOTION");
		break;

	case SOULECLIPSE_FSM::SUNSET:
		break;

	case SOULECLIPSE_FSM::OUTLINE:
		break;

	case SOULECLIPSE_FSM::END:
		break;

	default:
		break;
	}
}

void CSoulEclipseScript::SUNSETSkillstate()
{
	switch (SUNSET_State)
	{
	case SOULECLIPSE_FSM::NONE:
		Animator2D()->PlayAnim(L"blink");
		break;

	case SOULECLIPSE_FSM::MOONAPPEAR:
		break;

	case SOULECLIPSE_FSM::MOONPHASE1:
		break;

	case SOULECLIPSE_FSM::MOONPHASE2:
		break;

	case SOULECLIPSE_FSM::EFFECT_IN:
		break;

	case SOULECLIPSE_FSM::EFFECT_OUT:
		break;

	case SOULECLIPSE_FSM::PARTICLE:
		break;

	case SOULECLIPSE_FSM::PARTICLE_EXPLOTION:
		break;

	case SOULECLIPSE_FSM::SUNSET:
		Animator2D()->PlayAnim(L"SUNSET");
		break;

	case SOULECLIPSE_FSM::OUTLINE:
		break;

	case SOULECLIPSE_FSM::END:
		break;

	default:
		break;
	}
}

void CSoulEclipseScript::OUTLINESkillstate()
{
	switch (OUTLINE_State)
	{
	case SOULECLIPSE_FSM::NONE:
		Animator2D()->PlayAnim(L"blink");
		break;

	case SOULECLIPSE_FSM::MOONAPPEAR:
		break;

	case SOULECLIPSE_FSM::MOONPHASE1:
		break;

	case SOULECLIPSE_FSM::MOONPHASE2:
		break;

	case SOULECLIPSE_FSM::EFFECT_IN:
		break;

	case SOULECLIPSE_FSM::EFFECT_OUT:
		break;

	case SOULECLIPSE_FSM::PARTICLE:
		break;

	case SOULECLIPSE_FSM::PARTICLE_EXPLOTION:
		break;

	case SOULECLIPSE_FSM::SUNSET:
		break;

	case SOULECLIPSE_FSM::OUTLINE:
		Animator2D()->PlayAnim(L"OUTLINE");
		break;

	case SOULECLIPSE_FSM::END:
		break;

	default:
		break;
	}
}


void CSoulEclipseScript::SkillPos()
{
	findCS();

	if (CS == nullptr)
	{
		return;
	}
	vPos = CS->GetCameraPos();


	if (GetGameObject()->GetName() == L"ECLIPSE_SUNSET")
	{
		vPos.z = 991.f;
	}
	if (GetGameObject()->GetName() == L"ECLIPSE_MOON")
	{
		if (MOON_State == SOULECLIPSE_FSM::NONE)
		{
			vPos = DISABLE_POS;
		}
		vPos.z = 300.f;
	}
	if (GetGameObject()->GetName() == L"ECLIPSE_EFFECT")
	{
		vPos.z = 0.f;
	}
	if (GetGameObject()->GetName() == L"ECLIPSE_PARTICLE")
	{
		vPos.z = 100.f;
	}
	if (GetGameObject()->GetName() == L"ECLIPSE_OUTLINE")
	{
		vPos.z = 0.f;
	}
	Transform()->SetLocalPos(vPos);
	return;
}

void CSoulEclipseScript::SkillChangeState()
{
	PlayerState = CPlayerScript::GetPlayerState();
		
	if (PlayerState != FSM::JUMP && PlayerState != FSM::FLASHJUMP && PlayerState != FSM::LADDER && PlayerState != FSM::ROPE && CPlayerScript::TerrainIn)
	{
		if (KEYTAB(KEY_TYPE::KEY_E) && PlayerState != FSM::BUFF && PlayerState != FSM::SHOUT  && PlayerState != FSM::DEATHFAULT && PlayerState != FSM::SHADOWFLASH_BETA && PlayerState != FSM::SHADOWFLASH_ALPHA && PlayerState != FSM::DEATHFAULT)
		{
			if (PlayerState == FSM::PRONE)
			{
				return;
			}

			if (GetGameObject()->GetName() == L"ECLIPSE_MOON")
			{
				if (MOON_State == SOULECLIPSE_FSM::NONE)
				{
					//
					for (size_t i = 0; i < SOUL_ECLIPSE_PHASE1_OF_TIME; ++i)
					{
						if (Critical_Calculate())
						{
							EclipseDamage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 11.f);
						}
						else
							EclipseDamage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 11.f);
					}
					//

					MOON_State = SOULECLIPSE_FSM::MOONAPPEAR;
				}

				else if (MOON_State == SOULECLIPSE_FSM::MOONPHASE1)
				{
					//
					for (size_t i = 0; i < SOUL_ECLIPSE_PHASE2_OF_TIME; ++i)
					{
						if (Critical_Calculate())
						{
							Eclipse2Damage[i] = (int)(Well512Random::Instance().GetFloatValue(CPlayerScript::MIN_ATT * 1.2f, CPlayerScript::MAX_ATT * 1.2f) * 11.f);
						}
						else
							Eclipse2Damage[i] = (int)(Well512Random::Instance().GetFloatValue((float)CPlayerScript::MIN_ATT, (float)CPlayerScript::MAX_ATT) * 11.f);
					}
					//

					Eclipse2Can = false;
					MOON_State = SOULECLIPSE_FSM::MOONPHASE2;
				}
			}

			if (GetGameObject()->GetName() == L"ECLIPSE_EFFECT")
			{
				if (EFFECT_State == SOULECLIPSE_FSM::NONE && ECLIPSE_ACTIVE == ECLIPSE_ENABLE::ENABLE)
				{
					if (Eclipse2Phase)
					{
						EFFECT_State = SOULECLIPSE_FSM::EFFECT_OUT;
						Eclipse2Phase = false;
					}
					//EFFECT_State = SOULECLIPSE_FSM::EFFECT_OUT;
				}
				else if (EFFECT_State == SOULECLIPSE_FSM::NONE && Eclipse2Can)
				{
					EFFECT_State = SOULECLIPSE_FSM::EFFECT_IN;
				}

			}

			if (GetGameObject()->GetName() == L"ECLIPSE_PARTICLE")
			{
				if (PARTICLE_State == SOULECLIPSE_FSM::NONE && Eclipse2Can)
				{
					PARTICLE_State = SOULECLIPSE_FSM::PARTICLE;
				}

				else if (PARTICLE_State == SOULECLIPSE_FSM::PARTICLE)
				{
					if (EclipseParticle)
					{
						PARTICLE_State = SOULECLIPSE_FSM::NONE;
						EclipseParticle = false;
					}
					//	PARTICLE_State = SOULECLIPSE_FSM::NONE;
				}
			}

			if (GetGameObject()->GetName() == L"ECLIPSE_SUNSET")
			{
				if (SUNSET_State == SOULECLIPSE_FSM::NONE && Eclipse)
				{
					SUNSET_State = SOULECLIPSE_FSM::SUNSET;
					Eclipse = false;
				}
				else if (SUNSET_State == SOULECLIPSE_FSM::SUNSET)
				{
					//Explosion = true;

					//SUNSET_State = SOULECLIPSE_FSM::NONE;
				}
			}

			if (GetGameObject()->GetName() == L"ECLIPSE_OUTLINE")
			{
				if (OUTLINE_State == SOULECLIPSE_FSM::NONE && Eclipse)
				{
					OUTLINE_State = SOULECLIPSE_FSM::OUTLINE;
				}
				else if (OUTLINE_State == SOULECLIPSE_FSM::OUTLINE)
				{
					//OUTLINE_State = SOULECLIPSE_FSM::NONE;
				}
			}
		}

		if (GetGameObject()->GetName() == L"ECLIPSE_SUNSET")
		{
			if (SUNSET_State == SOULECLIPSE_FSM::SUNSET && Explosion)
			{
				SUNSET_State = SOULECLIPSE_FSM::NONE;
				Explosion = false;
			}
		}

		if (GetGameObject()->GetName() == L"ECLIPSE_OUTLINE")
		{
			if (OUTLINE_State == SOULECLIPSE_FSM::OUTLINE && Explosion)
			{
				OUTLINE_State = SOULECLIPSE_FSM::NONE;
			}
		}
	}


	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (GetGameObject()->GetName() == L"ECLIPSE_MOON")
		{
			if (Animator2D()->GetCurAnim()->IsFinish()) //현재 스킬 오브젝트의 현재 프레임이 끝이거나 0 일 경우 NONE으로
			{
				if (PrevMOON_State == SOULECLIPSE_FSM::MOONAPPEAR)
				{
					MOON_State = SOULECLIPSE_FSM::MOONPHASE1;
					Eclipse2Phase = true;
					EclipseParticle = true;
				}
				else if (MOON_State == SOULECLIPSE_FSM::MOONPHASE2)
				{
					MOON_State = SOULECLIPSE_FSM::NONE;
					Eclipse2Can = true;
					Eclipse = true;
				}
			}
		}

		else if (GetGameObject()->GetName() == L"ECLIPSE_EFFECT")
		{
			if (Animator2D()->GetCurAnim()->IsFinish()) //현재 스킬 오브젝트의 현재 프레임이 끝이거나 0 일 경우 NONE으로
			{
				if (EFFECT_State == SOULECLIPSE_FSM::EFFECT_IN && ECLIPSE_ACTIVE == ECLIPSE_ENABLE::ENABLE)
				{
					EFFECT_State = SOULECLIPSE_FSM::NONE;
				}

				else if (EFFECT_State == SOULECLIPSE_FSM::EFFECT_OUT && ECLIPSE_ACTIVE == ECLIPSE_ENABLE::DISABLE)
				{
					EFFECT_State = SOULECLIPSE_FSM::NONE;
					if (Animator2D()->GetCurAnim()->IsFinish())
					{	
						PARTICLE_State = SOULECLIPSE_FSM::PARTICLE_EXPLOTION;

						Explosion = true;
					}
				}

				else if (PARTICLE_State == SOULECLIPSE_FSM::PARTICLE_EXPLOTION)
				{
					PARTICLE_State = SOULECLIPSE_FSM::NONE;
				}
			}
		}

		//else if (GetGameObject()->GetName() == L"ECLIPSE_PARTICLE")
		//{
		//	if (Animator2D()->GetCurAnim()->IsFinish()) //현재 스킬 오브젝트의 현재 프레임이 끝이거나 0 일 경우 NONE으로
		//	{
		//		if (PARTICLE_State == SOULECLIPSE_FSM::PARTICLE_EXPLOTION)
		//		{
		//			PARTICLE_State = SOULECLIPSE_FSM::NONE;
		//		}
		//	}
		//}
	}
	return;
}

void CSoulEclipseScript::findCS()
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
