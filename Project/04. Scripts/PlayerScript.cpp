#include "stdafx.h"

#include "PlayerScript.h"
#include "GravityScript.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "AnrealEngine/Transform.h"
#include "ResMgr.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"

#include "AnrealEngine/CollisionMgr.h"

#include "AnrealEngine/define.h"
#include "AnrealEngine/Collider.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "RagingBlowScript.h"
#include "ShoutScript.h"
#include "ShadowAlphaScript.h"
#include "ShadowBetaScript.h"
#include "MapleWarriorScript.h"
#include "BoosterScript.h"
#include "InstinctScript.h"
#include "InstinctActiveScript.h"

#include "AnrealEngine/Layer.h"
#include "CraneScript.h"
#include "AreaScript.h"

CGameObject* CPlayerScript::PlayerObject = nullptr;
CScript*	CPlayerScript::PlayerToScript = nullptr;

bool CPlayerScript::m_bAttackON = false;
bool CPlayerScript::PushedLeft = false;
bool CPlayerScript::Invincibility = false;
bool CPlayerScript::isCanControl = true;

bool CPlayerScript::HangUp = false;

bool CPlayerScript::leapTerrain = false;
float CPlayerScript::InvincibilityAcc = 0.f;
float CPlayerScript::PushedAcc = 0.f;

UINT CPlayerScript::CollisionCount = 0;

Vec3 CPlayerScript::PlayerPos = { 0, 0, 0 };
Vec3 CPlayerScript::PlayerSkillPos = { 0, 0, 0 };
Vec3 CPlayerScript::EffectPlayerPos = { 0, 0, 0 };
float CPlayerScript::EffectYvalue = 0.f;

FSM CPlayerScript::m_PlayerState = FSM::STAND;
FSM CPlayerScript::Prev_PlayerState = FSM::STAND;
DIR CPlayerScript::m_PlayerDir = DIR::LEFT;

UINT CPlayerScript::Player_COMBO_COUNT = 0;
UINT CPlayerScript::COMBO_MAXCOUNT = 10;
bool CPlayerScript::COMBO_ON = false;

int CPlayerScript::MIN_ATT = 130000;
int CPlayerScript::MAX_ATT = 190000;
int CPlayerScript::CRITICAL = 40;
int CPlayerScript::MAPLESOLDIER = 0;

CGameObject* CPlayerScript::PlayerTerrain = nullptr;

bool CPlayerScript::m_IsJump = false;
float CPlayerScript::m_fSpeed = 1500.f;
float CPlayerScript::m_fJumpForce = 1000.f;
float CPlayerScript::FLASH_JUMP_F = 980.f;
bool CPlayerScript::TerrainIn = false;

bool CPlayerScript::m_isHangOn = false;

bool CPlayerScript::PushLeft = false;
bool CPlayerScript::PushRight = false;

bool CPlayerScript::DownJump = false;

float CPlayerScript::PushForce = 700.f;
float CPlayerScript::Idle = 0.f;

CPlayerScript::CPlayerScript()
	: CScript(100000, 100000, 100000, 100000) // 임시 수정 부분 문제가 생긴다면 여길 날려야 함.
	, m_bIsInstall(false)
	, m_fAccSpeed(20.f)
	, m_iSwingCount(0)
	, Enrage(false)
	, PushBackTrue(false)
	, m_bLeftAirWork(false)
	, m_bRightAirWork(false)
	, CurChange(false)
	, LeftJump(false)
	, RightJump(false)
	, NormalJump(false)
	, FirstTimePush(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::PLAYERSCRIPT);
}

CPlayerScript::~CPlayerScript()
{
}



void CPlayerScript::ComboAttackSet()
{
	if (KEYTAB(KEY_TYPE::KEY_1))
	{
		COMBO_ON = true;
	}
}

void CPlayerScript::Jump()
{
	if (KEYTAB(KEY_TYPE::KEY_ALT) && TerrainIn)
	{
		if (KEYHOLD(KEY_TYPE::KEY_DOWN))
		{
			DownJump = true;
		}

		if (Prev_PlayerState != FSM::LADDER && Prev_PlayerState != FSM::ROPE)
		{
			if (KEYHOLD(KEY_TYPE::KEY_LEFT))
			{
				LeftJump = true;
			}
			else if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
			{
				RightJump = true;
			}
			else
			{
				NormalJump = true;
			}
		}

		m_PlayerState = FSM::JUMP;
		m_fJumpForce = 1000.f;
		m_IsJump = true;
	}
}

void CPlayerScript::Attack()
{
	if (m_PlayerState != FSM::FLASHJUMP)
	{
		if (KEYTAB(KEY_TYPE::KEY_CTRL))
		{
			m_PlayerState = FSM::ATTACK;
		}
		else if (KEYTAB(KEY_TYPE::KEY_LSHIFT))
		{
			//	if (CRagingBlowScript::RAGINGBLOW_SkillState != RAGINGBLOW_FSM::ENRAGE_ACTIVE)
			if (isCanControl)
			{
				m_PlayerState = FSM::RAGINGBLOW;
			}
		}
	}
}

void CPlayerScript::FlashAttack()
{
	if (KEYTAB(KEY_TYPE::KEY_CTRL))
	{
		switch (m_iSwingCount % 4)
		{
		case 0: Animator2D()->PlayAnim(L"swing");
			++m_iSwingCount;
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
			}

			break;

		case 1: Animator2D()->PlayAnim(L"swing2");
			++m_iSwingCount;
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
			}

			break;

		case 2: Animator2D()->PlayAnim(L"swing3");
			++m_iSwingCount;
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
			}

			break;

		case 3: Animator2D()->PlayAnim(L"swingF");
			++m_iSwingCount;
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
			}

			break;

		default:
			break;
		}
	}
	else if (KEYTAB(KEY_TYPE::KEY_LSHIFT) && isCanControl)
	{
		Animator2D()->PlayAnim(L"ragingBlow");
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			Animator2D()->GetCurAnim()->SetFrmIdx(0);
		}
	}
}


