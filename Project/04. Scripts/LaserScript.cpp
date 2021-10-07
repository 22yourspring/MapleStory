#include "stdafx.h"
#include "LaserScript.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"
#include "AnrealEngine/Collider.h"
#include "ResMgr.h"

#include "AnrealEngine/CollisionMgr.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/define.h"

#include "AnrealEngine/Script.h"

#include "PlayerScript.h"
#include "PapulatusScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "PapulatusScript.h"
#include "InstantDeathAreaScript.h"
#include "LaserBodyScript.h"

bool CLaserScript::RedCutting = false;
bool CLaserScript::BlueCutting = false;

Vec3 CLaserScript::vRedPos = {0.f, 0.f, 0.f};
Vec3 CLaserScript::vBluePos = { 0.f, 0.f, 0.f };

bool CLaserScript::RedCollReady = false;
bool CLaserScript::BlueCollReady = false;
bool CLaserScript::CollisionReady = false;

float CLaserScript::BlueTheta = 0.f;
float CLaserScript::RedTheta = 0.f;
float CLaserScript::RedAssist = 0.f;
float CLaserScript::BlueAssist = 0.f;

float CLaserScript::RedAcc = 0.18f;
float CLaserScript::BlueAcc = 0.11f;

bool CLaserScript::LaserAccess = false;

float CLaserScript::AddTime = 0.f;

int CLaserScript::LaserPattern = 0;
int CLaserScript::LaserType = 10;
int CLaserScript::PrevLaserPat = 10;

bool CLaserScript::Fir_FirLaser = false;

bool CLaserScript::Accoff = true;

CLaserScript::CLaserScript()
	: LASER_STATE(LASER_FSM::NONE)
	, Laser_Identifier(LASER_TYPE::NONE)
	, LaserIdentifierChecker(true)
	, FirstAccess(true)
	, LaserSetting(true)
	, Fir_FirLaserSet(true)
	, AddRedX(0.f)
	, AddRedY(0.f)
	, AddBlueX(0.f)
	, AddBlueY(0.f)
	, FindStart(false)
	, isInit(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::LASERSCRIPT);
}

CLaserScript::~CLaserScript()
{
}

void CLaserScript::SetLaserPattern()
{
	if (LASER_STATE == LASER_FSM::DIE)
	{
		return;
	}
	if (LaserPattern != PrevLaserPat)
	{
		LaserType = Well512Random::Instance().GetValue(0, 4);

		if (CPapulatusScript::PapulatusPtr == nullptr)
		{
			return;
		}
		vRedPos = CPapulatusScript::PapulatusPtr->Transform()->GetLocalPos();
		
		vRedPos.y = 100.f;
		vRedPos.z = 405.f;

		vRedPos.x *= 0.5;
		vBluePos = vRedPos;

		switch (LaserType)
		{
		case 0:
			AddRedX = 400.f;
			AddBlueX = 500.f;
			AddRedY = 60.f;
			AddBlueY = 5.f;

			vRedPos.x -= AddRedX;
			vBluePos.x += AddBlueX;
			vRedPos.y += AddRedY;
			vBluePos.y -= AddBlueY;

			AddRedX = 0.f;
			AddBlueX = 0.f;
			AddRedY = 0.f;
			AddBlueY = 0.f;

			RedAcc = 0.25f;
			BlueAcc = 0.1f;
			break;
		case 1:
			AddRedX = 300.f;
			AddBlueX = 600.f;
			AddRedY = 5.f;
			AddBlueY = 37.5f;

			vRedPos.x -= AddRedX;
			vBluePos.x += AddBlueX;
			vRedPos.y -= AddRedY;
			vBluePos.y += AddBlueY;

			AddRedX = 0.f;
			AddBlueX = 0.f;
			AddRedY = 0.f;
			AddBlueY = 0.f;

			RedAcc = 0.12f;
			BlueAcc = 0.3f;
			break;
		case 2:
			AddRedX = 450.f;
			AddBlueX = 450.f;

			vRedPos.x -= AddRedX;
			vBluePos.x += AddBlueX;
			vRedPos.y += AddRedY;
			vBluePos.y += AddBlueY;

			AddRedX = 0.f;
			AddBlueX = 0.f;
			AddRedY = 0.f;
			AddBlueY = 0.f;

			RedAcc = 0.28f;
			BlueAcc = 0.1f;
			break;
		case 3:
			AddRedX = 400.f;
			AddBlueX = 500.f;
			AddRedY = 5.f;
			AddBlueY = 35.f;

			vRedPos.x -= AddRedX;
			vBluePos.x += AddBlueX;
			vRedPos.y -= AddRedY;
			vBluePos.y += AddBlueY;

			AddRedX = 0.f;
			AddBlueX = 0.f;
			AddRedY = 0.f;
			AddBlueY = 0.f;

			RedAcc = 0.35f;
			BlueAcc = 0.13f;
			break;
		case 4:
			AddRedX = 300.f;
			AddBlueX = 600.f;
			AddRedY = 10.f;
			AddBlueY = 10.f;

			vRedPos.x -= AddRedX;
			vBluePos.x += AddBlueX;
			vRedPos.y += AddRedY;
			vBluePos.y -= AddBlueY;

			AddRedX = 0.f;
			AddBlueX = 0.f;
			AddRedY = 0.f;
			AddBlueY = 0.f;

			RedAcc = 0.15f;
			BlueAcc = 0.35f;
			break;
		default:
			break;
		}

	}
	PrevLaserPat = LaserPattern;

	if (LaserPattern == 10)
	{
		LaserPattern = 0;
	}

	if (Laser_Identifier == LASER_TYPE::RED_LEFT)
	{
		Transform()->SetLocalPos(vRedPos);
	}
	else
	{
		Transform()->SetLocalPos(vBluePos);
	}
}

