#include "stdafx.h"
#include "PhantomwatchScript.h"

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

#include "PlayerScript.h"
#include "EffectScript.h"
#include "AnrealEngine/Well512Random.h"

const long long CPhantomwatchScript::PHANTOMWATCH_MAX_HP = 2268000;
const long long CPhantomwatchScript::PHANTOMWATCH_MAX_MP = 3000;

CPhantomwatchScript::CPhantomwatchScript()
	: CScript(CPhantomwatchScript::PHANTOMWATCH_MAX_HP, CPhantomwatchScript::PHANTOMWATCH_MAX_MP, CPhantomwatchScript::PHANTOMWATCH_MAX_HP, CPhantomwatchScript::PHANTOMWATCH_MAX_MP, (UINT)SKILL_TYPE::NONE, false, false, false, false, false, false, false, false, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0, 0)
	, m_Left(true)
	, m_bTracing(false)
	, Patrol_On(false)
	, m_bPatrolLeft(true)
	, m_bPatrolRight(true)
	, m_MonsterState(MONSTER_FSM::MOVE)
	, MonsterTerrain(nullptr)
	, m_fSleepAcc(0.f)
	, IdleAcc(0.f)
	, RandomTime(0.f)
	, Rand_SleepTime(0.f)
	, PushedAcc(0.f)
	, DistanceValue(0.f)
	, DistanceCheck(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::PHANTOMWATCHSCRIPT);
}


CPhantomwatchScript::~CPhantomwatchScript()
{

}



bool CPhantomwatchScript::LeftFromPlayer()
{
	if (vPos.x > CPlayerScript::GetPlayerPos().x)
	{
		return false;
	}
	else
	{
		return true;
	}
}


void CPhantomwatchScript::state()
{
	CGameObject* tempObj = GetGameObject();

	switch (m_MonsterState)
	{
	case MONSTER_FSM::NONE:
		//	Animator2D()->PlayAnim(L"blink");
		break;

	case MONSTER_FSM::ATTACK:
		Animator2D()->PlayAnim(L"8143000.img.attack1.frames");
		break;

	case MONSTER_FSM::ATTACK2:
		break;

	case MONSTER_FSM::SKILL:
		break;

	case MONSTER_FSM::SKILL2:
		break;

	case MONSTER_FSM::HIT:
		Animator2D()->PlayAnim(L"8143000.img.hit1");
		break;

	case MONSTER_FSM::DIE:
		Animator2D()->PlayAnim(L"8143000.img.die1.frames");
		break;

	case MONSTER_FSM::MOVE:
		Animator2D()->PlayAnim(L"8143000.img.move.frames");
		break;

	case MONSTER_FSM::STAND:
		Animator2D()->PlayAnim(L"8143000.img.stand.frames");
		break;

	case MONSTER_FSM::TRACING:
		Animator2D()->PlayAnim(L"8143000.img.move.frames");
		break;

	case MONSTER_FSM::REGENERATION:
		Animator2D()->PlayAnim(L"8143000.img.Regeneration.frames");
		break;

	case MONSTER_FSM::END:
		break;

	default:
		break;
	}
}

