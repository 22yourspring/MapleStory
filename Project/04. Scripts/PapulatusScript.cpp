#include "stdafx.h"
#include "PapulatusScript.h"
#include "MobGravityScript.h"
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
#include "AnrealEngine/Well512Random.h"

#include "AlterEgoScript.h"
#include "InstantDeathAreaScript.h"
#include "LaserScript.h"

#include "BeShotEffectScript.h"
#include "EffectScript.h"
#include "ActiveExplosionScript.h"


PAPULATUS_PHASE1_PATTERN CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::NONE;
PAPULATUS_PHASE2_PATTERN CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::NONE;

float CPapulatusScript::SleepHealCount = 0.f;

bool CPapulatusScript::CrackOfCheck = false;
bool CPapulatusScript::LaserOffAction = false;
bool CPapulatusScript::PapulatusFallen = false;

CGameObject* CPapulatusScript::PapulatusPtr = nullptr;

int CPapulatusScript::AlterEgo_SurviveCount = 0;

bool CPapulatusScript::AlterEgo_Distinguish = false;
bool CPapulatusScript::AlterEgoActive = false;
bool CPapulatusScript::Prev_AlterActive = false;

bool CPapulatusScript::HealingActive = false;

bool CPapulatusScript::AlterEgoReady = false;

float CPapulatusScript::AccAlterEgo = 0.f;

int CPapulatusScript::RandomSkill = 0;
bool CPapulatusScript::CurseCheck = false;
bool CPapulatusScript::AlramingCheck = false;
float CPapulatusScript::CurAcctime = 0.f;

float CPapulatusScript::AccTorrent = 0.f;

bool CPapulatusScript::TimeBombCheck = false;
bool CPapulatusScript::TorrentCheck = true;

const long long CPapulatusScript::Phase0HP = 1000;
//const long long CPapulatusScript::Phase1HP = 378000000000;
const long long CPapulatusScript::Phase1HP = 178000000000;
//const long long CPapulatusScript::Phase1HP = 178000;
//const long long CPapulatusScript::Phase2HP = 126000000000;
const long long CPapulatusScript::Phase2HP = 16000000000;

const long long CPapulatusScript::PAPULATUS_MAX_MP = 2000000;

bool CPapulatusScript::Phase0Pass = true;
bool CPapulatusScript::Phase1Pass = true;
bool CPapulatusScript::Phase2Pass = true;

bool CPapulatusScript::Phase0Check = false;
bool CPapulatusScript::Phase1Check = false;
bool CPapulatusScript::Phase2Check = false;

Vec3 CPapulatusScript::TerrainPosCalling = { 0.f, 0.f, 0.f, };
Vec3 CPapulatusScript::TerrainScaleCalling = { 0.f, 0.f, 0.f, };

bool CPapulatusScript::Phase2ReverseGravity = false;

CPapulatusScript* CPapulatusScript::PapulatusScriptPtr = nullptr;

bool CPapulatusScript::Phase1Start = false;

bool CPapulatusScript::TimeBombsOn = false;

CPapulatusScript::CPapulatusScript()
	: CScript(CPapulatusScript::Phase0HP, CPapulatusScript::PAPULATUS_MAX_MP, CPapulatusScript::Phase0HP, CPapulatusScript::PAPULATUS_MAX_MP, (UINT)SKILL_TYPE::NONE, false, false, false, false, false, false, false, false, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0.f, 0, 0, 0)
	, m_Left(true)
	, m_bTracing(false)
	, Patrol_On(false)
	, m_bPatrolLeft(true)
	, m_bPatrolRight(true)
	, SleepState(PAPULATUS_PHASE1_PATTERN::NONE)
	, Papulatus_State(PAPULATUS_FSM::NONE)
	, MonsterTerrain(nullptr)
	, m_fSleepAcc(0.f)
	, IdleAcc(0.f)
	, RandomTime(0.f)
	, Rand_SleepTime(0.f)
	, PushedAcc(0.f)
	, TempSkill(0)
	, GravityPtr(nullptr)
	, DeadLockProcess_CrackLaser(true)
	, AlramDeadLockProcessor(false)
	, SleepDeadLockProcessor(false)
	, DoSleepHeal(true)
	, YSort(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::PAPULATUSSCRIPT);
}


CPapulatusScript::~CPapulatusScript()
{

}


void CPapulatusScript::DoCount()
{
	if (!DoSleepHeal)
	{
		SleepHealCount += 0.001f * DT;

		if (SleepHealCount <= 0.1f)
		{
			return;
		}
		SleepHealCount = 0.f;
		DoSleepHeal = true;
	}
}

void CPapulatusScript::TimeBombsCount()
{
	if (TimeBombsOn)
	{
		static float TIMEacc = 0.f;

		TIMEacc += (float)DT;

		if (TIMEacc <= 1.2f)
		{
			return;
		}
		TIMEacc = 0.f;
		TimeBombsOn = false;
	}
}

bool CPapulatusScript::LeftFromPlayer()
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

bool CPapulatusScript::UpFromPlayer()
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

DIR CPapulatusScript::PapulatusDir()
{
	if (PapulatusScriptPtr != nullptr)
	{
		return PapulatusScriptPtr->m_MonsterDir;
	}
	else
	{
		if (PapulatusPtr == nullptr)
		{
			for (size_t i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec().size(); ++i)
			{
				if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec()[i]->GetName() == L"Papulatus")
				{
					PapulatusPtr = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec()[i];
				}
			}

			for (size_t i = 0; i < PapulatusPtr->GetScript().size(); ++i)
			{
				if (PapulatusPtr->GetScript()[i]->GetScriptType() == (UINT)SCRIPT_TYPE::PAPULATUSSCRIPT)
				{
					PapulatusScriptPtr = ((CPapulatusScript*)(PapulatusPtr->GetScript()[i]));

					return PapulatusScriptPtr->m_MonsterDir;
				}
			}

		}
		else
		{
			for (size_t i = 0; i < PapulatusPtr->GetScript().size(); ++i)
			{
				if (PapulatusPtr->GetScript()[i]->GetScriptType() == (UINT)SCRIPT_TYPE::PAPULATUSSCRIPT)
				{
					PapulatusScriptPtr = ((CPapulatusScript*)(PapulatusPtr->GetScript()[i]));

					return PapulatusScriptPtr->m_MonsterDir;
				}
			}
		}
		return PapulatusScriptPtr->m_MonsterDir;
	}
}


