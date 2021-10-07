#include "stdafx.h"

#include "CamCollScript.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Camera.h"
#include "Transform.h"

#include "ScriptMgr.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/Layer.h"

#include "AnrealEngine/Collider.h"
#include "AnrealEngine/CollisionMgr.h"

#include "ShoutScript.h"

#include "PlayerScript.h"

CCamCollScript::CCamCollScript() :
	m_pCamTarget(nullptr),
	iscollision(false),
	prevCollision(false),
	CameraOn(true),
	PlusY(0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::CAMCOLLSCRIPT);
}

CCamCollScript::~CCamCollScript()
{
}

void CCamCollScript::awake()
{
}

//	vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
//	vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

int CCamCollScript::update()
{
	if (CameraOn)
	{
		CCollisionMgr::GetInst()->CollisionCheck(L"CAM_COLL", L"AREA");
		CCollisionMgr::GetInst()->CollisionCheck(L"CAM_COLL", L"AREAUP");

		if (CSceneMgr::GetInst()->GetCurScene()->GetSceneName() == L"Papulatus_Scene.scene")
		{
			PlusY = 60.f;
		}
		else
		{
			PlusY = 150.f;
		}

		if (!iscollision)
		{
			vPos.x = CPlayerScript::GetPlayerPos().x;
			vPos.x = Lerf(PrevPos.x, vPos.x, 1.1f * DT);
			Transform()->SetLocalPos(vPos);
		}
		if (!isUpcollision)
		{
			vPos.y = CPlayerScript::GetPlayerPos().y + PlusY;
			vPos.y = Lerf(PrevPos.y, vPos.y, 1.1f * DT);
			Transform()->SetLocalPos(vPos);
		}

		PrevPos = vPos;
	}
	return 0;
}


void CCamCollScript::OnCollisionEnter(CCollider * _pOther)
{
	for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetObjectVec().size(); ++i)
	{
		if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetObjectVec()[i])
		{
			iscollision = true;
		}
	}

	for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREAUP")->GetObjectVec().size(); ++i)
	{
		if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREAUP")->GetObjectVec()[i])
		{
			isUpcollision = true;
		}
	}

}

void CCamCollScript::OnCollision(CCollider * _pOther)
{
	PlayerPos = CPlayerScript::GetPlayerPos();

	for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetObjectVec().size(); ++i)
	{
		if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetObjectVec()[i])
		{
			m_TerrainvPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
			m_TerrainvScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

			vPos = Transform()->GetLocalPos();

			if (m_TerrainvPos.x >= Transform()->GetLocalPos().x)		// 현재 카메라와 부딪힌 녀석이 만약 오른쪽 벽이라면.
			{
				fData = m_TerrainvPos.x - m_TerrainvScale.x * 0.5f - Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x * 0.5f;
				// 카메라의 고정 x값 (움직이지 않게하기 위해) 은 오른쪽 벽의 위치에서 두께의 절반만큼 빼기.
				vPos.x = fData;


				Transform()->SetLocalPos(vPos);

				if (((m_TerrainvPos.x - m_TerrainvScale.x * 0.5f) - PlayerPos.x) > 1920.f * Collider()->GetOffsetScale().x * 0.5f)
				{
					iscollision = false;
				}

				return;
			}

			if (m_TerrainvPos.x <= Transform()->GetLocalPos().x)
			{
				fData = m_TerrainvPos.x + m_TerrainvScale.x * 0.5f + Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x * 0.5f;

				vPos.x = fData;

				Transform()->SetLocalPos(vPos);

				if (PlayerPos.x - (m_TerrainvPos.x + m_TerrainvScale.x * 0.5f) > 1920.f * Collider()->GetOffsetScale().x * 0.5f)
				{
					iscollision = false;
				}

				return;
			}
		}
	}





	for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREAUP")->GetObjectVec().size(); ++i)
	{
		if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREAUP")->GetObjectVec()[i])
		{
			m_TerrainvPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
			m_TerrainvScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

			vPos = Transform()->GetLocalPos();

			if (m_TerrainvPos.y < Transform()->GetLocalPos().y)
			{
				fData = m_TerrainvPos.y + m_TerrainvScale.y * 0.5f + Transform()->GetLocalScale().y * Collider()->GetOffsetScale().x * 0.5f;

				vPos.y = fData;

				Transform()->SetLocalPos(vPos);

				if (PlayerPos.y - (m_TerrainvPos.y + m_TerrainvScale.y * 0.5f) > 1080.f * Collider()->GetOffsetScale().x * 0.5f)
				{
					isUpcollision = false;
				}

				return;


			}

			if (m_TerrainvPos.y > Transform()->GetLocalPos().y)
			{
				fData = m_TerrainvPos.y - m_TerrainvScale.y * 0.5f - Transform()->GetLocalScale().y * Collider()->GetOffsetScale().x * 0.5f;

				vPos.y = fData;

				Transform()->SetLocalPos(vPos);

				if ((m_TerrainvPos.y - m_TerrainvScale.y * 0.5f) - PlayerPos.y > 1080.f * Collider()->GetOffsetScale().x * 0.5f + PlusY)
				{
					isUpcollision = false;
				}

				return;
			}
		}
	}
}

void CCamCollScript::OnCollisionExit(CCollider * _pOther)
{
	for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetObjectVec().size(); ++i)
	{
		if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetObjectVec()[i])
		{
			iscollision = false;
		}
	}

	for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREAUP")->GetObjectVec().size(); ++i)
	{
		if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREAUP")->GetObjectVec()[i])
		{
			isUpcollision = false;
		}
	}
}