void CPlayerScript::OnCollisionEnter(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
	{
		CGravityScript::P_Pos = (GetGameObject()->Transform()->GetLocalPos().y);
		CGravityScript::P_Scale = (GetGameObject()->Transform()->GetLocalScale().y);
		CGravityScript::P_CalOffSet = (GetGameObject()->Collider()->GetOffsetPos().y);
		CGravityScript::T_Up = (_pOther->GetGameObject()->Transform()->GetLocalPos().y) + (_pOther->GetGameObject()->Transform()->GetLocalScale().y) * 0.5f;
		CGravityScript::T_Down = (_pOther->GetGameObject()->Transform()->GetLocalPos().y) - (_pOther->GetGameObject()->Transform()->GetLocalScale().y) * 0.5f;
		CGravityScript::P_Down = ((CGravityScript::P_Pos + (CGravityScript::P_Scale * CGravityScript::P_CalOffSet))) - (CGravityScript::P_Scale * (GetGameObject()->Collider()->GetOffsetScale().y) * 0.5f);
		CGravityScript::P_Up = ((CGravityScript::P_Pos + (CGravityScript::P_Scale * CGravityScript::P_CalOffSet))) + (CGravityScript::P_Scale * (GetGameObject()->Collider()->GetOffsetScale().y) * 0.5f);


		CGravityScript::T_Left = (_pOther->GetGameObject()->Transform()->GetLocalPos().x) - (_pOther->GetGameObject()->Transform()->GetLocalScale().x) * 0.5f;
		CGravityScript::T_Right = (_pOther->GetGameObject()->Transform()->GetLocalPos().x) + (_pOther->GetGameObject()->Transform()->GetLocalScale().x) * 0.5f;


		if (CGravityScript::P_Up >= CGravityScript::T_Down && CGravityScript::T_Up >= CGravityScript::P_Down && CGravityScript::T_Up >= CGravityScript::P_Up && CGravityScript::T_Down >= CGravityScript::P_Down)
		{
			CGravityScript::OverTerrain = true;
			return;
		}
		else if (CGravityScript::P_Down <= CGravityScript::T_Up && CGravityScript::T_Up <= CGravityScript::P_Up && CPlayerScript::m_IsJump)
		{
			CGravityScript::OverTerrain = false;
		}
		else if (CGravityScript::P_Up >= CGravityScript::T_Down && CGravityScript::T_Up >= CGravityScript::P_Down && CGravityScript::T_Up >= CGravityScript::P_Up && CGravityScript::T_Down <= CGravityScript::P_Down && CPlayerScript::m_IsJump)
		{
			CGravityScript::OverTerrain = true;
			return;
		}

		PushBackTrue = false;
		PushLeft = false;
		PushRight = false;
		isCanControl = true;

		PushForce = 700.f;
		
		CPlayerScript::Idle = 0.f;

		if (!DownJump)
		{
			PlayerTerrain = _pOther->GetGameObject();

			if (!leapTerrain)
			{
				if (Animator2D()->GetCurAnim()->GetName() == L"jump")
				{
					m_PlayerState = FSM::STAND;
				}
				else if (m_PlayerState == FSM::FLASHJUMP)
				{
					m_PlayerState = FSM::STAND_FLASH;
				}
			}			
		}
	}
	else if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		CGravityScript::P_Pos = (GetGameObject()->Transform()->GetLocalPos().y);
		CGravityScript::P_Scale = (GetGameObject()->Transform()->GetLocalScale().y);
		CGravityScript::P_CalOffSet = (GetGameObject()->Collider()->GetOffsetPos().y);
		CGravityScript::T_Up = (_pOther->GetGameObject()->Transform()->GetLocalPos().y) + (_pOther->GetGameObject()->Transform()->GetLocalScale().y) * 0.5f;
		CGravityScript::T_Down = (_pOther->GetGameObject()->Transform()->GetLocalPos().y) - (_pOther->GetGameObject()->Transform()->GetLocalScale().y) * 0.5f;
		CGravityScript::P_Down = ((CGravityScript::P_Pos + (CGravityScript::P_Scale* (GetGameObject()->Collider()->GetOffsetScale().y) * CGravityScript::P_CalOffSet))) - (CGravityScript::P_Scale * (GetGameObject()->Collider()->GetOffsetScale().y) * 0.5f);
		CGravityScript::P_Up = ((CGravityScript::P_Pos + (CGravityScript::P_Scale * (GetGameObject()->Collider()->GetOffsetScale().y) * CGravityScript::P_CalOffSet))) + (CGravityScript::P_Scale * (GetGameObject()->Collider()->GetOffsetScale().y) * 0.5f);

		if (CGravityScript::CGravityScript::P_Up >= CGravityScript::T_Down && CGravityScript::T_Up >= CGravityScript::P_Down && CPlayerScript::m_IsJump)
		{
			CGravityScript::OverTerrain = true;
			return;
		}
		else if (CGravityScript::P_Up >= CGravityScript::T_Up && CGravityScript::T_Down >= CGravityScript::P_Down - 10.f)
		{
			CGravityScript::OverTerrain = true;
		}
		else
		{
			CGravityScript::OverTerrain = false;
		}


		PushBackTrue = false;
		PushLeft = false;
		PushRight = false;
		isCanControl = true;

		PushForce = 700.f;

		CPlayerScript::Idle = 0.f;

		PlayerTerrain = _pOther->GetGameObject();

		if (!leapTerrain)
		{
			if (Animator2D()->GetCurAnim()->GetName() == L"jump")
			{
				m_PlayerState = FSM::STAND;
			}
			else if (m_PlayerState == FSM::FLASHJUMP)
			{
				m_PlayerState = FSM::STAND_FLASH;
			}
		}
	}

	if (_pOther->GetGameObject()->GetName() == L"LADDER")
	{
		//	if (vPos.y >= (_pOther->GetGameObject()->Transform()->GetLocalPos().y + _pOther->GetGameObject()->Transform()->GetLocalScale().y * 0.5f) + (Transform()->GetLocalScale().y * 0.5f * Collider()->GetOffsetScale().y - (Collider()->GetOffsetPos().y * Transform()->GetLocalScale().y)))

		if (KEYHOLD(KEY_TYPE::KEY_UP) && !HangUp)
		{
			if (m_PlayerState == FSM::FLASHJUMP)
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
			}
			m_PlayerState = FSM::LADDER;
			m_isHangOn = true;
		}

		else if (KEYHOLD(KEY_TYPE::KEY_DOWN))
		{
			if (m_PlayerState == FSM::FLASHJUMP)
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
			}
			m_PlayerState = FSM::LADDER;
			m_isHangOn = true;
		}

	}

	if (_pOther->GetGameObject()->GetName() == L"ROPE")
	{
		if (KEYHOLD(KEY_TYPE::KEY_UP) && !HangUp)
		{
			if (m_PlayerState == FSM::FLASHJUMP)
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
			}
			m_PlayerState = FSM::ROPE;
			m_isHangOn = true;
		}
		else if (KEYHOLD(KEY_TYPE::KEY_DOWN))
		{
			if (m_PlayerState == FSM::FLASHJUMP)
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
			}
			m_PlayerState = FSM::ROPE;
			m_isHangOn = true;
		}
	}

	if (_pOther->GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetIdx())
	{
		++CollisionCount;
	}
}