void CPhantomwatchScript::ChangeState()
{
	if (DistanceCheck)
	{
		switch (Well512Random::Instance().GetValue(1, 3))
		{
		case 1:
			DistanceValue = 240.f;
			break;
		case 2:
			DistanceValue = 200.f;
			break;
		case 3:
			DistanceValue = 220.f;
			break;
		default:
			break;
		}
		DistanceCheck = false;
	}

	if (m_MonsterState == MONSTER_FSM::ATTACK)
	{
		SetPushed(false);
	}

	if (m_MonsterState != MONSTER_FSM::ATTACK)
	{
		if (GetPushed())
		{
			if (GetHP() > 0)
			{
				m_MonsterState = MONSTER_FSM::HIT;

				vPos = Transform()->GetLocalPos();

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


	if (m_MonsterState == MONSTER_FSM::NONE)
	{
		SectionMove();
	}


	if ((GetGameObject()->Confirm_Life()) == false)
	{
		Regeneration(GetGameObject());

		if (GetGameObject()->Confirm_Life())
		{
			AddHP(GetValueHP());
			m_MonsterState = MONSTER_FSM::REGENERATION;

			return;
		}
	}

	if (GetHP() <= 0)
	{
		m_MonsterState = MONSTER_FSM::DIE;

		if (Object()->Confirm_Life())
		{
			KillObject(GetGameObject());

			SetPushed(false);

			return;
		}
	}


	if (m_MonsterState == MONSTER_FSM::REGENERATION)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			m_MonsterState = MONSTER_FSM::STAND;
		}
		else
		{
			return;
		}
	}

	if (m_MonsterState != MONSTER_FSM::DIE && !GetPushed())
	{
		if (m_bAggro == false)
		{
			m_MonsterState = MONSTER_FSM::MOVE;
			SectionMove();
		}
		else
		{
			if (m_MonsterState == MONSTER_FSM::ATTACK)
			{
				if (Animator2D()->GetCurAnim()->IsFinish())
				{
					//
					//Player에게 데미지 스킨

					((CScript*)((CPlayerScript::PlayerObject)->GetScript()[0]))->SetSkillType((UINT)SKILL_TYPE::PHANTOMWATCH_ATTACK);

					CPlayerScript::EffectPlayerPos = CPlayerScript::PlayerPos;
					CPlayerScript::EffectPlayerPos.y = CPlayerScript::EffectYvalue;


					if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
					{
						if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
						{
							CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
							CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

							CPlayerScript::PlayerObject->AddChild(EffectPtr);
							((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PHANTOMWATCH_ATTACK;

						}
					}


					//
					m_MonsterState = MONSTER_FSM::TRACING;
					PlayerTracing();
				}
			}
			else
			{
				m_MonsterState = MONSTER_FSM::TRACING;
				PlayerTracing();
			}
		}
	}


	if (Animator2D()->GetCurAnim() != nullptr && GetGameObject()->Confirm_Life() == false)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			m_MonsterState = MONSTER_FSM::NONE;
		}
	}
	if (Animator2D()->GetCurAnim() != nullptr && GetGameObject()->Confirm_Life() == true/* && m_MonsterState != MONSTER_FSM::REGENERATION*/)
	{
		if (Animator2D()->GetCurAnim()->IsFinish()) //현재 스킬 오브젝트의 현재 프레임이 끝이거나 0 일 경우 NONE으로
		{
			m_MonsterState = MONSTER_FSM::STAND;
		}
	}
	return;
}

void CPhantomwatchScript::CurDir()
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


void CPhantomwatchScript::SetDir()
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


void CPhantomwatchScript::SectionMove()
{
	m_fSleepAcc += DT;

	if (m_fSleepAcc < RandomTime)
	{
		if (m_Left)
		{
			m_MonsterDir = DIR::LEFT;

			vPos = Transform()->GetLocalPos();
			vPos.x -= 150.f * DT;

			Transform()->SetLocalPos(vPos);

			if (TerrainPos.x - (TerrainScale.x * 0.5f) >= Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
			{
				m_Left = false;
			}

			return;
		}
		else if (!m_Left)
		{
			m_MonsterDir = DIR::RIGHT;

			vPos = Transform()->GetLocalPos();
			vPos.x += 150.f * DT;

			Transform()->SetLocalPos(vPos);

			if (TerrainPos.x + (TerrainScale.x * 0.5f) <= Transform()->GetLocalPos().x + (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
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
			m_MonsterState = MONSTER_FSM::STAND;

			return;
		}
		IdleAcc = 0.f;
		m_fSleepAcc = 0.f;
		Rand_SleepTime = (float)Well512Random::Instance().GetValue(1, 4);
	}

	RandomTime = (float)Well512Random::Instance().GetValue(8, 20);
}

void CPhantomwatchScript::PlayerTracing()
{
	if (CPlayerScript::PlayerTerrain == MonsterTerrain)
	{
		Patrol_On = false;
	}
	else if (CPlayerScript::PlayerTerrain != MonsterTerrain)
	{
		Patrol_On = true;
		if (fabsf(CPlayerScript::GetPlayerPos().x - Transform()->GetLocalPos().x) <= 160.f)
		{
			Patrol_On = false;
		}
		if (fabsf(CPlayerScript::GetPlayerPos().y + 142.f - Transform()->GetLocalPos().y) >= 300.f)
		{
			m_bAggro = false;
		}
	}

	if (LeftFromPlayer())
	{
		if (TerrainPos.x + (TerrainScale.x * 0.5f) >= Transform()->GetLocalPos().x + (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f && !Patrol_On)
		{

			// TerrainPos.x + TerrainScale 0.5f = 현재 지형의 가장 오른쪽 끝

			// Transform().x 현재 몬스터의 위치에
			// Scale과 Collider를 곱한것은 현재 몬스터의 실질적인 가로 크기 

			//즉 현재 if문은 현재 지형의 가장 오른쪽끝 보다 가장 몬스터의 가로크기의 1/10만큼이 오른쪽 끝에 닿았을 경우.

			m_bTracing = m_bAggro;
			if (m_bTracing)
			{
				float PlayerPos = CPlayerScript::GetPlayerPos().x;
				float MonsterPos = Transform()->GetLocalPos().x;
				if (CPlayerScript::GetPlayerPos().x - Transform()->GetLocalPos().x >= DistanceValue)
				{
					m_MonsterDir = DIR::RIGHT;
					m_MonsterState = MONSTER_FSM::TRACING;

					vPos = Transform()->GetLocalPos();
					vPos.x += 150.f * DT;

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

				if (((CPlayerScript::GetPlayerPos().y) + 142.f) - (Transform()->GetLocalPos().y) <= 200.f && (Transform()->GetLocalPos().y) - ((CPlayerScript::GetPlayerPos().y) + 142.f) <= 200.f)
				{
					static float AttDelay = 0.f;
					AttDelay += DT;

					if (AttDelay < 0.7f)
					{
						return;
					}

					AttDelay = 0.f;

					m_MonsterState = MONSTER_FSM::ATTACK;
				}
				return;

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

		if (TerrainPos.x - (TerrainScale.x * 0.5f) <= Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f && !Patrol_On)
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
					m_MonsterState = MONSTER_FSM::TRACING;

					vPos = Transform()->GetLocalPos();
					vPos.x -= 150.f * DT;

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

				if (((CPlayerScript::GetPlayerPos().y) + 142.f) - (Transform()->GetLocalPos().y) <= 200.f && (Transform()->GetLocalPos().y) - ((CPlayerScript::GetPlayerPos().y) + 142.f) <= 200.f)
				{
					static float AttDelay = 0.f;
					AttDelay += DT;

					if (AttDelay < 0.7f)
					{
						return;
					}

					AttDelay = 0.f;

					m_MonsterState = MONSTER_FSM::ATTACK;
				}
				return;
			}
		}
		else
		{
			PatrolLeft();
		}
	}
}

void CPhantomwatchScript::PatrolLeft()
{
	if (m_bPatrolLeft)
	{
		if (TerrainPos.x - (TerrainScale.x * 0.5f) >= Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
		{
			m_bPatrolLeft = false;
			return;
		}

		m_MonsterDir = DIR::LEFT;

		vPos = Transform()->GetLocalPos();
		vPos.x -= 150.f * DT;

		Transform()->SetLocalPos(vPos);

		return;
	}
	else
	{
		if (TerrainPos.x - (TerrainScale.x * 0.5f) + 200.f <= Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
		{
			m_bPatrolLeft = true;
			return;
		}

		m_MonsterDir = DIR::RIGHT;

		vPos = Transform()->GetLocalPos();
		vPos.x += 150.f * DT;

		Transform()->SetLocalPos(vPos);

		return;
	}
}

void CPhantomwatchScript::PatrolRight()
{
	if (m_bPatrolRight)
	{
		if (TerrainPos.x + (TerrainScale.x * 0.5f) <= Transform()->GetLocalPos().x + (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
		{
			m_bPatrolRight = false;
		}

		m_MonsterDir = DIR::RIGHT;

		vPos = Transform()->GetLocalPos();
		vPos.x += 150.f * DT;

		Transform()->SetLocalPos(vPos);

		return;
	}
	else
	{
		if (TerrainPos.x + (TerrainScale.x * 0.5f) - 200.f >= Transform()->GetLocalPos().x + (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
		{
			m_bPatrolRight = true;
		}

		m_MonsterDir = DIR::LEFT;

		vPos = Transform()->GetLocalPos();
		vPos.x -= 150.f * DT;

		Transform()->SetLocalPos(vPos);

		return;
	}
}



void CPhantomwatchScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CPhantomwatchScript::update()
{
	ChangeState();
	CurDir();

	if (m_MonsterState != Prev_MonsterState)
	{
		state();
	}
	Prev_MonsterState = m_MonsterState;

	if (m_MonsterState == MONSTER_FSM::NONE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}

void CPhantomwatchScript::OnCollisionEnter(CCollider * _pOther)
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

	if (_pOther->GetGameObject()->GetName() == L"Player")
	{
		m_bAggro = true;
	}
}

void CPhantomwatchScript::OnCollision(CCollider * _pOther)
{
	if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN") == CSceneMgr::GetInst()->GetCurScene()->GetLayer(_pOther->GetGameObject()->GetLayerIdx()))
	{
		float TerrainPosY = _pOther->GetGameObject()->Transform()->GetLocalPos().y;
		float TerrainScaleY = _pOther->GetGameObject()->Transform()->GetLocalScale().y;
		float MonsterPosY = GetGameObject()->Transform()->GetLocalPos().y;
		float MonsterScaleY = GetGameObject()->Transform()->GetLocalScale().y;
		float MonsterColliderScale = GetGameObject()->Collider()->GetOffsetScale().y;

		if (TerrainPosY + (TerrainScaleY * 0.5f) <= MonsterPosY - (MonsterScaleY * MonsterColliderScale * 0.5f))
		{
			TerrainPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
			TerrainScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

			Script_TERRAIN = _pOther->GetGameObject();
			MonsterTerrain = _pOther->GetGameObject();
		}
	}
}

void CPhantomwatchScript::OnCollisionExit(CCollider * _pOther)
{

}