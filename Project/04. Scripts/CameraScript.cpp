#include "stdafx.h"

#include "CameraScript.h"
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

float CCameraScript::CamScale = 0.f;

CCameraScript::CCameraScript() : 
	m_pCamTarget (nullptr),
	iscollision (false),
	prevCollision (false),
	CameraOn (true),
	PlusY(0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::CAMERASCRIPT);
}

CCameraScript::~CCameraScript()
{
}

void CCameraScript::awake()
{
}

	//	vFront = Transform()->GetWorldDir(DIR_TYPE::FRONT);
	//	vRight = Transform()->GetWorldDir(DIR_TYPE::RIGHT);

int CCameraScript::update()
{
	CamScale = GetCamScale();
	Camera()->SetScale(0.85f);
	if (KEYTAB(KEY_TYPE::KEY_8))
	{
		if (CameraOn)
		{
			CameraOn = false;
		}
		else
		{
			CameraOn = true;
		}
	}

	if (KEYTAB(KEY_TYPE::KEY_7) && CameraOn == false)
	{
	//	vPos = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->Transform()->GetLocalPos();
		vPos = CPlayerScript::GetPlayerPos();
		
		
		Transform()->SetLocalPos(vPos);
		Camera()->SetScale(0.85f);
	}

	float fScale = Camera()->GetScale();
	if (KEYHOLD(KEY_TYPE::KEY_9))
	{
		fScale += DT * 0.3f;
	}
	if (KEYHOLD(KEY_TYPE::KEY_0))
	{
		fScale -= DT * 0.3f;
	}
	Camera()->SetScale(fScale);

	if (CameraOn)
	{
		/*float fScale = Camera()->GetScale();
		if (KEYHOLD(KEY_TYPE::KEY_9))
		{
			fScale += DT * 0.3f;
		}
		if (KEYHOLD(KEY_TYPE::KEY_0))
		{
			fScale -= DT * 0.3f;
		}
		Camera()->SetScale(fScale);*/


		CCollisionMgr::GetInst()->CollisionCheck(L"Camera", L"AREA");
		CCollisionMgr::GetInst()->CollisionCheck(L"Camera", L"AREAUP");

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



float CCameraScript::GetCamScale()
{
	return Camera()->GetScale();
}


void CCameraScript::OnCollisionEnter(CCollider * _pOther)
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

void CCameraScript::OnCollision(CCollider * _pOther)
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
				fData = m_TerrainvPos.x - m_TerrainvScale.x * 0.5f - Transform()->GetLocalScale().x * Camera()->GetScale() * 0.5f;
			// 카메라의 고정 x값 (움직이지 않게하기 위해) 은 오른쪽 벽의 위치에서 두께의 절반만큼 빼기.
				vPos.x = fData;
				

				Transform()->SetLocalPos(vPos);

				if (((m_TerrainvPos.x - m_TerrainvScale.x * 0.5f) - PlayerPos.x) > 1920.f * 0.5f)
				{
					iscollision = false;
				}

				return;
			}

			if (m_TerrainvPos.x <= Transform()->GetLocalPos().x)
			{
				fData = m_TerrainvPos.x + m_TerrainvScale.x * 0.5f + Transform()->GetLocalScale().x * Camera()->GetScale() * 0.5f;

				vPos.x = fData;

				Transform()->SetLocalPos(vPos);

				if (PlayerPos.x - (m_TerrainvPos.x + m_TerrainvScale.x * 0.5f) > 1920.f * 0.5f)
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
				fData = m_TerrainvPos.y + m_TerrainvScale.y * 0.5f + Transform()->GetLocalScale().y * Camera()->GetScale() * 0.5f;

				vPos.y = fData;

				Transform()->SetLocalPos(vPos);

				if (PlayerPos.y - (m_TerrainvPos.y + m_TerrainvScale.y * 0.5f) > 1080.f * 0.5f)
				{
					isUpcollision = false;
				}

				return;


			}

			if (m_TerrainvPos.y > Transform()->GetLocalPos().y)
			{
				fData = m_TerrainvPos.y - m_TerrainvScale.y * 0.5f - Transform()->GetLocalScale().y * Camera()->GetScale() * 0.5f;
				
				vPos.y = fData;

				Transform()->SetLocalPos(vPos);

				if ((m_TerrainvPos.y - m_TerrainvScale.y * 0.5f) - PlayerPos.y > 1080.f * 0.5f + PlusY)
				{
					isUpcollision = false;
				}

				return;
			}
		}
	}
}

void CCameraScript::OnCollisionExit(CCollider * _pOther)
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
