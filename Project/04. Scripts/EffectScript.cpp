#include "stdafx.h"
#include "EffectScript.h"

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

#include "PlayerScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

//#include "InstantDeathAreaScript.h"
#include "PapulatusScript.h"

CEffectScript::CEffectScript()
	: EFFECT_SkillState(SKILL_TYPE::NONE)
	, m_iScriptCount(0)
	, EffectSequence(0)
	, m_iEffectCount(-1)
	, PlayerPos(0.f, 0.f, 0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::EFFECTSCRIPT);
}


CEffectScript::~CEffectScript()
{
}



void CEffectScript::awake()
{
	m_pEffectPrefab = CResMgr::GetInst()->Load<CPrefab>(L"EFFECT_OBJECT");
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

void CEffectScript::Skillstate()
{
	switch (EFFECT_SkillState)
	{
	case SKILL_TYPE::NONE:
		break;

	case SKILL_TYPE::COMBO_DEATH_FAULT_HIT:
		Animator2D()->PlayAnim(L"400011027.hit.0.frames");
		break;

	case SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_0:
		Animator2D()->PlayAnim(L"Texture\\Resources\\Maplestory\\skill\\Combo Death fault\\400011027.special.0.frames\\");
		break;

	case SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_1:
		Animator2D()->PlayAnim(L"Texture\\Resources\\Maplestory\\skill\\Combo Death fault\\400011027.special.1.frames\\");
		break;

	case SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_2:
		Animator2D()->PlayAnim(L"Texture\\Resources\\Maplestory\\skill\\Combo Death fault\\400011027.special.2.frames\\");
		break;

	case SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_3:
		Animator2D()->PlayAnim(L"Texture\\Resources\\Maplestory\\skill\\Combo Death fault\\400011027.special.3.frames\\");
		break;

	case SKILL_TYPE::COMBO_INSTINCT:
		Animator2D()->PlayAnim(L"400011075.hit.0.frames");
		break;

	case SKILL_TYPE::RAGINGBLOW_PHASE1:
		Animator2D()->PlayAnim(L"1121008.hit.0.frames");
		break;

	case SKILL_TYPE::RAGINGBLOW_PHASE2:
		Animator2D()->PlayAnim(L"1120017.hit.0.frames");
		break;

	case SKILL_TYPE::SHOUT_HIT:
		Animator2D()->PlayAnim(L"1111008.hit.0.frames");
		break;

	case SKILL_TYPE::ALPHAPHASE_HIT:
		Animator2D()->PlayAnim(L"400011098.hit.0.frames");
		break;

	case SKILL_TYPE::BETAPHASE_HIT:
		Animator2D()->PlayAnim(L"400011100.hit.0.frames");
		break;

	case SKILL_TYPE::SOUL_ECLIPSE_PHASE:
		Animator2D()->PlayAnim(L"400011088.hit.0.frames");
		break;

	case SKILL_TYPE::GATEKEEPER_ATTACK:
		Animator2D()->PlayAnim(L"8160000.img.attack2.info.hit.frames");
		break;

	case SKILL_TYPE::GIGANTICVIKING_ATTACK:
		Animator2D()->PlayAnim(L"8141100.img.attack1.info.hit.frames");
		break;

	case SKILL_TYPE::PHANTOMWATCH_ATTACK:
		Animator2D()->PlayAnim(L"8143000.img.attack1.info.areaWarning.frames");
		break;

	case SKILL_TYPE::PAPULATUS_BLUERING:
		Animator2D()->PlayAnim(L"123.img.level.97.effect.frames");
		break;

	case SKILL_TYPE::PAPULATUS_REDRING:
		Animator2D()->PlayAnim(L"241.img.level.5.hit.frames");
		break;

	case SKILL_TYPE::PAPULATUS_TORRENT_OF_TIME:
		Animator2D()->PlayAnim(L"PAPUL.2Phase");
		break;

	case SKILL_TYPE::PAPULATUS_TIME_BOMB:
		Animator2D()->PlayAnim(L"8500021.img.attack1.info.hit.frames");
		break;

	case SKILL_TYPE::PAPULATUS_CRASH:
		Animator2D()->PlayAnim(L"8500021.img.attack2.info.hit.frames");
		break;

	case SKILL_TYPE::PAPULATUS_CRACK_OF_TIME:
		Animator2D()->PlayAnim(L"131.img.level.29.affected.frames");
		break;

	case SKILL_TYPE::PAPULATUS_LASER_OVER:
		Animator2D()->PlayAnim(L"176.img.level.41.hit.frames");
		break;

	case SKILL_TYPE::PAPULATUS_THROW_GEAR:
		Animator2D()->PlayAnim(L"8500002.img.attack1.info.hit.frames");
		break;

	case SKILL_TYPE::PAPULATUS_OBJECT_60:
		Animator2D()->PlayAnim(L"BasicEff.img.ObtacleAtom.60.hit.frames");
		break;

	case SKILL_TYPE::PAPULATUS_OBJECT_61:
		Animator2D()->PlayAnim(L"BasicEff.img.ObtacleAtom.61.hit.frames");
		break;

	case SKILL_TYPE::END:
		break;

	default:
		break;
	}
}

void CEffectScript::SkillPos()
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		if (EFFECT_SkillState == SKILL_TYPE::NONE)
		{
			Transform()->SetLocalPos(DISABLE_POS);
			return;
		}
		else
		{
			if (EFFECT_SkillState == SKILL_TYPE::PHANTOMWATCH_ATTACK || EFFECT_SkillState == SKILL_TYPE::GATEKEEPER_ATTACK)
			{
				vPos = CPlayerScript::EffectPlayerPos;
			}
			else
			{
				vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();
			}

			if (EFFECT_SkillState == SKILL_TYPE::RAGINGBLOW_PHASE1)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 5.f;
					}
					else
					{
						vPos.x += 58.f;
					}
					vPos.y -= 50.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.x += 30.f;
					vPos.y -= 100.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::RAGINGBLOW_PHASE2)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						//		vPos.x -= 15.f;
						vPos.x -= 30.f;
					}
					else
					{
						//		vPos.x += 25.f;
						vPos.x += 38.f;
					}
					vPos.y -= 45.f;
					//	vPos.y -= 30.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.x += 10.f;
					vPos.y -= 100.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::COMBO_INSTINCT)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 20.f;
					}
					else
					{
						vPos.x += 20.f;
					}
					vPos.y -= 45.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.y -= 75.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::SHOUT_HIT)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 20.f;
					}
					else
					{
						vPos.x += 20.f;
					}
					vPos.y -= 45.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.y -= 75.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::COMBO_DEATH_FAULT_HIT)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 20.f;
					}
					else
					{
						vPos.x += 20.f;
					}
					vPos.y -= 45.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.y -= 80.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_0)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 20.f;
					}
					else
					{
						vPos.x += 20.f;
					}
					vPos.y -= 45.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.y -= 80.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_1)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 20.f;
					}
					else
					{
						vPos.x += 20.f;
					}
					vPos.y -= 45.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.y -= 80.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_2)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 20.f;
					}
					else
					{
						vPos.x += 20.f;
					}
					vPos.y -= 45.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.y -= 80.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_3)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 20.f;
					}
					else
					{
						vPos.x += 20.f;
					}
					vPos.y -= 45.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.y -= 80.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::ALPHAPHASE_HIT)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 20.f;
					}
					else
					{
						vPos.x += 20.f;
					}
					vPos.y -= 45.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.y -= 80.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::BETAPHASE_HIT)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 20.f;
					}
					else
					{
						vPos.x += 20.f;
					}
					vPos.y -= 45.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.y -= 80.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::SOUL_ECLIPSE_PHASE)
			{
				if (GetGameObject()->GetParent()->GetName() == L"Papulatus")
				{
					if (CPapulatusScript::PapulatusDir() == DIR::LEFT)
					{
						vPos.x -= 20.f;
					}
					else
					{
						vPos.x += 20.f;
					}
					vPos.y -= 45.f;
					vPos.z -= 200.f;
				}
				else
				{
					vPos.y -= 60.f;
					vPos.z -= 200.f;
				}
			}
			else if (EFFECT_SkillState == SKILL_TYPE::GATEKEEPER_ATTACK)
			{
				vPos.y += 25.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::GIGANTICVIKING_ATTACK)
			{
				vPos.y -= 20.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PHANTOMWATCH_ATTACK)
			{
				vPos.x -= 5.f;
				vPos.y += 10.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_BLUERING)
			{
				vPos.x += 8.f;
				vPos.y -= 10.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_REDRING)
			{
				vPos.x -= 0.f;
				vPos.y += 10.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_TORRENT_OF_TIME)
			{
				vPos.x -= 0.f;
				vPos.y -= 18.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_TIME_BOMB)
			{
				vPos.y -= 5.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_CRASH)
			{
				vPos.x += 70.f;
				vPos.y -= 7.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_CRACK_OF_TIME)
			{
				vPos.x -= 0.f;
				vPos.y -= 20.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_LASER_OVER)
			{
				vPos.x += 50.f;
				vPos.y -= 10.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_THROW_GEAR)
			{
				vPos.x -= 0.f;
				vPos.y += 0.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_OBJECT_60)
			{
				vPos.x -= 0.f;
				vPos.y += 0.f;
				vPos.z -= 100.f;
			}
			else if (EFFECT_SkillState == SKILL_TYPE::PAPULATUS_OBJECT_60)
			{
				vPos.x -= 0.f;
				vPos.y += 0.f;
				vPos.z -= 100.f;
			}
			else
			{
				vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();
			}
		}
		if (GetGameObject()->GetParent()->GetName() != L"Player")
		{
			switch (EffectSequence % 5)
			{
			case 0:
				vPos.x += 10.f;
				vPos.y += 28.f;
				break;
			case 1:
				vPos.x -= 24.f;
				vPos.y -= 16.f;
				break;
			case 2:
				vPos.x += 28.f;
				break;
			case 3:
				vPos.x -= 24.f;
				vPos.y += 16.f;
				break;
			case 4:
				vPos.x += 10.f;
				vPos.y -= 28.f;
				break;
			default:
				break;
			}
		}
		vPos.z = CPlayerScript::PlayerPos.z - 1.f;
		Transform()->SetLocalPos(vPos);
	}
	else
	{
		CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->AddChild(GetGameObject());
	}
}

