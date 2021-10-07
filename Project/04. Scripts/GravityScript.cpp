#include "stdafx.h"
#include "GravityScript.h"

#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"

#include "ScriptMgr.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Collider.h"
#include "AnrealEngine/Component.h"
#include "AnrealEngine/Layer.h"

#include "AnrealEngine/CollisionMgr.h"

#include "PlayerScript.h"
#include "CraneScript.h"

bool CGravityScript::m_bOnGravity = true;
bool CGravityScript::OverTerrain = false;

float CGravityScript::m_fGravityAcc = 98.f * 5.f;
float CGravityScript::m_fGravity = m_fGravityAcc;
float CGravityScript::T_Up = 0.f;
float CGravityScript::T_Down = 0.f;
float CGravityScript::T_Left = 0.f;
float CGravityScript::T_Right = 0.f;
float CGravityScript::P_Up = 0.f;
float CGravityScript::P_Down = 0.f;
float CGravityScript::P_Scale = 0.f;
float CGravityScript::P_Pos = 0.f;
float CGravityScript::P_CalOffSet = 0.f;

CGravityScript::CGravityScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::GRAVITYSCRIPT);
}


CGravityScript::~CGravityScript()
{
}


void CGravityScript::OnCollisionEnter(CCollider * _pOther)
{
	//	for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN")->GetObjectVec().size(); ++i)

		//	if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN")->GetObjectVec()[i])

	if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
	{
		if (OverTerrain)
		{
			return;
		}

		if (CPlayerScript::DownJump)
		{
			m_bOnGravity = true;

			m_fGravity = m_fGravityAcc;

			CPlayerScript::m_IsJump = false;
			CPlayerScript::m_fSpeed = 1500.f;
			CPlayerScript::m_fJumpForce = 1000.f;
			CPlayerScript::TerrainIn = true;
		}
		else
		{
			m_bOnGravity = false;
		}

		if (CPlayerScript::leapTerrain)
		{
			m_bOnGravity = false;
		}
		else
		{		
			m_fGravity = m_fGravityAcc;

			CPlayerScript::m_IsJump = false;
			CPlayerScript::m_fSpeed = 1500.f;
			CPlayerScript::m_fJumpForce = 1000.f;
			CPlayerScript::TerrainIn = true;			
		}
	}
	else if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		m_bOnGravity = false;

		if (CPlayerScript::leapTerrain)
		{
			m_bOnGravity = false;
		}
		else
		{
			m_fGravity = m_fGravityAcc;

			CPlayerScript::m_IsJump = false;
			CPlayerScript::m_fSpeed = 1500.f;
			CPlayerScript::m_fJumpForce = 1000.f;
			CPlayerScript::TerrainIn = true;
		}
	}
}

void CGravityScript::OnCollision(CCollider * _pOther)
{
//	for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN")->GetObjectVec().size(); ++i)
	{
	//	if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN")->GetObjectVec()[i])
		if ((_pOther->GetGameObject()->GetName() == L"TERRAIN") && !m_bOnGravity)
		{
			if (OverTerrain)
			{
				return;
			}

			if (m_bOnGravity == true)
			{
				m_bOnGravity = false;
			}

			if (CPlayerScript::leapTerrain == false)
			{
				if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
				{
					if (CPlayerScript::DownJump == true)
					{
						m_bOnGravity = true;
						return;
					}
				}

				if (!(CPlayerScript::m_isHangOn))
				{
					CPlayerScript::TerrainIn = true;
					m_bOnGravity = false;

					m_TerrainvPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
					m_TerrainvRot = _pOther->GetGameObject()->Transform()->GetLocalRot();
					m_TerrainvScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

					m_fSlope = tan(m_TerrainvRot.z);	//기울기
					m_fY = (m_TerrainvScale.x * 0.5f) * sin(m_TerrainvRot.z);	//변해야 하는 y값 


					float fData = m_fSlope * m_fY / m_TerrainvScale.y;

					if (m_fSlope == 0.f)
					{
						fData = m_TerrainvPos.y + m_TerrainvScale.y * 0.5f + (Transform()->GetLocalScale().y * 0.5f * Collider()->GetOffsetScale().y) - (Collider()->GetOffsetPos().y * Transform()->GetLocalScale().y);
							//	터레인의 중심부터 표면 끝까지의 거리		//플레이어의 콜라이더 세로 길이												// 플레이어의 Offset 적용					

						m_ObjectPos = Transform()->GetLocalPos();

						m_ObjectPos.y = fData;

						Transform()->SetLocalPos(m_ObjectPos);

						return;
					}

					m_ObjectPos = Transform()->GetLocalPos();

					m_ObjectPos.y -= fData;

					Transform()->SetLocalPos(m_ObjectPos);
				}
			}
		}
		else if ((_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN"))
		{
			if (m_bOnGravity == true)
			{
				m_bOnGravity = false;
			}

			if (CPlayerScript::leapTerrain == false)
			{
				if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
				{
					if (CPlayerScript::DownJump == true)
					{
						m_bOnGravity = true;
						return;
					}
				}

				if (!(CPlayerScript::m_isHangOn))
				{
					CPlayerScript::TerrainIn = true;
					m_bOnGravity = false;

					m_TerrainvPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
					m_TerrainvRot = _pOther->GetGameObject()->Transform()->GetLocalRot();
					m_TerrainvScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

					m_fSlope = tan(m_TerrainvRot.z);	//기울기
					m_fY = (m_TerrainvScale.x * 0.5f) * sin(m_TerrainvRot.z);	//변해야 하는 y값 


					float fData = m_fSlope * m_fY / m_TerrainvScale.y;

					if (m_fSlope == 0.f)
					{
						fData = m_TerrainvPos.y + m_TerrainvScale.y * 0.5f + (Transform()->GetLocalScale().y * 0.5f * Collider()->GetOffsetScale().y) - (Collider()->GetOffsetPos().y * Transform()->GetLocalScale().y);
						//	터레인의 중심부터 표면 끝까지의 거리		//플레이어의 콜라이더 세로 길이												// 플레이어의 Offset 적용					

						m_ObjectPos = Transform()->GetLocalPos();

						m_ObjectPos.y = fData;

						Transform()->SetLocalPos(m_ObjectPos);

						return;
					}

					m_ObjectPos = Transform()->GetLocalPos();

					m_ObjectPos.y -= fData;

					Transform()->SetLocalPos(m_ObjectPos);
				}
			}
		}
	}
}



void CGravityScript::OnCollisionExit(CCollider * _pOther)
{
	if ((_pOther->GetGameObject()->GetName() == L"TERRAIN") || (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN"))
	{
		if (CCraneScript::CraneAction)
		{
			return;
		}
		m_bOnGravity = true;

		OverTerrain = false;
	}
}

void CGravityScript::awake()
{

}

int CGravityScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"PLAYER", L"TERRAIN");
	if (m_bOnGravity)
	{
		m_ObjectPos = Transform()->GetLocalPos();

		m_ObjectPos.y -= m_fGravity * DT;
		/*if (m_fGravity + m_fGravityAcc * DT >= m_fMaxGravity)
		{
			m_fGravity = m_fMaxGravity;
			Transform()->SetLocalPos(m_ObjectPos);
			return 0;
		}*/

		if (m_fGravity <= 700.f)
		{
			m_fGravity += m_fGravityAcc * DT;
		}


		Transform()->SetLocalPos(m_ObjectPos);
	}

	return 0;
}