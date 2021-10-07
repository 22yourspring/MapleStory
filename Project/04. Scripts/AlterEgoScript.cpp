#include "stdafx.h"
#include "AlterEgoScript.h"

#include "GravityScript.h"
#include "AnrealEngine/Transform.h"
#include "AnrealEngine/ResMgr.h"
#include "AnrealEngine/TimeMgr.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"

#include "AnrealEngine/CollisionMgr.h"
#include "AnrealEngine/SceneMgr.h"
#include "ScriptMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/Layer.h"


#include "AnrealEngine/define.h"
#include "AnrealEngine/Collider.h"

#include "PapulatusScript.h"
#include "PlayerScript.h"

#include "EffectScript.h"

#include "AnrealEngine/Well512Random.h"

int CAlterEgoScript::AlterEgoActiveCount = 0;

Vec3 CAlterEgoScript::vParentPos = { 0.f, 0.f, 0.f };

const long long CAlterEgoScript::ALTEREGO_MAX_HP = 16000000;
const long long CAlterEgoScript::ALTEREGO_MAX_MP = 20000;

CAlterEgoScript::CAlterEgoScript()
	: CScript(CAlterEgoScript::ALTEREGO_MAX_HP, CAlterEgoScript::ALTEREGO_MAX_MP, CAlterEgoScript::ALTEREGO_MAX_HP, CAlterEgoScript::ALTEREGO_MAX_MP, (UINT)SKILL_TYPE::NONE, false, false, false, false, false, false, false, false, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0, 0)
	, m_Left(true)
	, m_bTracing(false)
	, Patrol_On(false)
	, m_bPatrolLeft(true)
	, m_bPatrolRight(true)
	, m_MonsterState(PAPULATUS_FSM::NONE)
	, MonsterTerrain(nullptr)
	, m_fSleepAcc(0.f)
	, IdleAcc(0.f)
	, RandomTime(0.f)
	, Rand_SleepTime(0.f)
	, PushedAcc(0.f)
	, ReturnCheck(true)
	, DistanceValue(0.f)
	, DistanceCheck(true)
	, GetMovingSpeedCheck(true)
	, MovingSpeed(0.f)
	, YFixing(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::ALTEREGOSCRIPT);
}


CAlterEgoScript::~CAlterEgoScript()
{

}

void CAlterEgoScript::ReturnAlter()
{
	if (CPapulatusScript::AlterEgo_Distinguish)
	{
		if (m_MonsterState != PAPULATUS_FSM::NONE)
		{
			m_MonsterState = PAPULATUS_FSM::PHASE2_DIE;
		}
		if (GetGameObject()->GetActive() && ReturnCheck)
		{
			CPapulatusScript::AlterEgoSetCount();

			ReturnCheck = false;
		}
	}
}