void CEffectScript::SkillChangeState()
{
	if (Animator2D()->GetCurAnim() != nullptr)
	{
		if (Animator2D()->GetCurAnim()->IsFinish() && EFFECT_SkillState != SKILL_TYPE::NONE)
		{
			EFFECT_SkillState = SKILL_TYPE::NONE;

			if (GetGameObject()->GetParent()->GetScript().size() != 0)
			{
				GetGameObject()->GetParent()->GetScript()[m_iScriptCount]->SetSkillType((UINT)SKILL_TYPE::NONE);
			}

			CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->AddChild(GetGameObject());
			EffectSequence = 0;
		}
	}
	return;
}

void CEffectScript::CurDirSet()
{
	if (PlayerPos.x == 0.f && PlayerPos.y == 0.f && PlayerPos.z == 0.f)
	{
		PlayerPos = CPlayerScript::GetPlayerPos();
	}

	if (EFFECT_SkillState == SKILL_TYPE::COMBO_DEATH_FAULT_HIT || EFFECT_SkillState == SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_0 || EFFECT_SkillState == SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_1 || EFFECT_SkillState == SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_2 || EFFECT_SkillState == SKILL_TYPE::COMBO_DEATH_FAULT_SPECIAL_3)
	{
		SKILL_DIR = CPlayerScript::GetPlayerDir();

		switch (SKILL_DIR)
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

		return;
	}

	if (vPos.x < GetGameObject()->Transform()->GetLocalPos().x)
	{
		SKILL_DIR = DIR::RIGHT;
	}
	else
		SKILL_DIR = DIR::LEFT;

	switch (SKILL_DIR)
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

bool CEffectScript::LeftFromPlayer()
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

int CEffectScript::update()
{
	SkillPos();

	if (EFFECTPREV_SkillState != EFFECT_SkillState)
	{
		CurDirSet();
		Skillstate();
	}
	EFFECTPREV_SkillState = EFFECT_SkillState;

	SkillChangeState();

	if (EFFECT_SkillState == SKILL_TYPE::NONE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}