void CPlayerScript::OnCollision(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
	{
		if (!DownJump)
		{
			EffectYvalue = PlayerPos.y;
		}
	}
	else if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		EffectYvalue = PlayerPos.y;	
	}

	if (_pOther->GetGameObject()->GetName() == L"LADDER")
	{
		if (m_PlayerState == FSM::LADDER)
		{
			m_isHangOn = true;
		}
	}

	if (_pOther->GetGameObject()->GetName() == L"ROPE")
	{
		if (m_PlayerState == FSM::ROPE)
		{
			m_isHangOn = true;
		}
	}

	if (_pOther->GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetIdx())
	{
		if (_pOther->GetGameObject()->Confirm_Life())
		{
			if (CollisionCount != 0)
			{
				//InvincibilityAcc += DT;
				PushBackTrue = true;
			}
			//else
			//{
			//	//InvincibilityAcc += (DT / CollisionCount);
			//	PushBackTrue = true;
			//	MultipleObject = true;
			//}

			if (InvincibilityAcc < 1.f)
			{
				if (GetInvincibility() == false)
				{
					for (UINT i = 0; i < CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetObjectVec().size(); ++i)
					{
						if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetObjectVec()[i])
						{
							//	if (CollisionCount == 1)
							//if ((Transform()->GetLocalPos().x + Collider()->GetOffsetPos().x) < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->GetLocalPos().x)
							if ((_pOther->GetGameObject()->Transform()->GetLocalPos().x + _pOther->GetGameObject()->Collider()->GetOffsetPos().x) < Transform()->GetLocalPos().x)
							{
								PushRight = true;
								SetInvincibility(true);
							//	isCanControl = false;

								if (m_PlayerState == FSM::WALK)
								{
									m_PlayerState = FSM::STAND;
								}
							}
							else
							{
								PushLeft = true;
								SetInvincibility(true);
							//	isCanControl = false;

								if (m_PlayerState == FSM::WALK)
								{
									m_PlayerState = FSM::STAND;
								}
							}

						}
					}
				}

				return;
			}

			/*PushLeft = false;
			PushRight = false;*/

			/*UINT Random = Well512Random::Instance().GetValue(1, 100);

			if (Random % 2 == 0)
			{
				PushedLeft = false;
			}
			else
			{
				PushedLeft = true;
			}*/


			//	SetInvincibility(false);
			//	InvincibilityAcc = 0.f;
		}
	}
}

void CPlayerScript::OnCollisionExit(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
	{
//		float PlayerPos = GetGameObject()->Transform()->GetLocalPos().y;
//		float PlayerScale = GetGameObject()->Transform()->GetLocalScale().y;
//		float PlayerOffset = GetGameObject()->Collider()->GetOffsetPos().y;
//		if (DownJump && GetGameObject()->Transform()->GetLocalPos().y < _pOther->GetGameObject()->Transform()->GetLocalPos().y)
//		if (DownJump && PlayerPos + (PlayerScale * PlayerOffset) < _pOther->GetGameObject()->Transform()->GetLocalPos().y)
		if (DownJump && (GetGameObject()->Transform()->GetLocalPos().y) + ((GetGameObject()->Transform()->GetLocalScale().y) * (GetGameObject()->Collider()->GetOffsetPos().y)) < _pOther->GetGameObject()->Transform()->GetLocalPos().y)
		{
			DownJump = false;
		}

		TerrainIn = false;
		HangUp = false;
	}

	if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		//float PlayerPos = GetGameObject()->Transform()->GetLocalPos().y;
		//float PlayerScale = GetGameObject()->Transform()->GetLocalScale().y;
		//float PlayerOffset = GetGameObject()->Collider()->GetOffsetPos().y;
		//if (DownJump && GetGameObject()->Transform()->GetLocalPos().y < _pOther->GetGameObject()->Transform()->GetLocalPos().y)
		//if (DownJump && PlayerPos + (PlayerScale * PlayerOffset) < _pOther->GetGameObject()->Transform()->GetLocalPos().y)

		TerrainIn = false;
		HangUp = false;
	}

	if (_pOther->GetGameObject()->GetName() == L"LADDER")
	{
		if (KEYHOLD(KEY_TYPE::KEY_UP))
		{
			HangUp = true;

			m_PlayerState = FSM::STAND;
			SetLadderGravity(true);
			PlayerTerrain = nullptr;
		}

		else if (KEYHOLD(KEY_TYPE::KEY_DOWN))
		{
			m_PlayerState = FSM::STAND;
			SetLadderGravity(true);
			PlayerTerrain = nullptr;
		}

		m_isHangOn = false;
	}

	if (_pOther->GetGameObject()->GetName() == L"ROPE")
	{
		if (KEYHOLD(KEY_TYPE::KEY_UP))
		{
			HangUp = true;

			m_PlayerState = FSM::STAND;
			SetLadderGravity(true);
			PlayerTerrain = nullptr;
		}

		else if (KEYHOLD(KEY_TYPE::KEY_DOWN))
		{
			m_PlayerState = FSM::STAND;
			SetLadderGravity(true);
			PlayerTerrain = nullptr;
		}

		m_isHangOn = false;
	}

	if (_pOther->GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetLayer(L"MONSTER")->GetIdx())
	{
		--CollisionCount;
		//	SetInvincibility(false); 몬스터와 충돌 해제시 DT가 쌓이지 않는 문제를 해결하기 위해 몬스터와 충돌 나갈때 Bool 변수 추가로 UPdate에서 DT를 추가하는 방법 모색
	}
}