bool CAlterEgoScript::LeftFromPlayer()
{
	vPos = Transform()->GetLocalPos();

	if (vPos.x > CPlayerScript::GetPlayerPos().x)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CAlterEgoScript::UpFromPlayer()
{
	vPos = Transform()->GetLocalPos();

	if (vPos.y < CPlayerScript::GetPlayerPos().y)
	{
		return false;
	}
	else
	{
		return true;
	}
}


void CAlterEgoScript::state()
{
	switch (m_MonsterState)
	{
	case PAPULATUS_FSM::NONE:
		break;
	case PAPULATUS_FSM::PHASE2_APPEAR_ALTER_EGO:
		Animator2D()->PlayAnim(L"Summon.img.90.frames");
		break;
	case PAPULATUS_FSM::PHASE2_STAND:
		Animator2D()->PlayAnim(L"8500022.img.stand.frames");
		break;
	case PAPULATUS_FSM::PHASE2_DIE:
		Animator2D()->PlayAnim(L"8500022.img.die1.frames");
		break;
	case PAPULATUS_FSM::PHASE2_THROW_GEAR:
		Animator2D()->PlayAnim(L"8500022.img.attack1.frames");
		break;
	case PAPULATUS_FSM::PHASE2_FLY:
		Animator2D()->PlayAnim(L"8500022.img.fly.frames");
		break;
	case PAPULATUS_FSM::PHASE2_HIT:
		Animator2D()->PlayAnim(L"8500022.img.hit1");
		break;
	case PAPULATUS_FSM::PHASE2_TRACING:
		Animator2D()->PlayAnim(L"8500022.img.fly.frames");
		break;
	case PAPULATUS_FSM::PHASE2_FADEOUT:
		Animator2D()->PlayAnim(L"FADE.OUT");
		break;
	default:
		break;
	}
}

void CAlterEgoScript::ChangeState()
{
	if (DistanceCheck)
	{
		switch (Well512Random::Instance().GetValue(1, 3))
		{
		case 1:
			DistanceValue = 200.f;
			break;
		case 2:
			DistanceValue = 250.f;
			break;
		case 3:
			DistanceValue = 300.f;
			break;
		default:
			break;
		}
		DistanceCheck = false;
	}

	if (m_MonsterState == PAPULATUS_FSM::NONE)
	{
		SectionMove();

		if (GetGameObject()->Confirm_Life() == true)
		{
			KillObject(GetGameObject());
		}
	}

	if (CPapulatusScript::HealingActive)
	{
		if (m_MonsterState == PAPULATUS_FSM::PHASE2_THROW_GEAR || m_MonsterState == PAPULATUS_FSM::PHASE2_TRACING)
		{
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				m_MonsterState = PAPULATUS_FSM::PHASE2_FADEOUT;

				return;
			}
		}
	}

	if (m_MonsterState == PAPULATUS_FSM::PHASE2_THROW_GEAR)
	{
		SetPushed(false);
	}

	if (m_MonsterState != PAPULATUS_FSM::PHASE2_THROW_GEAR && m_MonsterState != PAPULATUS_FSM::PHASE2_FADEOUT)
	{
		if (GetPushed())
		{
			if (GetHP() > 0)
			{
				m_MonsterState = PAPULATUS_FSM::PHASE2_HIT;

				vPos = Transform()->GetLocalPos();

				TerrainPos = CPapulatusScript::TerrainPosCalling;
				TerrainScale = CPapulatusScript::TerrainScaleCalling;

				if (CPlayerScript::PlayerSkillPos.x <= Transform()->GetLocalPos().x)
				{
					if (TerrainPos.x + (TerrainScale.x * 0.5f) > Transform()->GetLocalPos().x + (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
					{
						vPos.x += 40 * DT;
					}
					m_MonsterDir = DIR::LEFT;

					Transform()->SetLocalPos(vPos);
				}
				else
				{
					if (TerrainPos.x - (TerrainScale.x * 0.5f) < Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
					{
						vPos.x -= 40 * DT;
					}
					m_MonsterDir = DIR::RIGHT;

					Transform()->SetLocalPos(vPos);
				}

				PushedAcc += DT;

				if (PushedAcc <= 0.5f)
				{
					return;
				}

				SetPushed(false);
				PushedAcc = 0.f;
			}
		}
	}

	if ((GetGameObject()->Confirm_Life()) == false)
	{
		if (CPapulatusScript::AlterEgoActive)
		{
			Regeneration(GetGameObject(), true);

			if (GetGameObject()->Confirm_Life())
			{
				AddHP(GetValueHP());
				m_MonsterState = PAPULATUS_FSM::PHASE2_APPEAR_ALTER_EGO;

				return;
			}
		}
	}

	if (GetHP() <= 0)
	{
		m_MonsterState = PAPULATUS_FSM::PHASE2_DIE;

		CAlterEgoScript::AlterEgoActiveCount += 1;

		m_bAggro = false;

		if (Object()->Confirm_Life())
		{
			KillObject(GetGameObject());

			SetPushed(false);

			return;
		}
	}

	// 스크립트 들어갈 분신들 만들기

	if (m_MonsterState == PAPULATUS_FSM::PHASE2_APPEAR_ALTER_EGO)
	{
		ReturnCheck = true;

		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			m_MonsterState = PAPULATUS_FSM::PHASE2_STAND;
		}
	}

	if (m_MonsterState != PAPULATUS_FSM::PHASE2_DIE && !GetPushed() && m_MonsterState != PAPULATUS_FSM::PHASE2_APPEAR_ALTER_EGO && m_MonsterState != PAPULATUS_FSM::NONE)
	{
		if (m_bAggro == false)
		{
			m_MonsterState = PAPULATUS_FSM::PHASE2_FLY;
			SectionMove();
		}
		else
		{
			if (m_MonsterState == PAPULATUS_FSM::PHASE2_THROW_GEAR)
			{
				if (Animator2D()->GetCurAnim()->IsFinish())
				{
					//
					//Player에게 데미지 스킨

					((CScript*)((CPlayerScript::PlayerObject)->GetScript()[0]))->SetSkillType((UINT)SKILL_TYPE::PAPULATUS_THROW_GEAR);

					CPlayerScript::EffectPlayerPos = CPlayerScript::PlayerPos;
					CPlayerScript::EffectPlayerPos.y = CPlayerScript::EffectYvalue;


					if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
					{
						if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
						{
							CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
							CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

							CPlayerScript::PlayerObject->AddChild(EffectPtr);

							((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PAPULATUS_THROW_GEAR;
						}
					}


					//
					m_MonsterState = PAPULATUS_FSM::PHASE2_TRACING;
					PlayerTracing();
				}
			}
			else if (m_MonsterState == PAPULATUS_FSM::PHASE2_FADEOUT)
			{
				if (Animator2D()->GetCurAnim()->IsFinish())
				{
					KillObject(GetGameObject());
					CAlterEgoScript::AlterEgoActiveCount += 1;
				}
			}
			else
			{
				m_MonsterState = PAPULATUS_FSM::PHASE2_TRACING;
				PlayerTracing();
			}
		}
	}


	if (Animator2D()->GetCurAnim() != nullptr && GetGameObject()->Confirm_Life() == false)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			m_MonsterState = PAPULATUS_FSM::NONE;
		}
	}
	if (Animator2D()->GetCurAnim() != nullptr && GetGameObject()->Confirm_Life() == true/* && m_MonsterState != MONSTER_FSM::REGENERATION*/)
	{
		if (Animator2D()->GetCurAnim()->IsFinish()) //현재 스킬 오브젝트의 현재 프레임이 끝이거나 0 일 경우 NONE으로
		{
			m_MonsterState = PAPULATUS_FSM::PHASE2_STAND;
		}
	}
	return;
}

void CAlterEgoScript::CurDir()
{
	switch (m_MonsterDir)
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


void CAlterEgoScript::SetDir()
{
	if (Animator2D()->GetCurAnim()->IsFinish())
	{
		if (Transform()->GetLocalPos().x < CPlayerScript::GetPlayerPos().x)
		{
			m_MonsterDir = DIR::RIGHT;
		}
		else
		{
			m_MonsterDir = DIR::LEFT;
		}
	}
}


void CAlterEgoScript::SectionMove()
{
	if (GetMovingSpeedCheck)
	{
		switch (Well512Random::Instance().GetValue(0, 3))
		{
		case 0:
			MovingSpeed = 150.f;
			break;
		case 1:
			MovingSpeed = 140.f;
			break;
		case 2:
			MovingSpeed = 130.f;
			break;
		case 3:
			MovingSpeed = 120.f;
			break;
		default:
			break;
		}

		GetMovingSpeedCheck = false;
	}
	m_fSleepAcc += DT;

	if (m_fSleepAcc < RandomTime)
	{
		if (m_Left)
		{
			m_MonsterDir = DIR::LEFT;

			vPos = Transform()->GetLocalPos();
			vPos.x -= MovingSpeed * DT;

			Transform()->SetLocalPos(vPos);

			
			/*if (GetGameObject()->GetParent()->Transform()->GetLocalPos().x - (GetGameObject()->GetParent()->Transform()->GetLocalScale().x * 0.3f) >= Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
			{
				m_Left = false;
			}*/
			TerrainPos = CPapulatusScript::TerrainPosCalling;
			TerrainScale = CPapulatusScript::TerrainScaleCalling;

			if (TerrainPos.x - (TerrainScale.x * 0.3f) >= Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
			{
				m_Left = false;
			}

			return;
		}
		else if (!m_Left)
		{
			m_MonsterDir = DIR::RIGHT;

			vPos = Transform()->GetLocalPos();
			vPos.x += MovingSpeed * DT;

			Transform()->SetLocalPos(vPos);

			TerrainPos = CPapulatusScript::TerrainPosCalling;
			TerrainScale = CPapulatusScript::TerrainScaleCalling;

			if (TerrainPos.x + (TerrainScale.x * 0.3f) <= Transform()->GetLocalPos().x + (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
			{
				m_Left = true;
			}

			return;
		}
		//
		return;
	}


	if (RandomTime != 0.f)
	{
		IdleAcc += DT;


		if (IdleAcc < Rand_SleepTime)
		{
			m_MonsterState = PAPULATUS_FSM::PHASE2_STAND;

			return;
		}
		IdleAcc = 0.f;
		m_fSleepAcc = 0.f;
		Rand_SleepTime = (float)Well512Random::Instance().GetValue(1, 4);
	}

	RandomTime = (float)Well512Random::Instance().GetValue(8, 20);
}

void CAlterEgoScript::PlayerTracing()
{
	//	if (CPlayerScript::PlayerTerrain == MonsterTerrain)
	//	{
	Patrol_On = false;
	//}
		/*else if (CPlayerScript::PlayerTerrain != MonsterTerrain)
		{
			Patrol_On = true;
			if (fabsf(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->GetLocalPos().x - Transform()->GetLocalPos().x) <= 160.f)
			{
				Patrol_On = false;
			}
			if (fabsf(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->GetLocalPos().y + 142.f - Transform()->GetLocalPos().y) >= 300.f)
			{
				m_bAggro = false;
			}
		}*/

	if (LeftFromPlayer())
	{
		TerrainPos = CPapulatusScript::TerrainPosCalling;
		TerrainScale = CPapulatusScript::TerrainScaleCalling;

		if (TerrainPos.x + (TerrainScale.x * 0.3f) >= Transform()->GetLocalPos().x + (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f && !Patrol_On)
		{

			// TerrainPos.x + TerrainScale 0.5f = 현재 지형의 가장 오른쪽 끝

			// Transform().x 현재 몬스터의 위치에
			// Scale과 Collider를 곱한것은 현재 몬스터의 실질적인 가로 크기 

			//즉 현재 if문은 현재 지형의 가장 오른쪽끝 보다 가장 몬스터의 가로크기의 1/10만큼이 오른쪽 끝에 닿았을 경우.

			m_bTracing = m_bAggro;
			if (m_bTracing)
			{
				if (CPlayerScript::GetPlayerPos().x - Transform()->GetLocalPos().x >= DistanceValue)
				{
					m_MonsterDir = DIR::RIGHT;
					m_MonsterState = PAPULATUS_FSM::PHASE2_TRACING;

					vPos = Transform()->GetLocalPos();
					vPos.x += MovingSpeed * DT;

					Transform()->SetLocalPos(vPos);
				}
				else
				{
					m_bTracing = false;
				}
				SetDir();
			}
			if (!m_bTracing)
			{
				SetDir();

				static float AttDelay = 0.f;
				AttDelay += DT;

				if (AttDelay < 0.7f)
				{
					return;
				}

				AttDelay = 0.f;

				m_MonsterState = PAPULATUS_FSM::PHASE2_THROW_GEAR;
			}
		}
		else
		{
			PatrolRight();
		}
	}
	else
	{
		m_bTracing = m_bAggro;

		TerrainPos = CPapulatusScript::TerrainPosCalling;
		TerrainScale = CPapulatusScript::TerrainScaleCalling;

		if (TerrainPos.x - (TerrainScale.x * 0.3f) < Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f && !Patrol_On)
		{
			Patrol_On = false;

			// TerrainPos.x - TerrainScale 0.5f = 현재 지형의 가장 왼쪽 끝

			// Transform().x 현재 몬스터의 위치에
			// Scale과 Collider를 곱한것은 현재 몬스터의 실질적인 가로 크기 

			//즉 현재 if문은 현재 지형의 가장 오른쪽끝 보다 가장 몬스터의 가로크기의 1/10만큼이 왼쪽 끝에 닿았을 경우.


			if (m_bTracing)
			{
				if (Transform()->GetLocalPos().x - CPlayerScript::GetPlayerPos().x >= DistanceValue)
				{
					m_MonsterDir = DIR::LEFT;
					m_MonsterState = PAPULATUS_FSM::PHASE2_TRACING;

					vPos = Transform()->GetLocalPos();
					vPos.x -= MovingSpeed * DT;

					Transform()->SetLocalPos(vPos);
				}
				else
				{
					m_bTracing = false;
				}
				SetDir();
			}
			if (!m_bTracing)
			{
				SetDir();

				static float AttDelay = 0.f;
				AttDelay += DT;

				if (AttDelay < 0.7f)
				{
					return;
				}

				AttDelay = 0.f;

				m_MonsterState = PAPULATUS_FSM::PHASE2_THROW_GEAR;
			}
		}
		else
		{
			PatrolLeft();
		}
	}


	if (UpFromPlayer())
	{
		TerrainPos = CPapulatusScript::TerrainPosCalling;
		TerrainScale = CPapulatusScript::TerrainScaleCalling;
		
		m_bTracing = m_bAggro;
		if (m_bTracing)
		{
			if (Transform()->GetLocalPos().y - CPlayerScript::GetPlayerPos().x >= DistanceValue - 170.f)
			{
				m_MonsterState = PAPULATUS_FSM::PHASE2_TRACING;

				vPos = Transform()->GetLocalPos();
				vPos.y -= MovingSpeed * DT;

				Transform()->SetLocalPos(vPos);
			}
			else
			{
				m_bTracing = false;
			}
			SetDir();
		}
		if (!m_bTracing)
		{
			SetDir();

			static float AttDelay = 0.f;
			AttDelay += DT;

			if (AttDelay < 0.7f)
			{
				return;
			}

			AttDelay = 0.f;

			m_MonsterState = PAPULATUS_FSM::PHASE2_THROW_GEAR;
		}		
	}
	else
	{
		TerrainPos = CPapulatusScript::TerrainPosCalling;
		TerrainScale = CPapulatusScript::TerrainScaleCalling;
		
		m_bTracing = m_bAggro;
		if (m_bTracing)
		{
			if (CPlayerScript::GetPlayerPos().y - Transform()->GetLocalPos().y >= DistanceValue - 170.f)
			{
				m_MonsterState = PAPULATUS_FSM::PHASE2_TRACING;

				vPos = Transform()->GetLocalPos();
				vPos.y += MovingSpeed * DT;

				Transform()->SetLocalPos(vPos);

				return;
			}
			else
			{
				m_bTracing = false;
			}
			SetDir();
		}
		if (!m_bTracing)
		{
			SetDir();

			static float AttDelay = 0.f;
			AttDelay += DT;

			if (AttDelay < 0.7f)
			{
				return;
			}

			AttDelay = 0.f;

			m_MonsterState = PAPULATUS_FSM::PHASE2_THROW_GEAR;
			return;
		}
	}
}

void CAlterEgoScript::PatrolLeft()
{
	if (m_bPatrolLeft)
	{
		TerrainPos = CPapulatusScript::TerrainPosCalling;
		TerrainScale = CPapulatusScript::TerrainScaleCalling;

		if (TerrainPos.x - (TerrainScale.x * 0.5f) >= Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
		{
			m_bPatrolLeft = false;
			return;
		}

		m_MonsterDir = DIR::LEFT;

		vPos = Transform()->GetLocalPos();
		vPos.x -= MovingSpeed * DT;

		Transform()->SetLocalPos(vPos);

		return;
	}
	else
	{
		TerrainPos = CPapulatusScript::TerrainPosCalling;
		TerrainScale = CPapulatusScript::TerrainScaleCalling;

		if (TerrainPos.x - (TerrainScale.x * 0.5f) + 200.f <= Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
		{
			m_bPatrolLeft = true;
			return;
		}

		m_MonsterDir = DIR::RIGHT;

		vPos = Transform()->GetLocalPos();
		vPos.x += MovingSpeed * DT;

		Transform()->SetLocalPos(vPos);

		return;
	}
}

void CAlterEgoScript::PatrolRight()
{
	if (m_bPatrolRight)
	{
		TerrainPos = CPapulatusScript::TerrainPosCalling;
		TerrainScale = CPapulatusScript::TerrainScaleCalling;

		if (TerrainPos.x + (TerrainScale.x * 0.5f) <= Transform()->GetLocalPos().x + (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
		{
			m_bPatrolRight = false;
		}

		m_MonsterDir = DIR::RIGHT;

		vPos = Transform()->GetLocalPos();
		vPos.x += MovingSpeed * DT;

		Transform()->SetLocalPos(vPos);

		return;
	}
	else
	{
		TerrainPos = CPapulatusScript::TerrainPosCalling;
		TerrainScale = CPapulatusScript::TerrainScaleCalling;

		if (TerrainPos.x + (TerrainScale.x * 0.5f) - 200.f >= Transform()->GetLocalPos().x + (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
		{
			m_bPatrolRight = true;
		}

		m_MonsterDir = DIR::LEFT;

		vPos = Transform()->GetLocalPos();
		vPos.x -= MovingSpeed * DT;

		Transform()->SetLocalPos(vPos);

		return;
	}
}

void CAlterEgoScript::YFixingFunc()
{
	if (!CPapulatusScript::Phase0Check && !CPapulatusScript::Phase1Check && CPapulatusScript::Phase2Check && CPapulatusScript::Phase2Pass)
	{
		if (vParentPos.x == 0.f && vParentPos.y == 0.f && vParentPos.z == 0.f)
		{
			vParentPos = CPapulatusScript::PapulatusPtr->Transform()->GetLocalPos();
		}
		if (YFixing && CPapulatusScript::PapulatusPtr != nullptr)
		{
			vPos = Transform()->GetLocalPos();
			vPos.y = vParentPos.y;
			vPos.z = vParentPos.z + 100.f;
			Transform()->SetLocalPos(vPos);

			YFixing = false;
		}
	}
}



void CAlterEgoScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CAlterEgoScript::update()
{
	YFixingFunc();

	vPos = { -0.0178f, 0.0534f, 0.f };
	Collider()->SetOffsetPos(vPos);
	vPos = { 0.10f, 0.267f, 0.f };
	Collider()->SetOffsetScale(vPos);

	if (4 <= CAlterEgoScript::AlterEgoActiveCount)
	{
		CPapulatusScript::HealingActive = false;

		AlterEgoActiveCount = 0;
	}

	ChangeState();
	CurDir();

	if (m_MonsterState != Prev_MonsterState)
	{
		state();
	}
	Prev_MonsterState = m_MonsterState;

	if (m_MonsterState == PAPULATUS_FSM::NONE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		m_bAggro = true;
		GetGameObject()->SetActive(true);
	}

	return 0;
}

void CAlterEgoScript::OnCollisionEnter(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN") == CSceneMgr::GetInst()->GetCurScene()->GetLayer(_pOther->GetGameObject()->GetLayerIdx()))
		{
			float TerrainPosY = _pOther->GetGameObject()->Transform()->GetLocalPos().y;
			float TerrainScaleY = _pOther->GetGameObject()->Transform()->GetLocalScale().y;
			float MonsterPosY = GetGameObject()->Transform()->GetLocalPos().y;
			float MonsterScaleY = GetGameObject()->Transform()->GetLocalScale().y;
			float MonsterColliderScale = GetGameObject()->Collider()->GetOffsetScale().y;

			if (TerrainPosY + (TerrainScaleY * 0.5f) >= MonsterPosY - (MonsterScaleY * MonsterColliderScale * 0.5f))
			{
				TerrainPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
				TerrainScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

				Script_TERRAIN = _pOther->GetGameObject();
				MonsterTerrain = _pOther->GetGameObject();
			}
		}
	}

	if (_pOther->GetGameObject()->GetName() == L"Player")
	{
		m_bAggro = true;
	}
}

void CAlterEgoScript::OnCollision(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN") == CSceneMgr::GetInst()->GetCurScene()->GetLayer(_pOther->GetGameObject()->GetLayerIdx()))
		{
			float TerrainPosY = _pOther->GetGameObject()->Transform()->GetLocalPos().y;
			float TerrainScaleY = _pOther->GetGameObject()->Transform()->GetLocalScale().y;
			float MonsterPosY = GetGameObject()->Transform()->GetLocalPos().y;
			float MonsterScaleY = GetGameObject()->Transform()->GetLocalScale().y;
			float MonsterColliderScale = GetGameObject()->Collider()->GetOffsetScale().y;
			float MonsterOffsetY = GetGameObject()->Collider()->GetOffsetPos().y;

			float TerrainUpY = TerrainPosY + (TerrainScaleY * 0.5f);
			float MonsterUnderY = MonsterPosY - (MonsterScaleY * MonsterColliderScale * 0.5f);
			float ApplyMONSTERY = MonsterPosY - (MonsterScaleY * MonsterColliderScale * 0.5f) + (MonsterScaleY * MonsterColliderScale * MonsterOffsetY);

			if (TerrainPosY + (TerrainScaleY * 0.5f) <= MonsterPosY - (MonsterScaleY * MonsterColliderScale * 0.5f) + (MonsterScaleY * MonsterColliderScale * MonsterOffsetY) + 100)
			{
				TerrainPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
				TerrainScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

				Script_TERRAIN = _pOther->GetGameObject();
				MonsterTerrain = _pOther->GetGameObject();
			}
		}
	}
}

void CAlterEgoScript::OnCollisionExit(CCollider * _pOther)
{

}