#include "stdafx.h"
#include "FallenScript.h"

#include "AnrealEngine/CollisionMgr.h"
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

#include "AreaScript.h"

#include "PlayerScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "PapulatusScript.h"
#include "EffectScript.h"

CFallenScript::CFallenScript()
	: OBJECT_State(PAPULATUS_OBJECT::NONE)
	, m_iScriptCount(0)
	, m_iEffectCount(-1)
	, AreaUpPos(0.f ,0.f ,0.f)
	, AreaUpScale(0.f, 0.f, 0.f)
	, AddGravity(0.f)
	, Pattern1(false)
	, PlayerCollisionReady(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::FALLENSCRIPT);
}


CFallenScript::~CFallenScript()
{
}



void CFallenScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

void CFallenScript::Skillstate()
{
	switch (OBJECT_State)
	{
	case PAPULATUS_OBJECT::NONE:
		break;
	case PAPULATUS_OBJECT::ING_1:
		Animator2D()->PlayAnim(L"BasicEff.img.ObtacleAtom.60.ing.frames");
		break;
	case PAPULATUS_OBJECT::ING_2:
		Animator2D()->PlayAnim(L"BasicEff.img.ObtacleAtom.61.ing.frames");
		break;
	case PAPULATUS_OBJECT::END_1:
		Animator2D()->PlayAnim(L"BasicEff.img.ObtacleAtom.60.end.frames");
		break;
	case PAPULATUS_OBJECT::END_2:
		Animator2D()->PlayAnim(L"BasicEff.img.ObtacleAtom.61.end.frames");
		break;
	case PAPULATUS_OBJECT::END:
		break;
	default:
		break;
	}
}

void CFallenScript::SkillPos()
{
	if (CPapulatusScript::AlramingCheck)
	{
		if (OBJECT_State == PAPULATUS_OBJECT::NONE && CPapulatusScript::PapulatusFallen)
		{
			AddGravity = 0.f;

			int FallenGearType = Well512Random::Instance().GetValue(1,10);
			if (FallenGearType % 2 == 0)
			{
				OBJECT_State = PAPULATUS_OBJECT::ING_1START;
				Pattern1 = true;
			}
			else
			{
				OBJECT_State = PAPULATUS_OBJECT::ING_2START;
				Pattern1 = false;
			}			
		}
		if(OBJECT_State == PAPULATUS_OBJECT::ING_1START || OBJECT_State == PAPULATUS_OBJECT::ING_2START)
		{
			vPos = CAreaScript::FallenPos;
			vPos.x = (float)Well512Random::Instance().GetFloatValue((vPos.x - (CAreaScript::AreaScale.x) * 0.5f), (vPos.x + (CAreaScript::AreaScale.x) * 0.5f));
			vPos.y += (float)Well512Random::Instance().GetValue(10, 100);
			vPos.z = 0.f;

			Transform()->SetLocalPos(vPos);
			return;
		}
		else
		{
			if (AddGravity == 0.f)
			{
				AddGravity = Well512Random::Instance().GetFloatValue(5.f, 500.f);
			}
		}
	}
	else if ((!CPapulatusScript::AlramingCheck) && OBJECT_State == PAPULATUS_OBJECT::NONE)
	{
	//	Transform()->SetLocalPos(DISABLE_POS);
		AddGravity = 0.f;
	}
}

void CFallenScript::SkillChangeState()
{	
	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->GetCurFrm() >= (int)(Animator2D()->GetCurAnim()->GetFrmVec().size() - 1))
		{
			if (OBJECT_State == PAPULATUS_OBJECT::ING_1START)
			{
				OBJECT_State = PAPULATUS_OBJECT::ING_1;
			}
			else if(OBJECT_State == PAPULATUS_OBJECT::ING_2START)
			{
				OBJECT_State = PAPULATUS_OBJECT::ING_2;
			}
		}

		if (Animator2D()->GetCurAnim()->IsFinish() && (OBJECT_State == PAPULATUS_OBJECT::END_1 || OBJECT_State == PAPULATUS_OBJECT::END_2))
		{
			OBJECT_State = PAPULATUS_OBJECT::NONE;
		}
	}
	return;
}