void CLaserScript::LaserAcc()
{
	if (Accoff && !LaserAccess && LaserType < 10)
	{
		static float Acc = 0.f;
		Acc += DT;

		if (Acc <= 10.f)
		{
			return;
		}
		Accoff = false;
	}
}

void CLaserScript::awake()
{
}

void CLaserScript::LaserCheck()
{
	if (LaserIdentifierChecker)
	{
		if (GetGameObject()->GetName() == L"REDLEFT_LASER")
		{
			Laser_Identifier = LASER_TYPE::RED_LEFT;
		}
		else if (GetGameObject()->GetName() == L"BLUERIGHT_LASER")
		{
			Laser_Identifier = LASER_TYPE::BLUE_RIGHT;
		}
		LaserIdentifierChecker = false;
	}
}

void CLaserScript::LaserRot()
{
	if (!LaserAccess)
	{
		return;
	}

	if (LASER_STATE == LASER_FSM::NONE && LaserSetting)
	{
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			RedTheta = Well512Random::Instance().GetFloatValue(0.f, PI * 0.3f);
			RedAssist = RedTheta;
			AddTheta = 0.f;
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			BlueTheta = Well512Random::Instance().GetFloatValue(PI * 1.7f, 6.283185307179588f);
			BlueAssist = BlueTheta;
			AddTheta = 0.f;
		}

		LaserSetting = false;
	}

	if (FirstAccess)
	{
		vRot = GetGameObject()->Transform()->GetLocalRot();

		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			vRot.z = RedTheta;
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			vRot.z = BlueTheta;
		}

		GetGameObject()->Transform()->SetLocalRot(vRot);

		FirstAccess = false;
	}

	if (LASER_STATE == LASER_FSM::ACTIVE)
	{
		ThetaUpdate();

		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			vRot = GetGameObject()->Transform()->GetLocalRot();
			AddTheta -= (float)(RedAcc * DT);

			vRot.z = RedTheta + AddTheta;
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			vRot = GetGameObject()->Transform()->GetLocalRot();
			AddTheta += (float)(BlueAcc * DT);
			vRot.z = BlueTheta + AddTheta;
		}

		Transform()->SetLocalRot(vRot);
	}
}

void CLaserScript::Skillstate()
{
	switch (LASER_STATE)
	{
	case LASER_FSM::NONE:
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			++LaserPattern; //static이라서 한곳에서만 해야하는게 효율적이라서 RED에서만 실행
		}
		break;

	case LASER_FSM::SUMMON:
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			Animator2D()->PlayAnim(L"BossPapulatus.img.ticktockLaser.type.0.activate.frames");
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			Animator2D()->PlayAnim(L"BossPapulatus.img.ticktockLaser.type.1.activate.frames");
		}
		break;

	case LASER_FSM::ACTIVE:
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			Animator2D()->PlayAnim(L"BossPapulatus.img.ticktockLaser.type.0.repeat.frames");
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			Animator2D()->PlayAnim(L"BossPapulatus.img.ticktockLaser.type.1.repeat.frames");
		}
		break;

	case LASER_FSM::DIE:
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			Animator2D()->PlayAnim(L"BossPapulatus.img.ticktockLaser.type.0.deactivate.frames");
		}
		else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			Animator2D()->PlayAnim(L"BossPapulatus.img.ticktockLaser.type.1.deactivate.frames");
		}

		Accoff = true;

		break;

	case LASER_FSM::END:
		break;

	default:
		break;
	}
}