void CPlayerScript::PlayerBe_LEFT_Pushed()
{
	if (m_PlayerState == FSM::DEATHFAULT)
	{
		return;
	}

	if (FirstTimePush)
	{
		Idle = 0.5f;
		FirstTimePush = false;
	}

	Idle += DT;

	if (Idle < 0.5f)
	{
		return;
	}

	if (m_PlayerState == FSM::ROPE || m_PlayerState == FSM::LADDER)
	{
		m_PlayerState = FSM::STAND;

		m_isHangOn = false;

		SetLadderGravity(true);

		isCanControl = false;
	}
	else if (m_PlayerState == FSM::WALK)
	{
		m_PlayerState = FSM::STAND;
	}

	if (PushBackTrue)
	{
		vPos = Transform()->GetLocalPos();

		if (m_IsJump)
		{
			vPos.x -= 200 * DT;
			vPos.y += 35.f * DT;
			isCanControl = false;
		}
		else
		{
			vPos.x -= 170 * DT;
			vPos.y += (PushForce)* DT;
			PushForce -= 700.f * DT;
			isCanControl = false;
		}
		Transform()->SetLocalPos(vPos);
	}
}

void CPlayerScript::PlayerBe_RIGHT_Pushed()
{
	/*if (Pushing)
	{
		if (m_PlayerState == FSM::ROPE || m_PlayerState == FSM::LADDER)
		{
			m_PlayerState = FSM::STAND;

			m_isHangOn = false;

			SetLadderGravity(true);
			isCanControl = false;
		}
		else if (m_PlayerState == FSM::WALK)
		{
			m_PlayerState = FSM::STAND;
		}

		if (PushBackTrue)
		{
			vPos = Transform()->GetLocalPos();

			if (m_IsJump)
			{
				vPos.x += 200 * DT;
				vPos.y += 35.f * DT;
				isCanControl = false;
			}
			else
			{
				vPos.x += 170 * DT;
				vPos.y += (PushForce)* DT;
				PushForce -= 700.f * DT;
				isCanControl = false;
			}
			Transform()->SetLocalPos(vPos);

			if (TerrainIn)
			{
				Pushing = false;
			}
		}
	}
	else
	{
		Idle += DT;

		if (Idle < 0.5f)
		{
			return;
		}
		Pushing = true;
	}*/
	if (m_PlayerState == FSM::DEATHFAULT)
	{
		return;
	}

	if (FirstTimePush)
	{
		Idle = 0.5f;
		FirstTimePush = false;
	}

	Idle += DT;

	if (Idle < 0.5f)
	{
		return;
	}

	if (m_PlayerState == FSM::ROPE || m_PlayerState == FSM::LADDER)
	{
		m_PlayerState = FSM::STAND;

		m_isHangOn = false;

		SetLadderGravity(true);

		isCanControl = false;
	}
	else if (m_PlayerState == FSM::WALK)
	{
		m_PlayerState = FSM::STAND;
	}

	if (PushBackTrue)
	{
		vPos = Transform()->GetLocalPos();

		if (m_IsJump)
		{
			vPos.x += 200 * DT;
			vPos.y += 35.f * DT;
			isCanControl = false;
		}
		else
		{
			vPos.x += 170 * DT;
			vPos.y += (PushForce)* DT;
			PushForce -= 700.f * DT;
			isCanControl = false;
		}
		Transform()->SetLocalPos(vPos);
	}
}

void CPlayerScript::SetPlayerPos(Vec3 _SetPos)
{
	PlayerObject->Transform()->SetLocalPos(_SetPos);
}

void CPlayerScript::PushReset()
{
	if (FirstTimePush)
	{
		return;
	}

	static float resetCount = 0.f;

	if (CollisionCount == 0)
	{
		resetCount += 0.01f * DT;

		if (resetCount <= 0.01f)
		{
			return;
		}
		FirstTimePush = true;
		resetCount = 0.f;
	}
	else
	{
		resetCount = 0.f;
	}
}


void CPlayerScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