int CFallenScript::update()
{
	if (Pattern1)
	{
	//	vPos = { -0.0600f, -0.00500f, 0.0f };
		vPos = { -0.0600f, -0.0700f, 0.0f };
		Collider()->SetOffsetPos(vPos);
		vPos = { 0.33000f, 0.15000f, 0.0f };
		Collider()->SetOffsetScale(vPos);
	}
	else if (!Pattern1)
	{
//		vPos = { -0.0600f, -0.05000f, 0.0f };
		vPos = { -0.0600f, -0.13000f, 0.0f };
		Collider()->SetOffsetPos(vPos);
		vPos = { 0.35000f, 0.10000f, 0.0f };
		Collider()->SetOffsetScale(vPos);
	}

	if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PAPULATUS_OBJECT") != nullptr)
	{
		CCollisionMgr::GetInst()->CollisionCheck(L"PAPULATUS_OBJECT", L"TERRAIN");
		CCollisionMgr::GetInst()->CollisionCheck(L"PAPULATUS_OBJECT", L"PLAYER");
	}

	SkillPos();
	SkillChangeState();

	if (PREV_OBJECTState != OBJECT_State)
	{
		Skillstate();
	}
	PREV_OBJECTState = OBJECT_State;

	
	if (OBJECT_State == PAPULATUS_OBJECT::ING_1 || OBJECT_State == PAPULATUS_OBJECT::ING_2)
	{
		vPos = Transform()->GetLocalPos();
		vPos.y -= (9.8f + AddGravity)* DT;
		Transform()->SetLocalPos(vPos);
	}



	if (OBJECT_State == PAPULATUS_OBJECT::NONE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}
	
	return 0;
}

void CFallenScript::OnCollisionEnter(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
	{
		if (OBJECT_State == PAPULATUS_OBJECT::ING_1)
		{
			OBJECT_State = PAPULATUS_OBJECT::END_1;
		}
		else if (OBJECT_State == PAPULATUS_OBJECT::ING_2)
		{
			OBJECT_State = PAPULATUS_OBJECT::END_2;
		}
	}
	else if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		if (OBJECT_State == PAPULATUS_OBJECT::ING_1)
		{
			OBJECT_State = PAPULATUS_OBJECT::END_1;
		}
		else if (OBJECT_State == PAPULATUS_OBJECT::ING_2)
		{
			OBJECT_State = PAPULATUS_OBJECT::END_2;
		}
	}
	if (_pOther->GetGameObject()->GetName() == L"Player")
	{
		if (!PlayerCollisionReady)
		{
			// 피격시 할 행동 여기에 Todo
			PlayerCollisionReady = true;
		}
	}
}

void CFallenScript::OnCollision(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"Player")
	{
		if ((OBJECT_State == PAPULATUS_OBJECT::END_1 || OBJECT_State == PAPULATUS_OBJECT::END_2) && (PlayerCollisionReady))
		{
			// 피격시 할 행동 여기에 Todo
			
			if (OBJECT_State == PAPULATUS_OBJECT::END_1)
			{
				if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
				{
					if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
					{
						CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
						CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

						CPlayerScript::PlayerObject->AddChild(EffectPtr);
						((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PAPULATUS_OBJECT_60;
					}
				}
			}
			else if (OBJECT_State == PAPULATUS_OBJECT::END_2)
			{
				if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
				{
					if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
					{
						CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
						CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

						CPlayerScript::PlayerObject->AddChild(EffectPtr);
						((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PAPULATUS_OBJECT_61;
					}
				}
			}

			PlayerCollisionReady = false;
		}
	}
}

void CFallenScript::OnCollisionExit(CCollider * _pOther)
{
	//if (_pOther->GetGameObject()->GetName() == L"Player")
	//{
	//	if ((OBJECT_State == PAPULATUS_OBJECT::END_1 || OBJECT_State == PAPULATUS_OBJECT::END_2) && (!PlayerCollisionReady))
	//	{
	//		PlayerCollisionReady = true;
	//		// Player에게 피격;
	//	}
	//}
}