void CLaserScript::SkillChangeState()
{
	if (LASER_STATE == LASER_FSM::NONE && !LaserAccess)
	{
		return;
	}

	if (LASER_STATE == LASER_FSM::NONE && LaserAccess)
	{
		LASER_STATE = LASER_FSM::SUMMON;
	}

	if (Animator2D()->GetCurAnim()->IsFinish() && LASER_STATE == LASER_FSM::SUMMON && PREV_LASER_STATE != LASER_FSM::NONE && LaserAccess)
	{
		LASER_STATE = LASER_FSM::ACTIVE;
	}
	else if (LASER_STATE == LASER_FSM::ACTIVE && LaserAccess)
	{	
		FindStart = true;

		if (RedAcc >= BlueAcc)
		{
			RedSlow = false;
		}
		else if (RedAcc < BlueAcc)
		{
			RedSlow = true;
		}
		
	}
	else if (LASER_STATE == LASER_FSM::ACTIVE && !LaserAccess)
	{
		LASER_STATE = LASER_FSM::DIE;
	}
	else if (Animator2D()->GetCurAnim()->IsFinish() && LASER_STATE == LASER_FSM::DIE)
	{
		CLaserScript::CollisionReady = false;
		SetAddTime(0.f);
		LaserAccess = false;

		LASER_STATE = LASER_FSM::NONE;
		LaserSetting = true;
		FirstAccess = true;

	}
}

void CLaserScript::AccCheck()
{
	if ((CPapulatusScript::Phase0Check && !CPapulatusScript::Phase1Check && !CPapulatusScript::Phase2Check && CPapulatusScript::Phase0Pass))
	{
		LaserAccess = false;
		return;
	}

	if (Fir_FirLaser && Fir_FirLaserSet)
	{
		AddTime = 100.f;
		Fir_FirLaserSet = false;

		CInstantDeathAreaScript::PapulatusSignal = true;
	}

	if ((!(CPapulatusScript::Phase0Check) && (CPapulatusScript::Phase1Check) && !(CPapulatusScript::Phase2Check) && !(CPapulatusScript::Phase1Pass)) && CInstantDeathAreaScript::PapulatusSignal && !CPapulatusScript::LaserOffAction && !LaserAccess && Fir_FirLaser)
	{
		AddTime += DT;

		if (AddTime <= 100.f)
		{
			return;
		}
		LaserAccess = true;

		SetAddTime(0.f);
	}
	if (LaserAccess)
	{
		OffLaser();
	}
}

void CLaserScript::LaserCollision()
{
	vRot = Transform()->GetLocalRot();

	if (Laser_Identifier == LASER_TYPE::RED_LEFT)
	{
		if (180.f <= vRot.z * _RADIAN && vRot.z * _RADIAN <= 360.f)
		{
			RedCollReady = true;
		}
		else
		{
			RedCollReady = false;
		}
	}
	else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
	{
		if (0.f <= vRot.z * _RADIAN && vRot.z * _RADIAN <= 180.f)
		{
			BlueCollReady = true;
		}
		else
		{
			BlueCollReady = false;
		}
	}

	if (RedCollReady && BlueCollReady)
	{
		CollisionReady = true;
	}
	else
	{
		CollisionReady = false;
	}
}


int CLaserScript::update()
{
	LaserCheck();
	LaserCollision();
	SetLaserPattern();
	AccCheck();

	LaserRot();

	LaserAcc();

	SkillChangeState();

	if (LASER_STATE != PREV_LASER_STATE)
	{
		Skillstate();
	}
	PREV_LASER_STATE = LASER_STATE;

	if (LASER_STATE == LASER_FSM::NONE)
	{
		GetGameObject()->SetActive(false);
	}
	else
	{
		GetGameObject()->SetActive(true);
	}

	return 0;
}

void CLaserScript::OffLaser()
{
	if (!FindStart)
	{
		return;
	}

	if (RedSlow)
	{
		if (Laser_Identifier == LASER_TYPE::RED_LEFT)
		{
			if (Transform()->GetLocalRot().z * _RADIAN <= 90.f)
			{
				return;
			}
			if (RedAssist * _RADIAN + 90.f > Transform()->GetLocalRot().z * _RADIAN)
			{
				LaserAccess = false;
			}
		}
	}
	else
	{
		if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
		{
			if (Transform()->GetLocalRot().z * _RADIAN >= 270.f)
			{
				return;
			}
			if (BlueAssist * _RADIAN - 90.f < Transform()->GetLocalRot().z * _RADIAN)
			{
				LaserAccess = false;
			}
		}
	}

	FindStart = false;
}

void CLaserScript::ThetaUpdate()
{
	if (Laser_Identifier == LASER_TYPE::RED_LEFT)
	{
		if (RedTheta + AddTheta < 0.f)
		{
			AddTheta = 360.f * _DEGREE;
			RedTheta = 0.f;
		}
	}
	else if (Laser_Identifier == LASER_TYPE::BLUE_RIGHT)
	{
		if (AddTheta + BlueTheta > 360.f * _DEGREE)
		{
			AddTheta = 0.f;
			BlueTheta = 0.f;
		}
	}
}