void CPlayerScript::state()
{
	switch (m_PlayerState)
	{
	case FSM::ALERT:
		CurDir();
		Animator2D()->PlayAnim(L"alert");
		break;

	case FSM::BLINK:
		CurDir();
		Animator2D()->PlayAnim(L"blink");
		break;

	case FSM::DEATH:
		CurDir();
		Animator2D()->PlayAnim(L"death");
		break;


	case FSM::JUMP:
		CurDir();
		Animator2D()->PlayAnim(L"jump");
		Attack();
		if (KEYTAB(KEY_TYPE::KEY_ALT))
		{
			m_IsJump;

			vPos = Transform()->GetLocalPos();

			m_PlayerState = FSM::FLASHJUMP;

			m_IsJump = true;
		}

		break;

	case FSM::FLASHJUMP:
		FlashAttack();

		break;

	case FSM::LADDER:
		m_IsJump = false;
		SetLadderGravity(false);
		SetLadderJumpForce(0.f);

		if (KEYHOLD(KEY_TYPE::KEY_UP))
		{
			Animator2D()->PlayAnim(L"ladder");
			Animator2D()->GetCurAnim()->SetAnimRepeat(true);
		}
		else if (KEYHOLD(KEY_TYPE::KEY_DOWN))
		{
			Animator2D()->PlayAnim(L"ladder");
			Animator2D()->GetCurAnim()->SetAnimRepeat(true);
		}
		else if (KEYAWAY(KEY_TYPE::KEY_UP))
		{
			Animator2D()->PlayAnim(L"ladder");
			Animator2D()->GetCurAnim()->SetAnimRepeat(false);
		}
		else if (KEYAWAY(KEY_TYPE::KEY_DOWN))
		{
			Animator2D()->PlayAnim(L"ladder");
			Animator2D()->GetCurAnim()->SetAnimRepeat(false);
		}
		break;

	case FSM::PRONE:
		Jump();
		CurDir();
		Animator2D()->PlayAnim(L"prone");
		break;

	case FSM::ROPE:
		m_IsJump = false;
		SetLadderGravity(false);
		SetLadderJumpForce(0.f);

		if (KEYHOLD(KEY_TYPE::KEY_UP))
		{
			Animator2D()->PlayAnim(L"rope");
			Animator2D()->GetCurAnim()->SetAnimRepeat(true);
		}
		else if (KEYHOLD(KEY_TYPE::KEY_DOWN))
		{
			Animator2D()->PlayAnim(L"rope");
			Animator2D()->GetCurAnim()->SetAnimRepeat(true);
		}
		else if (KEYAWAY(KEY_TYPE::KEY_UP))
		{
			Animator2D()->PlayAnim(L"rope");
			Animator2D()->GetCurAnim()->SetAnimRepeat(false);
		}
		else if (KEYAWAY(KEY_TYPE::KEY_DOWN))
		{
			Animator2D()->PlayAnim(L"rope");
			Animator2D()->GetCurAnim()->SetAnimRepeat(false);
		}

		break;

	case FSM::RUSH:
		CurDir();
		Animator2D()->PlayAnim(L"rush");
		break;

	case FSM::STAND:
		Jump();
		Attack();
		CurDir();
		Animator2D()->PlayAnim(L"stand");
		break;

	case FSM::STAND_FLASH:
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			m_PlayerState = FSM::STAND;
			Animator2D()->GetCurAnim()->SetFrmIdx(0);
		}
		break;

	case FSM::ATTACK:
		CurDir();

		switch (m_iSwingCount % 4)
		{
		case 0: Animator2D()->PlayAnim(L"swing");
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
				m_PlayerState = FSM::STAND;

				++m_iSwingCount;
				if (m_iSwingCount >= 10000)
				{
					m_iSwingCount = 0;
				}
			}

			break;

		case 1: Animator2D()->PlayAnim(L"swing2");
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
				m_PlayerState = FSM::STAND;
				++m_iSwingCount;
			}
			break;

		case 2: Animator2D()->PlayAnim(L"swing3");
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
				m_PlayerState = FSM::STAND;
				++m_iSwingCount;
			}
			break;

		case 3: Animator2D()->PlayAnim(L"swingF");
			if (Animator2D()->GetCurAnim()->IsFinish())
			{
				Animator2D()->GetCurAnim()->SetFrmIdx(0);
				m_PlayerState = FSM::STAND;
				++m_iSwingCount;
			}
			break;

		default:
			break;
		}

		break;


	case FSM::SHOUT:
		CurDir();
		Animator2D()->PlayAnim(L"alert");

		break;

	case FSM::SHADOWFLASH_ALPHA:
		CurDir();
		Animator2D()->PlayAnim(L"alert");

		break;

	case FSM::SHADOWFLASH_BETA:
		CurDir();
		Animator2D()->PlayAnim(L"alert");

		break;

	case FSM::DEATHFAULT:
		CurDir();
		Animator2D()->PlayAnim(L"deathfault");
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			Animator2D()->GetCurAnim()->SetFrmIdx(0);
			m_PlayerState = FSM::BUFF;
		}
		break;

	case FSM::BUFF:
		CurDir();
		Animator2D()->PlayAnim(L"casting");

		break;

	case FSM::WALK:
		Jump();
		CurDir();
		Animator2D()->PlayAnim(L"walk");

		break;

	case FSM::RAGINGBLOW:
		CurDir();
		Animator2D()->PlayAnim(L"ragingBlow");
		if (Animator2D()->GetCurAnim()->IsFinish())
		{
			Animator2D()->GetCurAnim()->SetFrmIdx(0);
			m_PlayerState = FSM::STAND;

			//			CInstinctScript::PlayerAction = true;

		}

	case FSM::END:
		break;

	default:
		break;
	}
}