void CPapulatusScript::state()
{
	switch (Papulatus_State)
	{
	case PAPULATUS_FSM::NONE:
		break;
	case PAPULATUS_FSM::PHASE0_STAND:
		Animator2D()->PlayAnim(L"8500000.img.stand.frames");
		break;
	case PAPULATUS_FSM::PHASE0_DIE:
		Animator2D()->PlayAnim(L"8500000.img.die1.frames");
		break;
	case PAPULATUS_FSM::PHASE_SEPARATOR1:
		break;
	case PAPULATUS_FSM::PHASE1_BLUERING:
		Animator2D()->PlayAnim(L"2600631.img.skill1.frames");
		break;
	case PAPULATUS_FSM::PHASE1_CURSE:
		Animator2D()->PlayAnim(L"8500021.img.skill1.frames");
		break;
	case PAPULATUS_FSM::PHASE1_REDRING:
		Animator2D()->PlayAnim(L"2600631.img.skill3.frames");
		break;
	case PAPULATUS_FSM::PHASE1_TIME_BOMB:
		Animator2D()->PlayAnim(L"8500021.img.attack1.frames");
		break;
	case PAPULATUS_FSM::PHASE1_CRASH:
		Animator2D()->PlayAnim(L"8500021.img.attack2.frames");
		break;
	case PAPULATUS_FSM::PHASE1_DIE:
		Animator2D()->PlayAnim(L"8500021.img.die1.frames");
		break;
	case PAPULATUS_FSM::PHASE1_HIT:
		Animator2D()->PlayAnim(L"8500021.img.hit1");
		break;
	case PAPULATUS_FSM::PHASE1_MOVE:
		Animator2D()->PlayAnim(L"8500021.img.move.frames");
		break;
	case PAPULATUS_FSM::PHASE1_STAND:
		Animator2D()->PlayAnim(L"8500021.img.stand.frames");
		break;
	case PAPULATUS_FSM::PHASE1_TRACING:
		Animator2D()->PlayAnim(L"8500021.img.move.frames");
		break;
	case PAPULATUS_FSM::PHASE1_TOSLEEP:
		Animator2D()->PlayAnim(L"8500021.img.skill2tosleep");
		break;
	case PAPULATUS_FSM::PHASE1_SLEEPING:
		Animator2D()->PlayAnim(L"8500021.img.sleep.frames");
		break;
	case PAPULATUS_FSM::PHASE1_WAKE_UP:
		Animator2D()->PlayAnim(L"8500021.img.wakeup.frames");
		break;
	case PAPULATUS_FSM::PHASE1_ALARM_START:
		Animator2D()->PlayAnim(L"8500021.img.skill3.frames");
		break;
	case PAPULATUS_FSM::PHASE1_ALARM_ING:
		Animator2D()->PlayAnim(L"skill3_4ing");
		break;
	case PAPULATUS_FSM::PHASE1_ALARM_END:
		Animator2D()->PlayAnim(L"8500021.img.skill4.frames");
		break;
	case PAPULATUS_FSM::PHASE1_CRACK_OF_TIME:
		Animator2D()->PlayAnim(L"8500021.img.skill5.frames");
		break;
	case PAPULATUS_FSM::PHASE_SEPARATOR2:
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
	case PAPULATUS_FSM::PHASE2_SUMMON_ALTER_EGO:
		Animator2D()->PlayAnim(L"8500022.img.skill1.frames");
		break;
	case PAPULATUS_FSM::PHASE2_HEALLING:
		Animator2D()->PlayAnim(L"8500022.img.skill2ing.frames");
		break;
	case PAPULATUS_FSM::PHASE2_BLINK:
		Animator2D()->PlayAnim(L"8500022.img.skill3.frames");
		break;
	case PAPULATUS_FSM::PHASE2_TORRENT_OF_TIME:
		Animator2D()->PlayAnim(L"8500022.img.skill4.frames");
		break;
	case PAPULATUS_FSM::PHASE2_ALARM_START:
		Animator2D()->PlayAnim(L"8500022.img.skill5.frames");
		break;
	case PAPULATUS_FSM::PHASE2_ALARMING:
		Animator2D()->PlayAnim(L"SKILL5_6ing");
		break;
	case PAPULATUS_FSM::PHASE2_ALARM_END:
		Animator2D()->PlayAnim(L"8500022.img.skill6.frames");
		break;
	case PAPULATUS_FSM::END:
		break;
	default:
		break;
	}
}

