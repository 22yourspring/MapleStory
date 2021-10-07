#include "stdafx.h"
#include "MobGravityScript.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"

#include "ScriptMgr.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Collider.h"
#include "AnrealEngine/Component.h"
#include "AnrealEngine/Layer.h"

#include "AnrealEngine/CollisionMgr.h"
#include "PapulatusScript.h"


CMobGravityScript::CMobGravityScript() :
	m_bOnGravity(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::MOBGRAVITYSCRIPT);
}


CMobGravityScript::~CMobGravityScript()
{
}


void CMobGravityScript::OnCollisionEnter(CCollider * _pOther)
{
	/*for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN")->GetObjectVec().size(); ++i)
	{
		if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN")->GetObjectVec()[i])
		{
			SetGravity(false);

			m_fGravity = m_fGravityAcc;
		}
	}*/

	if (GetGameObject()->GetName() == L"Papulatus_ALTER_EGO")
	{
		return;
	}

	if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
	{
		if (GetGameObject()->GetName() == L"Papulatus" || GetGameObject()->GetName() == L"Papulatus_ALTER_EGO")
		{
			return;
		}
		SetGravity(false);

		m_fGravity = m_fGravityAcc;
	}
	else if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		SetGravity(false);

		m_fGravity = m_fGravityAcc;
	}
}

void CMobGravityScript::OnCollision(CCollider * _pOther)
{
	if (GetGameObject()->GetName() == L"Papulatus_ALTER_EGO")
	{
		return;
	}

	if (_pOther->GetGameObject()->GetName() == L"TERRAIN")
	{
		if (GetGameObject()->GetName() == L"Papulatus" || GetGameObject()->GetName() == L"Papulatus_ALTER_EGO")
		{
			return;
		}
		SetGravity(false);

		m_fGravity = m_fGravityAcc;

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
	else if (_pOther->GetGameObject()->GetName() == L"LAST_TERRAIN")
	{
		SetGravity(false);

		m_fGravity = m_fGravityAcc;

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

	//for (size_t i = 0; i < GetGameObject()->GetScript().size(); ++i)
	//{
	//	if (GetGameObject()->GetScript()[i]->GetScriptType() != (UINT)SCRIPT_TYPE::MOBGRAVITYSCRIPT)
	//	{
	//		if (GetGameObject()->GetScript()[i]->Script_TERRAIN == _pOther->GetGameObject())
	//		{
	//			SetGravity(false);

	//			m_TerrainvPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
	//			m_TerrainvRot = _pOther->GetGameObject()->Transform()->GetLocalRot();
	//			m_TerrainvScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

	//			m_fSlope = tan(m_TerrainvRot.z);	//기울기
	//			m_fY = (m_TerrainvScale.x * 0.5f) * sin(m_TerrainvRot.z);	//변해야 하는 y값 


	//			float fData = m_fSlope * m_fY / m_TerrainvScale.y;

	//			if (m_fSlope == 0.f)
	//			{
	//				fData = m_TerrainvPos.y + m_TerrainvScale.y * 0.5f + (Transform()->GetLocalScale().y * 0.5f * Collider()->GetOffsetScale().y) - (Collider()->GetOffsetPos().y * Transform()->GetLocalScale().y);
	//				//	터레인의 중심부터 표면 끝까지의 거리		//플레이어의 콜라이더 세로 길이												// 플레이어의 Offset 적용					

	//				m_ObjectPos = Transform()->GetLocalPos();

	//				m_ObjectPos.y = fData;

	//				Transform()->SetLocalPos(m_ObjectPos);

	//				return;
	//			}

	//			m_ObjectPos = Transform()->GetLocalPos();

	//			m_ObjectPos.y -= fData;

	//			Transform()->SetLocalPos(m_ObjectPos);
	//		}
	//	}
	//}
}



void CMobGravityScript::OnCollisionExit(CCollider * _pOther)
{
	if (GetGameObject()->GetName() == L"Papulatus_ALTER_EGO")
	{
		return;
	}

	for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN")->GetObjectVec().size(); ++i)
	{
		if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"TERRAIN")->GetObjectVec()[i])
		{
			SetGravity(true);
		}
	}
}

void CMobGravityScript::awake()
{

}

int CMobGravityScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"MONSTER", L"TERRAIN");

	if (GetGameObject()->GetName() == L"Papulatus_ALTER_EGO")
	{
		return 0;
	}

	if (GetGameObject()->GetName() == L"Papulatus")
	{
		if (CPapulatusScript::Phase2ReverseGravity)
		{
			SetGravity(false);
		}
	}

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