void CPlayerScript::ChangeState()
{
	if (KEYTAB(KEY_TYPE::KEY_P))
	{
		vPos = Transform()->GetLocalPos();

		vPos.x = 0.f;
		vPos.y = 1000.f;
		Transform()->SetLocalPos(vPos);
	}

	ComboAttackSet();

	if (m_PlayerState == FSM::FLASHJUMP)
	{
		return;
	}

	if (m_PlayerState == FSM::DEATHFAULT)
	{
		return;
	}


	if (m_PlayerState == FSM::SHADOWFLASH_ALPHA || m_PlayerState == FSM::SHADOWFLASH_BETA)
	{
		return;
	}

	if (m_PlayerState == FSM::LADDER || m_PlayerState == FSM::ROPE)
	{
		if (m_isHangOn)
		{
			if (KEYHOLD(KEY_TYPE::KEY_UP))
			{
				vPos = Transform()->GetLocalPos();

				vPos.y += PlayerSpeed * DT;

				Transform()->SetLocalPos(vPos);
				return;
			}

			if (KEYHOLD(KEY_TYPE::KEY_DOWN))
			{
				vPos = Transform()->GetLocalPos();

				vPos.y -= PlayerSpeed * DT;

				Transform()->SetLocalPos(vPos);
				return;
			}

			if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
			{
				m_PlayerDir = DIR::RIGHT;
			}
			if (KEYHOLD(KEY_TYPE::KEY_LEFT))
			{
				m_PlayerDir = DIR::LEFT;
			}

		}


		if (KEYHOLD(KEY_TYPE::KEY_RIGHT) && KEYHOLD(KEY_TYPE::KEY_ALT) && KEYHOLD(KEY_TYPE::KEY_UP))
		{
			return;
		}

		if (KEYHOLD(KEY_TYPE::KEY_LEFT) && KEYHOLD(KEY_TYPE::KEY_ALT) && KEYHOLD(KEY_TYPE::KEY_UP))
		{
			return;
		}

		SetGravityInit();


		if (KEYHOLD(KEY_TYPE::KEY_RIGHT) && KEYHOLD(KEY_TYPE::KEY_ALT))
		{
			vPos = Transform()->GetLocalPos();

			vPos.x += PlayerSpeed * DT;
			m_PlayerState = FSM::JUMP;
			SetLadderGravity(true);
			SetLadderJumpForce(820.f);
			m_IsJump = true;

			if (!m_IsJump)
			{
				m_PlayerState = FSM::WALK;
			}

			Transform()->SetLocalPos(vPos);
			return;
		}

		if (KEYHOLD(KEY_TYPE::KEY_LEFT) && KEYHOLD(KEY_TYPE::KEY_ALT))
		{
			vPos = Transform()->GetLocalPos();

			if (!CAreaScript::AreaLeftCancel)
			{
				vPos.x -= PlayerSpeed * DT;
			}
			m_PlayerState = FSM::JUMP;
			SetLadderGravity(true);
			SetLadderJumpForce(820.f);
			m_IsJump = true;

			if (!m_IsJump)
			{
				m_PlayerState = FSM::WALK;
			}

			Transform()->SetLocalPos(vPos);
			return;
		}
	}

	if (m_PlayerState != FSM::LADDER && m_PlayerState != FSM::ROPE)
	{
		if (KEYTAB(KEY_TYPE::KEY_S) && m_PlayerState != FSM::BUFF && m_PlayerState != FSM::SHOUT  && m_PlayerState != FSM::DEATHFAULT && isCanControl)
		{
			if (CShoutScript::SHOUT_SkillState == SHOUT::ENABLE)
			{
				m_PlayerState = FSM::SHOUT;
			}
			return;
		}

		if (KEYTAB(KEY_TYPE::KEY_G) && m_PlayerState != FSM::BUFF && m_PlayerState != FSM::SHOUT  && m_PlayerState != FSM::DEATHFAULT && isCanControl)
		{
			if (CMapleWarriorScript::MAPLEWARRIOR_SkillState == MAPLEWARRIOR::ENABLE)
			{
				m_PlayerState = FSM::BUFF;
			}
			return;
		}

		if (KEYTAB(KEY_TYPE::KEY_W) && m_PlayerState != FSM::BUFF && isCanControl)
		{
			if (CBoosterScript::BOOSTER_SkillState == BOOSTER::ENABLE)
			{
				m_PlayerState = FSM::BUFF;
			}
			return;
		}

		if (KEYTAB(KEY_TYPE::KEY_F) && m_PlayerState != FSM::BUFF && isCanControl)
		{
			if (CInstinctActiveScript::INSTINCTACTIVE_SkillState == INSTINCT_FSM::ENABLE)
			{
				m_PlayerState = FSM::BUFF;
			}
			return;
		}


		if (m_PlayerState == FSM::STAND_FLASH)
		{
			if (TerrainIn)
			{
				if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
				{
					vPos = Transform()->GetLocalPos();

					if (!CAreaScript::AreaRightCancel)
					{
						vPos.x += PlayerSpeed * DT;
					}

					return;
				}
			}
		}

		if (m_PlayerState == FSM::STAND_FLASH)
		{
			if (TerrainIn)
			{
				if (KEYHOLD(KEY_TYPE::KEY_LEFT))
				{
					vPos = Transform()->GetLocalPos();
					
					if (!CAreaScript::AreaLeftCancel)
					{
						vPos.x -= PlayerSpeed * DT;
					}
					return;
				}
			}
		}



		if (m_PlayerState == FSM::BUFF)
		{
			if (TerrainIn)
			{
				if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
				{
					vPos = Transform()->GetLocalPos();

					if (!CAreaScript::AreaRightCancel)
					{
						vPos.x += PlayerSpeed * DT;
					}

					return;
				}
			}
		}

		if (m_PlayerState == FSM::BUFF)
		{
			if (TerrainIn)
			{
				if (KEYHOLD(KEY_TYPE::KEY_LEFT))
				{
					vPos = Transform()->GetLocalPos();

					if (!CAreaScript::AreaLeftCancel)
					{
						vPos.x -= PlayerSpeed * DT;
					}

					return;
				}
			}
		}


		if (m_PlayerState == FSM::SHOUT)
		{
			if (TerrainIn)
			{
				if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
				{
					vPos = Transform()->GetLocalPos();

					if (!CAreaScript::AreaRightCancel)
					{
						vPos.x += PlayerSpeed * DT;
					}

					return;
				}
			}
		}


		if (m_PlayerState == FSM::SHOUT)
		{
			if (TerrainIn)
			{
				if (KEYHOLD(KEY_TYPE::KEY_LEFT))
				{
					vPos = Transform()->GetLocalPos();

					if (!CAreaScript::AreaLeftCancel)
					{
						vPos.x -= PlayerSpeed * DT;
					}

					return;
				}
			}
		}


		if (m_PlayerState == FSM::RAGINGBLOW)
		{
			if (TerrainIn)
			{
				if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
				{
					vPos = Transform()->GetLocalPos();

					if (!CAreaScript::AreaRightCancel)
					{
						vPos.x += PlayerSpeed * DT;
					}

					return;
				}
			}
		}

		if (m_PlayerState == FSM::RAGINGBLOW)
		{
			if (TerrainIn)
			{
				if (KEYHOLD(KEY_TYPE::KEY_LEFT))
				{
					vPos = Transform()->GetLocalPos();

					if (!CAreaScript::AreaLeftCancel)
					{
						vPos.x -= PlayerSpeed * DT;
					}

					return;
				}
			}
		}



		if (m_PlayerState == FSM::ATTACK)
		{
			if (TerrainIn)
			{
				if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
				{
					vPos = Transform()->GetLocalPos();

					if (!CAreaScript::AreaRightCancel)
					{
						vPos.x += PlayerSpeed * DT;
					}

					return;
				}
			}
		}

		if (m_PlayerState == FSM::ATTACK)
		{
			if (TerrainIn)
			{
				if (KEYHOLD(KEY_TYPE::KEY_LEFT))
				{
					vPos = Transform()->GetLocalPos();

					if (!CAreaScript::AreaLeftCancel)
					{
						vPos.x -= PlayerSpeed * DT;
					}

					return;
				}
			}
		}

		if (isCanControl == true)
		{
			if (KEYHOLD(KEY_TYPE::KEY_RIGHT) && KEYHOLD(KEY_TYPE::KEY_LEFT) && m_IsJump == false)
			{
				m_PlayerState = FSM::STAND;
				return;
			}

			if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
			{
				if (m_IsJump && m_PlayerDir == DIR::LEFT)
				{
					m_PlayerDir = DIR::RIGHT;

					CurChange = true;

					return;
				}

				if (CurChange)
				{
					vPos = Transform()->GetLocalPos();
					if (!CAreaScript::AreaRightCancel)
					{
						vPos.x += 30.f * DT;
					}
					Transform()->SetLocalPos(vPos);

					m_bLeftAirWork = true;
					return;
				}

				vPos = Transform()->GetLocalPos();

				if (m_bAttackON && m_PlayerDir == DIR::RIGHT)
				{
					if (!CAreaScript::AreaRightCancel)
					{
						vPos.x += PlayerSpeed * DT;
					}
				}

				if (m_bAttackON == false)
				{
					if (!CAreaScript::AreaRightCancel)
					{
						vPos.x += PlayerSpeed * DT;
					}
					m_PlayerDir = DIR::RIGHT;
				}

				if (!m_bAttackON)
				{
					m_PlayerDir = DIR::RIGHT;
				}

				if (m_PlayerState == FSM::STAND_FLASH)
				{
					if (Animator2D()->GetCurAnim()->IsFinish())
					{
						if (m_PlayerState != FSM::ATTACK || m_PlayerState != FSM::RAGINGBLOW)
						{
							if (!m_IsJump)
							{
								m_PlayerState = FSM::WALK;
							}
						}
						if (KEYTAB(KEY_TYPE::KEY_CTRL))
						{
							if (!m_IsJump)
							{
								m_PlayerState = FSM::ATTACK;
							}
						}
						if (KEYTAB(KEY_TYPE::KEY_LSHIFT))
						{
							if (!m_IsJump)
							{
								m_PlayerState = FSM::RAGINGBLOW;
							}
						}
						Transform()->SetLocalPos(vPos);
						Animator2D()->GetCurAnim()->SetFrmIdx(0);
						return;
					}
				}
				//
				else if (m_PlayerState != FSM::ATTACK && m_PlayerState != FSM::RAGINGBLOW && m_PlayerState != FSM::SHOUT)
				{
					if (!m_IsJump)
					{
						m_PlayerState = FSM::WALK;
					}
				}
				if (KEYTAB(KEY_TYPE::KEY_CTRL))
				{
					if (!m_IsJump)
					{
						m_PlayerState = FSM::ATTACK;
					}
				}
				if (KEYTAB(KEY_TYPE::KEY_LSHIFT))
				{
					if (!m_IsJump)
					{
						m_PlayerState = FSM::RAGINGBLOW;
					}
				}

				Transform()->SetLocalPos(vPos);
				return;
			}
			else if (KEYAWAY(KEY_TYPE::KEY_RIGHT))
			{
				if (m_IsJump && m_PlayerDir == DIR::RIGHT)
				{
					m_bRightAirWork = true;
				}

				if (m_PlayerState != FSM::WALK)
				{
					if (!(Animator2D()->GetCurAnim()->IsFinish()))
					{
						return;
					}
				}
			}

			if (KEYAWAY(KEY_TYPE::KEY_RIGHT))
			{
				if (m_PlayerState != FSM::ATTACK)
				{
					if (!m_IsJump)
					{
						m_PlayerState = FSM::STAND;
					}
					return;
				}
			}

			if (KEYHOLD(KEY_TYPE::KEY_LEFT))
			{
				if (m_IsJump && m_PlayerDir == DIR::RIGHT)
				{
					m_PlayerDir = DIR::LEFT;

					CurChange = true;

					return;
				}

				if (CurChange)
				{
					vPos = Transform()->GetLocalPos();
				
					if (!CAreaScript::AreaLeftCancel)
					{
						vPos.x -= 30.f * DT;
					}

					Transform()->SetLocalPos(vPos);

					m_bRightAirWork = true;
					return;
				}

				vPos = Transform()->GetLocalPos();

				if (m_bAttackON && m_PlayerDir == DIR::LEFT)
				{
					if (!CAreaScript::AreaLeftCancel)
					{
						vPos.x -= PlayerSpeed * DT;
					}
				}


				if (m_bAttackON == false)
				{
					if (!CAreaScript::AreaLeftCancel)
					{
						vPos.x -= PlayerSpeed * DT;
					}

					m_PlayerDir = DIR::LEFT;
				}

				//
				if (m_PlayerState == FSM::STAND_FLASH)
				{
					if (Animator2D()->GetCurAnim()->IsFinish())
					{
						if (m_PlayerState != FSM::ATTACK)
						{
							if (!m_IsJump)
							{
								m_PlayerState = FSM::WALK;
							}
						}
						if (KEYTAB(KEY_TYPE::KEY_CTRL))
						{
							if (!m_IsJump)
							{
								m_PlayerState = FSM::ATTACK;
							}
						}
						if (KEYTAB(KEY_TYPE::KEY_LSHIFT))
						{
							if (!m_IsJump)
							{
								m_PlayerState = FSM::RAGINGBLOW;
							}
						}

						Transform()->SetLocalPos(vPos);
						Animator2D()->GetCurAnim()->SetFrmIdx(0);
						return;
					}
				}
				//
				else if (m_PlayerState != FSM::ATTACK && m_PlayerState != FSM::RAGINGBLOW && m_PlayerState != FSM::SHOUT)
				{
					if (!m_IsJump)
					{
						m_PlayerState = FSM::WALK;
					}
				}
				if (KEYTAB(KEY_TYPE::KEY_CTRL))
				{
					if (!m_IsJump)
					{
						m_PlayerState = FSM::ATTACK;
					}
				}
				if (KEYTAB(KEY_TYPE::KEY_LSHIFT))
				{
					if (!m_IsJump)
					{
						m_PlayerState = FSM::RAGINGBLOW;
					}
				}

				Transform()->SetLocalPos(vPos);
				return;
			}
			else if (KEYAWAY(KEY_TYPE::KEY_LEFT))
			{
				if (m_IsJump && m_PlayerDir == DIR::LEFT)
				{
					m_bLeftAirWork = true;
				}

				if (m_PlayerState != FSM::WALK)
				{
					if (!(Animator2D()->GetCurAnim()->IsFinish()))
					{
						return;
					}
				}
			}

			if (KEYAWAY(KEY_TYPE::KEY_LEFT))
			{
				if (m_PlayerState != FSM::ATTACK)
				{
					if (!m_IsJump)
					{
						m_PlayerState = FSM::STAND;
					}
					return;
				}
			}

			/*if (KEYAWAY(KEY_TYPE::KEY_LEFT))
			{
				if (!m_IsJump)
				{
					m_PlayerState = FSM::STAND;
				}
				return;
			}*/
		}

		if (KEYHOLD(KEY_TYPE::KEY_DOWN))
		{
			if ((!m_IsJump) && TerrainIn)
			{
				m_PlayerState = FSM::PRONE;
			}
			return;
		}
		if (KEYAWAY(KEY_TYPE::KEY_DOWN))
		{
			if (m_PlayerState != FSM::LADDER && m_PlayerState != FSM::ROPE && !m_IsJump)
			{
				m_PlayerState = FSM::STAND;
			}
			return;
		}
	}



	return;
}