void CPapulatusScript::ChangeState()
{
	PapualtusHP = GetHP();
	if (PapualtusHP > Phase1HP && Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR1 && Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
	{
		PapualtusHP = GetHP();
		SetHP(PrevPapualtusHP);
	}
	else if (PapualtusHP > Phase2HP && Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
	{
		PapualtusHP = GetHP();
		SetHP(PrevPapualtusHP);
	}
	if (Phase0Check && !Phase1Check && !Phase2Check && Phase0Pass)
	{
		vPos = { -0.0870f, -0.11f, 0.f };
		Collider()->SetOffsetPos(vPos);
		vPos = { 0.20f, 0.20f, 0.f };
		Collider()->SetOffsetScale(vPos);

		if (GravityPtr == nullptr)
		{
			for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec().size(); ++i)
			{
				if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec()[i]->GetName() == L"Papulatus")
				{
					for (size_t j = 0; j < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec()[i]->GetScript().size(); ++j)
					{
						if ((SCRIPT_TYPE)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec()[i]->GetScript()[j]->GetScriptType()) == SCRIPT_TYPE::MOBGRAVITYSCRIPT)
						{
							GravityPtr = (CMobGravityScript*)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec()[i]->GetScript()[j]);
						}
					}
				}
			}
		}

		if (GravityPtr != nullptr)
		{
			GravityPtr->SetGravity(false);
		}


		Phase0Pass = false;
	}
	else if (!Phase0Check && Phase1Check && !Phase2Check && Phase1Pass)
	{
		vPos = { -0.0170f, -0.080f, 0.f };
		Collider()->SetOffsetPos(vPos);
		vPos = { 0.170f, 0.260f, 0.f };
		Collider()->SetOffsetScale(vPos);

		if (GravityPtr != nullptr)
		{
			GravityPtr->SetGravity(true);
		}

		Phase1Pass = false;

		Phase1Start = true;
	}
	else if (!Phase0Check && !Phase1Check && Phase2Check && Phase2Pass)
	{
		vPos = { -0.0178f, 0.0534f, 0.f };
		Collider()->SetOffsetPos(vPos);
		vPos = { 0.10f, 0.267f, 0.f };
		Collider()->SetOffsetScale(vPos);

		if (GravityPtr != nullptr)
		{
			GravityPtr->SetGravity(true);
		}

		Phase2Pass = false;
	}

	if (Papulatus_State != PAPULATUS_FSM::PHASE1_TRACING && Papulatus_State != PAPULATUS_FSM::PHASE1_MOVE && Papulatus_State != PAPULATUS_FSM::PHASE2_FLY && Papulatus_State != PAPULATUS_FSM::PHASE2_TRACING && Papulatus_State != PAPULATUS_FSM::PHASE2_HIT && Papulatus_State != PAPULATUS_FSM::PHASE1_HIT)   //원래는 ATTACK 이어야 false == 잡몹들은 공격할 동안 넉백당하지 않음. 즉 넉백 당하지 않는 상황을 적어야 함.
	{
		SetPushed(false);
	}

	else if (Papulatus_State == PAPULATUS_FSM::PHASE2_HIT || Papulatus_State == PAPULATUS_FSM::PHASE1_HIT || Papulatus_State == PAPULATUS_FSM::PHASE1_TRACING || Papulatus_State == PAPULATUS_FSM::PHASE1_MOVE || Papulatus_State == PAPULATUS_FSM::PHASE2_FLY || Papulatus_State == PAPULATUS_FSM::PHASE2_TRACING)			// ORIGINAL . ATTACK이 아니라면 (false해야 하는 상황 즉 넉백되지 않는 상황이 아니라면  =  넉백이 되는 상황이라면)
	{
		if (GetPushed())
		{
			if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
			{
				if (GetHP() <= (CPapulatusScript::Phase1HP) / 10)
				{
					Papulatus_State = PAPULATUS_FSM::PHASE1_HIT;
					SetMove();
				}
				else
				{
					SetPushed(false);
				}
			}
			else
			{
				if (GetHP() <= (CPapulatusScript::Phase2HP) / 10)
				{
					Papulatus_State = PAPULATUS_FSM::PHASE2_HIT;
					SetMove();
				}
				else
				{
					SetPushed(false);
				}
			}
		}
	}

	if (Papulatus_State == PAPULATUS_FSM::NONE)
	{
		if ((GetGameObject()->Confirm_Life()))
		{
			Papulatus_State = PAPULATUS_FSM::PHASE0_STAND;
			Phase0Check = true;
			return;
		}
	}


	if ((GetGameObject()->Confirm_Life()) == false)
	{
		//	Regeneration(GetGameObject());

		if (GetGameObject()->Confirm_Life())
		{
			SetHP(GetValueHP());
			//	Papulatus_State = PAPULATUS_FSM::NONE;

			return;
		}
	}

	if (GetHP() <= 0)
	{
		if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR1)
		{
			Papulatus_State = PAPULATUS_FSM::PHASE0_DIE;

			for (size_t i = 0; i < GetGameObject()->GetScript().size(); ++i)
			{
				if (GetGameObject()->GetScript()[i]->GetScriptType() != (UINT)SCRIPT_TYPE::MOBGRAVITYSCRIPT)
				{
					GetGameObject()->GetScript()[i]->SetValueHP(CPapulatusScript::Phase1HP);
				}
			}

			if (Object()->Confirm_Life())
			{
				SetHP(Phase1HP);
				return;
			}
		}
		else if (PAPULATUS_FSM::PHASE_SEPARATOR1 < Papulatus_State && Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
		{
			Papulatus_State = PAPULATUS_FSM::PHASE1_DIE;

			CPapulatusScript::Phase2ReverseGravity = true;

			for (size_t i = 0; i < GetGameObject()->GetScript().size(); ++i)
			{
				if (GetGameObject()->GetScript()[i]->GetScriptType() != (UINT)SCRIPT_TYPE::MOBGRAVITYSCRIPT)
				{
					GetGameObject()->GetScript()[i]->SetValueHP(CPapulatusScript::Phase2HP);
				}
			}

			if (Object()->Confirm_Life())
			{
				SetHP(Phase2HP);
				return;
			}
		}
		else if (Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
		{
			Papulatus_State = PAPULATUS_FSM::PHASE2_DIE;

			if (Object()->Confirm_Life())
			{
				KillObject(GetGameObject());

				SetPushed(false);

				return;
			}
		}
	}


	/*if (Papulatus_State == PAPULATUS_FSM::REGENERATION)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			Papulatus_State = PAPULATUS_FSM::STAND;
		}
		else
		{
			return;
		}
	}*/


	// 타임밤 //주석
	// 저주 // 주석
	// 시간의 틈 // 주석
	// 시간의 격류 //주석
	// 위치이동 // 주석

	if (Papulatus_State != PAPULATUS_FSM::PHASE0_DIE && Papulatus_State != PAPULATUS_FSM::PHASE1_DIE && Papulatus_State != PAPULATUS_FSM::PHASE2_DIE && !GetPushed() && Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR1)
	{
		if (m_bAggro == false)
		{
			/*Papulatus_State = PAPULATUS_FSM::MOVE;
			SectionMove();*/
			m_bAggro = true;
		}

		if (Papulatus_State == PAPULATUS_FSM::PHASE1_BLUERING)
		{
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				//
				//Player에게 데미지 스킨

				((CScript*)((CPlayerScript::PlayerObject)->GetScript()[0]))->SetSkillType((UINT)SKILL_TYPE::PAPULATUS_BLUERING);

				CPlayerScript::EffectPlayerPos = CPlayerScript::GetPlayerPos();
				CPlayerScript::EffectPlayerPos.y = CPlayerScript::EffectYvalue;


				if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
				{
					if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
					{
						CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
						CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

						CPlayerScript::PlayerObject->AddChild(EffectPtr);
						((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PAPULATUS_BLUERING;

					}
				}


				//
				Papulatus_State = PAPULATUS_FSM::PHASE1_TRACING;
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE1_REDRING)
		{
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				//
				//Player에게 데미지 스킨

				((CScript*)((CPlayerScript::PlayerObject)->GetScript()[0]))->SetSkillType((UINT)SKILL_TYPE::PAPULATUS_REDRING);

				CPlayerScript::EffectPlayerPos = CPlayerScript::GetPlayerPos();
				CPlayerScript::EffectPlayerPos.y = CPlayerScript::EffectYvalue;


				if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
				{
					if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
					{
						CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
						CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

						CPlayerScript::PlayerObject->AddChild(EffectPtr);
						((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PAPULATUS_REDRING;

					}
				}


				//
				Papulatus_State = PAPULATUS_FSM::PHASE1_TRACING;
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE1_CURSE)
		{

		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE1_TIME_BOMB)
		{
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				//
				//Player에게 데미지 스킨
				// 폭탄 오브젝트 활성화로 바꾸고 오브젝트 폭발시 해당 코드삽입으로 수정해야함.
				
				/*((CScript*)((CSceneMgr::GetInst()->GetLayer(L"PLAYER")->GetObjectVec()[0])->GetScript()[0]))->SetSkillType((UINT)SKILL_TYPE::PAPULATUS_TIME_BOMB);

				CPlayerScript::EffectPlayerPos = CPlayerScript::GetPlayerPos();
				CPlayerScript::EffectPlayerPos.y = CPlayerScript::EffectYvalue;


				if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
				{
					if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
					{
						CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
						CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

						CSceneMgr::GetInst()->GetLayer(L"PLAYER")->GetObjectVec()[0]->AddChild(EffectPtr);
					}
				}*/

				CPapulatusScript::TimeBombsOn = true;
				//
				Papulatus_State = PAPULATUS_FSM::PHASE1_TRACING;
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE1_CRASH)
		{
			if (Animator2D()->GetCurAnim()->GetCurFrm() == 16)
			{
				//
				//Player에게 데미지 스킨

				((CScript*)((CPlayerScript::PlayerObject)->GetScript()[0]))->SetSkillType((UINT)SKILL_TYPE::PAPULATUS_CRASH);

				CPlayerScript::EffectPlayerPos = CPlayerScript::GetPlayerPos();
				CPlayerScript::EffectPlayerPos.y = CPlayerScript::EffectYvalue;


				if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
				{
					if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
					{
						CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
						CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

						CPlayerScript::PlayerObject->AddChild(EffectPtr);
						((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PAPULATUS_CRASH;

					}
				}


				//
				Papulatus_State = PAPULATUS_FSM::PHASE1_TRACING;
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE1_CRACK_OF_TIME)
		{
			if (Animator2D()->GetCurAnim()->GetCurFrm() == 20)
			{
				CrackOfCheck = true;
				//
				Papulatus_State = PAPULATUS_FSM::PHASE1_TRACING;
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE2_THROW_GEAR)
		{
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				//
				//Player에게 데미지 스킨

				((CScript*)((CPlayerScript::PlayerObject)->GetScript()[0]))->SetSkillType((UINT)SKILL_TYPE::PAPULATUS_THROW_GEAR);

				CPlayerScript::EffectPlayerPos = CPlayerScript::GetPlayerPos();
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
				Papulatus_State = PAPULATUS_FSM::PHASE2_TRACING;
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE2_BLINK)
		{
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				//
				//Player에게 데미지 스킨
				((CScript*)((CPlayerScript::PlayerObject)->GetScript()[0]))->SetSkillType((UINT)SKILL_TYPE::PAPULATUS_BLINK);

				CPlayerScript::EffectPlayerPos = CPlayerScript::GetPlayerPos();
				CPlayerScript::EffectPlayerPos.y = CPlayerScript::EffectYvalue;


				if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetName() == L"EFFECT")
				{
					if (CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild().size() != 0)
					{
						CGameObject* EffectPtr = CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0];
						CSceneMgr::GetInst()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->GetChild()[0]->DisconnectParent();

						CPlayerScript::PlayerObject->AddChild(EffectPtr);
						((CEffectScript*)EffectPtr->GetScript()[0])->EFFECT_SkillState = SKILL_TYPE::PAPULATUS_BLINK;

					}
				}

				Papulatus_State = PAPULATUS_FSM::PHASE2_TRACING;
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE2_HEALLING)
		{
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				HealingActive = true;

				Papulatus_State = PAPULATUS_FSM::PHASE2_TRACING;
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE2_SUMMON_ALTER_EGO)
		{
			if (AlterEgoActive)
			{
				AlterEgoActive = false;
			}
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				AlterEgoActive = true;
				Prev_AlterActive = true;

				HealingActive = false;

				Papulatus_State = PAPULATUS_FSM::PHASE2_TRACING;
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE2_TORRENT_OF_TIME)
		{
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				Papulatus_State = PAPULATUS_FSM::PHASE2_TRACING;
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_START || Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_ING || Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_END)
		{
			if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_ING)
			{
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE1_TOSLEEP || Papulatus_State == PAPULATUS_FSM::PHASE1_SLEEPING || Papulatus_State == PAPULATUS_FSM::PHASE1_WAKE_UP)
		{
			if (Papulatus_State == PAPULATUS_FSM::PHASE1_SLEEPING)
			{
				PlayerTracing();
			}
		}
		else if (Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_START || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARMING || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_END)
		{
			if (Papulatus_State == PAPULATUS_FSM::PHASE2_ALARMING)
			{
				PlayerTracing();
			}
		}
		else
		{
			if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_TRACING;
			}
			else if (Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
			{
				Papulatus_State = PAPULATUS_FSM::PHASE2_TRACING;
			}
			PlayerTracing();
		}

	}


	if (Animator2D()->GetCurAnim() != nullptr && GetGameObject()->Confirm_Life() == false)
	{
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			Papulatus_State = PAPULATUS_FSM::NONE;
		}
	}

	if (Animator2D()->GetCurAnim() != nullptr && GetGameObject()->Confirm_Life() == true /* && m_MonsterState != MONSTER_FSM::REGENERATION*/)
	{
		if (Animator2D()->GetCurAnim()->IsFinish()) //현재 스킬 오브젝트의 현재 프레임이 끝이거나 0 일 경우 NONE으로
		{
			if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR1)
			{
				if (Papulatus_State == PAPULATUS_FSM::PHASE0_DIE)
				{
					Papulatus_State = PAPULATUS_FSM::PHASE1_STAND;
					Phase0Check = false;
					Phase1Check = true;
				}
			}
			else if (PAPULATUS_FSM::PHASE_SEPARATOR1 < Papulatus_State && Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
			{
				if (Papulatus_State == PAPULATUS_FSM::PHASE1_DIE)
				{
					Papulatus_State = PAPULATUS_FSM::PHASE2_FLY;
					Phase1Check = false;
					Phase2Check = true;
					return;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_START)
				{
					PapulatusFallen = true;

					Papulatus_State = PAPULATUS_FSM::PHASE1_ALARM_ING;
					return;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_ING)
				{
					return;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_END)
				{
					PapulatusFallen = false;
					CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_CRASH;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE1_TOSLEEP)
				{
					Papulatus_State = PAPULATUS_FSM::PHASE1_SLEEPING;
					return;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE1_SLEEPING)
				{
					return;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE1_WAKE_UP)
				{
					CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_CRASH;
				}

				SetDir();
				Papulatus_State = PAPULATUS_FSM::PHASE1_STAND;
			}
			else if (Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
			{
				if (Papulatus_State == PAPULATUS_FSM::PHASE2_DIE)
				{
					Phase2Check = false;
					//		Papulatus_State = PAPULATUS_FSM::NONE;
					return;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_START)
				{
					Papulatus_State = PAPULATUS_FSM::PHASE2_ALARMING;
					return;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE2_ALARMING)
				{
					return;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_END)
				{
					CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::THROW_GEAR;
				}

				SetDir();
				Papulatus_State = PAPULATUS_FSM::PHASE2_STAND;
			}
		}
	}
	return;
}

void CPapulatusScript::CurDir()
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


void CPapulatusScript::SetDir()
{
	if (Animator2D()->GetCurAnim()->IsFinish())
	{
		//		if (Transform()->GetLocalPos().x < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->GetLocalPos().x) 
		if ((Transform()->GetLocalPos().x + Collider()->GetOffsetPos().x) < CPlayerScript::GetPlayerPos().x)
		{
			m_MonsterDir = DIR::RIGHT;
		}
		else
		{
			m_MonsterDir = DIR::LEFT;
		}
	}
}


void CPapulatusScript::SectionMove()
{
	if (Papulatus_State <= PAPULATUS_FSM::PHASE_SEPARATOR1)
	{
		return;
	}

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
			if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_STAND;
			}
			else if (Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
			{
				Papulatus_State = PAPULATUS_FSM::PHASE2_STAND;
			}
			return;
		}
		IdleAcc = 0.f;
		m_fSleepAcc = 0.f;
		Rand_SleepTime = (float)Well512Random::Instance().GetValue(1, 4);
	}

	RandomTime = (float)Well512Random::Instance().GetValue(8, 20);
}

void CPapulatusScript::PlayerTracing()
{
	if (Papulatus_State <= PAPULATUS_FSM::PHASE_SEPARATOR1)
	{
		return;
	}

	if (GravityPtr != nullptr)
	{
		if (GravityPtr->IsColl())
		{
			return;
		}
	}

	if (CPlayerScript::PlayerTerrain == MonsterTerrain)
	{
		Patrol_On = false;
	}

	if (Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
	{
		if (UpFromPlayer())
		{
			TerrainPos = CPapulatusScript::TerrainPosCalling;
			TerrainScale = CPapulatusScript::TerrainScaleCalling;

			m_bTracing = m_bAggro;
			if (m_bTracing)
			{
				if (Transform()->GetLocalPos().y - CPlayerScript::GetPlayerPos().y > 40.f)
				{
					if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_START || Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_ING || Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_END || Papulatus_State == PAPULATUS_FSM::PHASE1_TOSLEEP || Papulatus_State == PAPULATUS_FSM::PHASE1_SLEEPING || Papulatus_State == PAPULATUS_FSM::PHASE1_WAKE_UP || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_START || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARMING || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_END)
					{
						m_bTracing = false;
					}
					else
					{
						Papulatus_State = PAPULATUS_FSM::PHASE2_TRACING;

						vPos = Transform()->GetLocalPos();
						vPos.y -= 130.f * DT;

						Transform()->SetLocalPos(vPos);
					}
				}
				else
				{
					m_bTracing = false;
				}
			}
			if (!m_bTracing)
			{
				YSort = true;
			}
			else
			{
				YSort = false;
			}
		}
		else
		{
			TerrainPos = CPapulatusScript::TerrainPosCalling;
			TerrainScale = CPapulatusScript::TerrainScaleCalling;

			m_bTracing = m_bAggro;
			if (m_bTracing)
			{
				if (CPlayerScript::GetPlayerPos().y - Transform()->GetLocalPos().y > 40.f)
				{
					if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_START || Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_ING || Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_END || Papulatus_State == PAPULATUS_FSM::PHASE1_TOSLEEP || Papulatus_State == PAPULATUS_FSM::PHASE1_SLEEPING || Papulatus_State == PAPULATUS_FSM::PHASE1_WAKE_UP || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_START || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARMING || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_END)
					{
						m_bTracing = false;
					}
					else
					{
						Papulatus_State = PAPULATUS_FSM::PHASE2_TRACING;

						vPos = Transform()->GetLocalPos();
						vPos.y += 130.f * DT;

						Transform()->SetLocalPos(vPos);
					}
				}
				else
				{
					m_bTracing = false;
				}
			}
			if (!m_bTracing)
			{
				YSort = true;
			}
			else
			{
				YSort = false;
			}
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
				if ((CPlayerScript::GetPlayerPos().x) - (Transform()->GetLocalPos().x) >= 250.f) //몬스터의 공격 실행하는
				{
					if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_START || Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_ING || Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_END || Papulatus_State == PAPULATUS_FSM::PHASE1_TOSLEEP || Papulatus_State == PAPULATUS_FSM::PHASE1_SLEEPING || Papulatus_State == PAPULATUS_FSM::PHASE1_WAKE_UP || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_START || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARMING || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_END)
					{
						m_bTracing = false;
					}
					else
					{
						m_MonsterDir = DIR::RIGHT;
						if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
						{
							Papulatus_State = PAPULATUS_FSM::PHASE1_TRACING;
						}
						else if (Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
						{
							Papulatus_State = PAPULATUS_FSM::PHASE2_TRACING;
						}

						vPos = Transform()->GetLocalPos();
						vPos.x += 130.f * DT;

						Transform()->SetLocalPos(vPos);
					}
				}
				else
				{
					//		m_MonsterDir = DIR::RIGHT;
					m_bTracing = false;
				}
				SetDir();
			}
			if (!m_bTracing)
			{
				SetDir();
//				if (((CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->GetLocalPos().y)) - (Transform()->GetLocalPos().y) <= 200.f && (Transform()->GetLocalPos().y) - ((CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->GetLocalPos().y)) <= 200.f)
				{
					static float AttDelay = 0.f;
					AttDelay += DT;

					if (AttDelay < 0.7f)
					{
						return;
					}

					AttDelay = 0.f;

					if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
					{
						PapulatusFSM();
					}
					else if (Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
					{
						if (YSort)
						{
							PapulatusFSM();
						}
					}
				}
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
				if ((Transform()->GetLocalPos().x) - (CPlayerScript::GetPlayerPos().x) >= 250.f)
				{
					if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_START || Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_ING || Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_END || Papulatus_State == PAPULATUS_FSM::PHASE1_TOSLEEP || Papulatus_State == PAPULATUS_FSM::PHASE1_SLEEPING || Papulatus_State == PAPULATUS_FSM::PHASE1_WAKE_UP || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_START || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARMING || Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_END)
					{
						m_bTracing = false;
					}
					else
					{
						m_MonsterDir = DIR::LEFT;
						if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
						{
							Papulatus_State = PAPULATUS_FSM::PHASE1_TRACING;
						}
						else if (Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
						{
							Papulatus_State = PAPULATUS_FSM::PHASE2_TRACING;
						}

						vPos = Transform()->GetLocalPos();
						vPos.x -= 130.f * DT;

						Transform()->SetLocalPos(vPos);
					}
				}
				else
				{
					//	m_MonsterDir = DIR::LEFT;
					m_bTracing = false;
				}
				SetDir();
			}
			if (!m_bTracing)
			{
				SetDir();
				// 보스는 멀어져도 공격해야 하므로.
//				if (((CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->GetLocalPos().y) + 142.f) - (Transform()->GetLocalPos().y) <= 200.f && (Transform()->GetLocalPos().y) - ((CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->GetLocalPos().y) + 142.f) <= 200.f)
				{
					static float AttDelay = 0.f;
					AttDelay += DT;

					if (AttDelay < 0.7f)
					{
						return;
					}

					AttDelay = 0.f;

					if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
					{
						PapulatusFSM();
					}
					else if (Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
					{
						if (YSort)
						{
							PapulatusFSM();
						}
					}
				}
			}
		}
		else
		{
			PatrolLeft();
		}
	}
}

void CPapulatusScript::TorrentTimeCheck()
{
	if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
	{
		return;
	}

	if (TorrentCheck == false)
	{
		/*AccTorrent += 0.0001f;

		if (AccTorrent <= 0.25f)
		{
			return;
		}
		AccTorrent = 0.f;*/

		if (/*CTorrentBombScript::AfterToExplosion*/true)
		{
			TorrentCheck = true;
		}
	}
}

void CPapulatusScript::AlterEgoCheck()
{
	if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
	{
		return;
	}

	if (AlterEgoReady || AlterEgoActive)
	{
		AccAlterEgo += DT * 0.1f;
		
		if (AccAlterEgo <= 0.1f)
		{
			return;
		}
		AccAlterEgo = 0.f;
		AlterEgoActive = false;
		AlterEgoReady = false;
	}
}

void CPapulatusScript::SetMove()
{
	vPos = Transform()->GetLocalPos();

	if (CPlayerScript::PlayerSkillPos.x <= Transform()->GetLocalPos().x)
	{
		if (TerrainPos.x + (TerrainScale.x * 0.5f) > Transform()->GetLocalPos().x + (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
		{
			vPos.x += 15 * DT;
		}
		m_MonsterDir = DIR::LEFT;

		Transform()->SetLocalPos(vPos);
	}
	else
	{
		if (TerrainPos.x - (TerrainScale.x * 0.5f) < Transform()->GetLocalPos().x - (Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x) * 0.1f)
		{
			vPos.x -= 15 * DT;
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

void CPapulatusScript::BlinkPlayer()
{
	if (PapulatusPtr == nullptr)
	{
		for (size_t i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec().size(); ++i)
		{
			if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec()[i]->GetName() == L"Papulatus")
			{
				PapulatusPtr = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec()[i];
			}
		}
	}

	Vec3 ofPlayerPos = CPlayerScript::GetPlayerPos();
	Vec3 ofPapulatusPos = PapulatusPtr->Transform()->GetLocalPos();


	Vec3 ApplyPos = ofPlayerPos;

	ApplyPos.z = ofPapulatusPos.z;

	PapulatusPtr->Transform()->SetLocalPos(ApplyPos);

	ApplyPos = ofPapulatusPos;
	ApplyPos.z = ofPlayerPos.z;

	CPlayerScript::PlayerObject->Transform()->SetLocalPos(ApplyPos);
}

void CPapulatusScript::PapulatusFSM()
{
	if (PapulatusPtr == nullptr)
	{
		for (size_t i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec().size(); ++i)
		{
			if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec()[i]->GetName() == L"Papulatus")
			{
				PapulatusPtr = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"MONSTER")->GetObjectVec()[i];
			}
		}
	}

	if (CPlayerScript::PlayerObject == nullptr)
	{
		for (size_t i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec().size(); ++i)
		{
			if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[i]->GetName() == L"Player")
			{
				CPlayerScript::PlayerObject = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[i];
			}
		}
	}

	if (Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
	{
		if (CInstantDeathAreaScript::PapulatusSignal && DeadLockProcess_CrackLaser && Papulatus_State != PAPULATUS_FSM::PHASE0_DIE && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_END && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_ING && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_START && Papulatus_State != PAPULATUS_FSM::PHASE1_TOSLEEP && Papulatus_State != PAPULATUS_FSM::PHASE1_SLEEPING && Papulatus_State != PAPULATUS_FSM::PHASE1_WAKE_UP)
		{
			CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::CRACK_OF_TIME;

			DeadLockProcess_CrackLaser = false;
		}
		else if (CPapulatusScript::PatternDesign1 != PAPULATUS_PHASE1_PATTERN::CRACK_OF_TIME)
		{
			CrackOfCheck = false;
		}
		
		
		if (AlramDeadLockProcessor || SleepDeadLockProcessor && !CLaserScript::LaserAccess)
		{
			if (SleepState != PAPULATUS_PHASE1_PATTERN::NONE)
			{
				CPapulatusScript::PatternDesign1 = SleepState;
			}
			SleepState = PAPULATUS_PHASE1_PATTERN::NONE;
			AlramDeadLockProcessor = false;
			SleepDeadLockProcessor = false;
		}

		switch (CPapulatusScript::PatternDesign1)
		{
		case PAPULATUS_PHASE1_PATTERN::NONE:
			CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::CRACK_OF_TIME;
			break;

		case PAPULATUS_PHASE1_PATTERN::CRACK_OF_TIME:
			if (CInstantDeathAreaScript::PapulatusSignal && !CLaserScript::LaserAccess) // 장판이 가능하다면
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_CRACK_OF_TIME;
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::TIME_BOMBS;
			}
			else
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_CRASH;
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER;
				DeadLockProcess_CrackLaser = false;
			}
			break;

		case PAPULATUS_PHASE1_PATTERN::ATTACK_CRASH:
			Papulatus_State = PAPULATUS_FSM::PHASE1_CRASH;
			CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER;
			break;

		case PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER:

			CPapulatusScript::RandomSkill = Well512Random::Instance().GetValue(1, 9);

			switch (RandomSkill)
			{
			case 1:
				Papulatus_State = PAPULATUS_FSM::PHASE1_CRASH;
				//	CPapulatusScript::PatternDesign = PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER;

				if ((GetValueHP() * 0.6f) < GetHP())
				{
					CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ALRAM;
				}
				else if (GetHP() <= (GetValueHP() * 0.6f))
				{
					if (RandomSkill % 2 == 1)
					{
						CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ALRAM;
					}
					else
					{
						CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::SLEEP;
					}
				}
				break;
			case 2:
				Papulatus_State = PAPULATUS_FSM::PHASE1_BLUERING;

				TempSkill = Well512Random::Instance().GetValue(1, 2);

				if (TempSkill == 1)
				{
					CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_CRASH;
				}
				else
				{
					CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER;
				}
				break;
			case 3:
				Papulatus_State = PAPULATUS_FSM::PHASE1_REDRING;

				TempSkill = Well512Random::Instance().GetValue(1, 2);

				if (TempSkill == 1)
				{
					CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_CRASH;
				}
				else
				{
					CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::TIME_BOMBS;
				}
				break;
			case 4:
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::TIME_BOMBS;
				break;
			case 5:
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::CURSE;
				break;
			case 6:
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::TIME_BOMBS;
				break;
			case 7:
				if (!CInstantDeathAreaScript::isAlive && !CLaserScript::Accoff && !CLaserScript::LaserAccess && CInstantDeathAreaScript::PapulatusSignal && Papulatus_State != PAPULATUS_FSM::PHASE0_DIE && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_END && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_ING && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_START && Papulatus_State != PAPULATUS_FSM::PHASE1_TOSLEEP && Papulatus_State != PAPULATUS_FSM::PHASE1_SLEEPING && Papulatus_State != PAPULATUS_FSM::PHASE1_WAKE_UP)
				{
					CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::CRACK_OF_TIME;
				}
				break;
			case 8:
				if (!CInstantDeathAreaScript::isAlive && !CLaserScript::Accoff && !CLaserScript::LaserAccess && CInstantDeathAreaScript::PapulatusSignal && Papulatus_State != PAPULATUS_FSM::PHASE0_DIE && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_END && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_ING && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_START && Papulatus_State != PAPULATUS_FSM::PHASE1_TOSLEEP && Papulatus_State != PAPULATUS_FSM::PHASE1_SLEEPING && Papulatus_State != PAPULATUS_FSM::PHASE1_WAKE_UP)
				{
					if (SleepState == PAPULATUS_PHASE1_PATTERN::ALRAM)
					{
						CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ALRAM;
					}
					else
					{
						CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::CRACK_OF_TIME;
					}
				}
				break;
			case 9:
				if (!CInstantDeathAreaScript::isAlive && !CLaserScript::Accoff && !CLaserScript::LaserAccess && CInstantDeathAreaScript::PapulatusSignal && Papulatus_State != PAPULATUS_FSM::PHASE0_DIE && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_END && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_ING && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_START && Papulatus_State != PAPULATUS_FSM::PHASE1_TOSLEEP && Papulatus_State != PAPULATUS_FSM::PHASE1_SLEEPING && Papulatus_State != PAPULATUS_FSM::PHASE1_WAKE_UP)
				{
					CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::CRACK_OF_TIME;
				}
				break;
			default:
				break;
			}

			break;

		case PAPULATUS_PHASE1_PATTERN::TIME_BOMBS:
			if (TimeBombCheck) // 현재 폭탄이 설치되어 있다면.
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_CRASH;
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER;
				break;
			}
			else
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_TIME_BOMB;
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_CRASH;
				break;
			}

		case PAPULATUS_PHASE1_PATTERN::CURSE:

			if (CurseCheck == false)
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_CURSE;
				CurseCheck = true;
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER;
			}
			else
			{
				if (CurAcctime <= 3.f)
				{
					CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_CRASH;
					return;
				}
				CurAcctime = 0.f;
				CurseCheck = false;
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER;
			}
			break;

		case PAPULATUS_PHASE1_PATTERN::ALRAM:
			if (!DoSleepHeal || !CInstantDeathAreaScript::PapulatusSignal)
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_CRASH;
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER;
				break;
			}
			if (Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_START && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_ING && Papulatus_State != PAPULATUS_FSM::PHASE1_ALARM_END && !CLaserScript::LaserAccess && !CLaserScript::Accoff && !CActiveExplosionScript::Active_Explosion)
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_ALARM_START;
				AlramingCheck = true;
				LaserOffAction = true;
			}
			else if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_START)
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_ALARM_ING;
				AlramingCheck = true;
				LaserOffAction = true;
			}
			else if (Papulatus_State == PAPULATUS_FSM::PHASE1_ALARM_ING)
			{
				AlramingCheck = true;
				LaserOffAction = true;
				static float AccAlram = 0.f;

				AccAlram += DT;

				if (AccAlram <= 0.02f)
				{
					return;
				}
				AccAlram = 0.f;

				Papulatus_State = PAPULATUS_FSM::PHASE1_ALARM_END;
				AlramingCheck = false;
				LaserOffAction = false;

				DoSleepHeal = false;

			}
			else if (CLaserScript::LaserAccess && !CActiveExplosionScript::Active_Explosion)
			{
				AlramDeadLockProcessor = true;
				SleepState = PAPULATUS_PHASE1_PATTERN::ALRAM;
				Papulatus_State = PAPULATUS_FSM::PHASE1_CRASH;
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER;
			}
			break;

		case PAPULATUS_PHASE1_PATTERN::SLEEP:

			if (Papulatus_State != PAPULATUS_FSM::PHASE1_TOSLEEP && Papulatus_State != PAPULATUS_FSM::PHASE1_SLEEPING && Papulatus_State != PAPULATUS_FSM::PHASE1_WAKE_UP && !CLaserScript::LaserAccess)
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_TOSLEEP;
				AlramingCheck = true;
				LaserOffAction = true;
			}
			else if (Papulatus_State == PAPULATUS_FSM::PHASE1_TOSLEEP)
			{
				Papulatus_State = PAPULATUS_FSM::PHASE1_SLEEPING;
				LaserOffAction = true;
			}
			else if (Papulatus_State == PAPULATUS_FSM::PHASE1_SLEEPING)
			{
				LaserOffAction = true;
				static float AccSleep = 0.f;

				AccSleep += 0.01f;

				AddHP((int)(GetValueHP() * 0.01f));

				if (AccSleep <= 0.15f)
				{
					return;
				}
				AccSleep = 0.f;
				LaserOffAction = false;
				Papulatus_State = PAPULATUS_FSM::PHASE1_WAKE_UP;
			}
			else if (CLaserScript::LaserAccess && !CActiveExplosionScript::Active_Explosion)
			{
				SleepDeadLockProcessor = true;
				SleepState = PAPULATUS_PHASE1_PATTERN::SLEEP;
				Papulatus_State = PAPULATUS_FSM::PHASE1_CRASH;
				CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_OTHER;
			}
			break;

		case PAPULATUS_PHASE1_PATTERN::END:
			CPapulatusScript::PatternDesign1 = PAPULATUS_PHASE1_PATTERN::ATTACK_CRASH;
			break;

		default:
			break;
		}
	}
	else if (Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
	{	
		if (Prev_AlterActive && !AlterEgoActive)
		{
			CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::HEALING;
			Prev_AlterActive = false;
		}
		else
		{
			switch (PatternDesign2)
			{
			case PAPULATUS_PHASE2_PATTERN::NONE:
				CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::THROW_GEAR;
				break;

			case PAPULATUS_PHASE2_PATTERN::THROW_GEAR:
				Papulatus_State = PAPULATUS_FSM::PHASE2_THROW_GEAR;

				CPapulatusScript::RandomSkill = Well512Random::Instance().GetValue(0, 4);

				switch (CPapulatusScript::RandomSkill)
				{
				case 0:
					CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::BLINK;
					break;
				case 1:
					CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::SUMMON_ALTER_EGO;
					break;
				case 2:
					CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::SUMMON_ALTER_EGO;
					break;
				case 3:
					CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::TORRENT_OF_TIME;
					break;
				case 4:
					CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::ALARMMING;
					break;
				default:
					break;
				}
				break;

			case PAPULATUS_PHASE2_PATTERN::SUMMON_ALTER_EGO:

				if (!AlterEgoReady && !AlterEgoActive)
				{
					Papulatus_State = PAPULATUS_FSM::PHASE2_SUMMON_ALTER_EGO;
					CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::THROW_GEAR;

					AlterEgoReady = true;
				}
				else
				{
					Papulatus_State = PAPULATUS_FSM::PHASE2_THROW_GEAR;
					CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::THROW_GEAR;
				}
				break;

			case PAPULATUS_PHASE2_PATTERN::HEALING:

				CPapulatusScript::AlterEgo_Distinguish = true;
				Papulatus_State = PAPULATUS_FSM::PHASE2_HEALLING;
				CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::THROW_GEAR;
				break;

			case PAPULATUS_PHASE2_PATTERN::BLINK:				
				Papulatus_State = PAPULATUS_FSM::PHASE2_BLINK;
				CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::THROW_GEAR;
				break;

			case PAPULATUS_PHASE2_PATTERN::TORRENT_OF_TIME:

				if (TorrentCheck == false) // Torrent 불가능한 상황
				{
					Papulatus_State = PAPULATUS_FSM::PHASE2_THROW_GEAR;
					CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::THROW_GEAR;
				}
				else
				{
					Papulatus_State = PAPULATUS_FSM::PHASE2_TORRENT_OF_TIME;
					CPapulatusScript::PatternDesign2 = PAPULATUS_PHASE2_PATTERN::THROW_GEAR;

					TorrentCheck = false;
				}
				break;

			case PAPULATUS_PHASE2_PATTERN::ALARMMING:
				if (Papulatus_State != PAPULATUS_FSM::PHASE2_ALARM_START && Papulatus_State != PAPULATUS_FSM::PHASE2_ALARMING && Papulatus_State != PAPULATUS_FSM::PHASE2_ALARM_END)
				{
					Papulatus_State = PAPULATUS_FSM::PHASE2_ALARM_START;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE2_ALARM_START)
				{
					Papulatus_State = PAPULATUS_FSM::PHASE2_ALARMING;
					AlramingCheck = true;
				}
				else if (Papulatus_State == PAPULATUS_FSM::PHASE2_ALARMING)
				{
					AlramingCheck = true;

					static float AccAlram = 0.f;

					AccAlram += 0.01f;

					if (AccAlram <= 0.25f)
					{
						return;
					}
					AccAlram = 0.f;

					Papulatus_State = PAPULATUS_FSM::PHASE2_ALARM_END;
					AlramingCheck = false;
				}
				break;

			case PAPULATUS_PHASE2_PATTERN::END:
				break;

			default:
				break;
			}
		}
	}
}


void CPapulatusScript::PatrolLeft()
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

void CPapulatusScript::PatrolRight()
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



void CPapulatusScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

int CPapulatusScript::update()
{
	TorrentTimeCheck();
	AlterEgoCheck();
	ChangeState();
	CurDir();
	DoCount();

	CurAcctime += 0.001f;

	if (Papulatus_State != Prev_PapulatusState)
	{
		state();
	}
	Prev_PapulatusState = Papulatus_State;

	TimeBombsCount();

	/*if (Papulatus_State == PAPULATUS_FSM::NONE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}*/

	//	Prev_TERRAIN = Script_TERRAIN;
	if (PapualtusHP < Phase1HP && Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR1 && Papulatus_State < PAPULATUS_FSM::PHASE_SEPARATOR2)
	{
		PrevPapualtusHP = PapualtusHP;
	}
	else if (PapualtusHP < Phase2HP && Papulatus_State > PAPULATUS_FSM::PHASE_SEPARATOR2)
	{
		PrevPapualtusHP = PapualtusHP;
	}

	if (PapualtusHP > Phase1HP)
	{
		PrevPapualtusHP = 50000000;
	}
	return 0;
}

void CPapulatusScript::OnCollisionEnter(CCollider * _pOther)
{
	//CSceneMgr::GetInst()->GetCurScene()->GetLayer(_pOther->GetGameObject()->GetLayerIdx());

	//if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
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
//수정된 부분 여기

void CPapulatusScript::OnCollision(CCollider * _pOther)
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

			TerrainPosCalling = _pOther->GetGameObject()->Transform()->GetLocalPos();
			TerrainScaleCalling = _pOther->GetGameObject()->Transform()->GetLocalScale();

			if (TerrainPosY + (TerrainScaleY * 0.5f) <= MonsterPosY - (MonsterScaleY * MonsterColliderScale * 0.5f))
			{
				TerrainPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
				TerrainScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

				Script_TERRAIN = _pOther->GetGameObject();
				MonsterTerrain = _pOther->GetGameObject();
			}
		}
	}
}

void CPapulatusScript::OnCollisionExit(CCollider * _pOther)
{

}