void CPlayerScript::CurDir()
{
	if (m_PlayerState == FSM::ROPE || m_PlayerState == FSM::LADDER)
	{
		vRot = { 0.f, 0.f, 0.f };

		Transform()->SetLocalRot(vRot);
		return;
	}

	switch (m_PlayerDir)
	{
	case DIR::RIGHT:
		vRot = { 0.f, PI, 0.f };

		Transform()->SetLocalRot(vRot);
		break;

	case DIR::LEFT:
		vRot = { 0.f, 0.f, 0.f };

		Transform()->SetLocalRot(vRot);
		break;

	default:
		break;
	}
}

void CPlayerScript::SetLadderGravity(bool _GravitySet)
{
	CGravityScript::SetGravity(_GravitySet);
	m_fJumpForce = 0.f;
}

void CPlayerScript::SetLadderJumpForce(float _JumpForce)
{
	m_fJumpForce = _JumpForce;
}

void CPlayerScript::SetGravityInit()
{
	CGravityScript::SetGravityAcc();
}


int CPlayerScript::update()
{
	//if (KEYHOLD(KEY_TYPE::KEY_M) && CSceneMgr::m_bFadeIn)
	//{
	//	CSceneMgr::m_bFadeIn = false;//            Instinct active 수정 m버튼이 아닌 화면 전환용 포토샵 제작
	//}
	//else if (KEYHOLD(KEY_TYPE::KEY_M) && !CSceneMgr::m_bFadeIn)
	//{
	//	CSceneMgr::m_bFadeIn = true;
	//}

	if (KEYHOLD(KEY_TYPE::KEY_RIGHT))
	{
		CAreaScript::AreaLeftCancel = false;
	}
	if (KEYHOLD(KEY_TYPE::KEY_LEFT))
	{
		CAreaScript::AreaRightCancel = false;
	}

	CCollisionMgr::GetInst()->CollisionCheck(L"Default", L"MONSTER");
	CCollisionMgr::GetInst()->CollisionCheck(L"PLAYER", L"MONSTER");

	
	CPlayerScript::PlayerObject = GetGameObject();

	for (size_t i = 0; i < PlayerObject->GetScript().size(); ++i)
	{
		if (PlayerObject->GetScript()[i]->GetName() == L"PlayerScript")
		{
			PlayerToScript = PlayerObject->GetScript()[i];  //이부분 맞는지 체크 
		}
	}
	

	if (Animator2D()->GetCurAnim()->GetName() == L"swing" || Animator2D()->GetCurAnim()->GetName() == L"swing2" || Animator2D()->GetCurAnim()->GetName() == L"swing3" || Animator2D()->GetCurAnim()->GetName() == L"swingF" || Animator2D()->GetCurAnim()->GetName() == L"ragingBlow")
	{
		m_bAttackON = true;
	}
	else
	{
		m_bAttackON = false;
	}
	PushReset();

	ChangeState();
	CurDir();

	state();

	CPlayerScript::PlayerPos = GetGameObject()->Transform()->GetLocalPos();

	Prev_PlayerState = m_PlayerState;

	if (!m_IsJump)
	{
		m_bRightAirWork = false;
		m_bLeftAirWork = false;
		CurChange = false;

		RightJump = false;
		LeftJump = false;
		NormalJump = false;
	}

	if (NormalJump == false)
	{
		if (m_bRightAirWork && !CCraneScript::CraneAction)
		{
			vPos = Transform()->GetLocalPos();
			
			if (!CAreaScript::AreaRightCancel)
			{
				vPos.x += 80.f * DT;
			}
			Transform()->SetLocalPos(vPos);
		}

		if (m_bLeftAirWork && !CCraneScript::CraneAction)
		{
			vPos = Transform()->GetLocalPos();
			if (!CAreaScript::AreaLeftCancel)
			{
				vPos.x -= 80.f * DT;
			}
			Transform()->SetLocalPos(vPos);
		}
	}



	if (m_IsJump)
	{
		vPos = Transform()->GetLocalPos();
		vPos.y += m_fJumpForce * DT;
		Transform()->SetLocalPos(vPos);

		if (0.f < m_fJumpForce)
		{
			m_fJumpForce -= 1000.f * DT;
		}
	}


	if (m_PlayerState == FSM::FLASHJUMP && !CCraneScript::CraneAction)
	{
		vPos = Transform()->GetLocalPos();
		vPos.y += (m_fJumpForce * DT * 0.5f);
		m_fJumpForce -= 1000.f * DT;

		if (m_PlayerDir == DIR::RIGHT)
		{
			if (!CAreaScript::AreaRightCancel)
			{
				vPos.x += m_fSpeed * DT * 0.65f;
			}

			if (m_fSpeed - m_fAccSpeed * DT >= 0)
			{
				m_fSpeed -= m_fAccSpeed * DT;
			}
		}
		else if (m_PlayerDir == DIR::LEFT)
		{
			if (!CAreaScript::AreaLeftCancel)
			{
				vPos.x -= m_fSpeed * DT * 0.65f;
			}

			if (m_fSpeed - m_fAccSpeed * DT >= 0)
			{
				m_fSpeed -= m_fAccSpeed * DT;
			}
		}

		Transform()->SetLocalPos(vPos);
	}

	if (PushLeft)
	{
		if (!CCraneScript::CraneAction)
		{
			PlayerBe_LEFT_Pushed();
		}
	}
	else if (PushRight)
	{
		if (!CCraneScript::CraneAction)
		{
			PlayerBe_RIGHT_Pushed();
		}
	}


	if (m_IsJump)
	{
		if (vPos.y < PrevPos.y)
		{
			leapTerrain = false;
		}
		else
		{
			leapTerrain = true;
		}
	}
	else
	{
		leapTerrain = false;
	}

	SetPrevPos(vPos);

	if (PushBackTrue)
	{
		InvincibilityAcc += (DT / CollisionCount);

		if (1.f < InvincibilityAcc)
		{
			UINT Random = Well512Random::Instance().GetValue(1, 100);

			if (Random % 2 == 0)
			{
				PushedLeft = false;
			}
			else
			{
				PushedLeft = true;
			}

			SetInvincibility(false);
			InvincibilityAcc = 0.f;
		}
	}

	return 0;
}
