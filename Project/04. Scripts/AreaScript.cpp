#include "stdafx.h"
#include "AreaScript.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Transform.h"

#include "ScriptMgr.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Collider.h"
#include "AnrealEngine/Component.h"
#include "AnrealEngine/Layer.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/Script.h"

#include "AnrealEngine/CollisionMgr.h"

#include "PlayerScript.h"

Vec3 CAreaScript::FallenPos = { 0.f, 0.f, 0.f };
Vec3 CAreaScript::AreaScale = { 0.f, 0.f, 0.f };

bool CAreaScript::AreaLeftCancel = false;
bool CAreaScript::AreaRightCancel = false;

CAreaScript::CAreaScript()
	: m_ObjectPos(0.f, 0.f, 0.f)
	, m_ObjectScale(0.f, 0.f, 0.f)
	, AreaPos(0.f, 0.f, 0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::AREASCRIPT);
}


CAreaScript::~CAreaScript()
{
}


void CAreaScript::OnCollisionEnter(CCollider * _pOther)
{
	if (GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetIdx())
	{
		for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetObjectVec().size(); ++i)
		{
			if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetObjectVec()[i])
			{
				m_TerrainvPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
				m_TerrainvScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

				m_ObjectPos = Transform()->GetLocalPos();

				if (m_TerrainvPos.x >= Transform()->GetLocalPos().x)
				{
					float fData = m_TerrainvPos.x - m_TerrainvScale.x * 0.5f - Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x * 0.5f;

					m_ObjectPos.x = fData - 18.f;

					Transform()->SetLocalPos(m_ObjectPos);

					AreaRightCancel = true;

					return;
				}

				if (m_TerrainvPos.x <= Transform()->GetLocalPos().x)
				{
					float fData = m_TerrainvPos.x + m_TerrainvScale.x * 0.5f + Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x * 0.5f;

					m_ObjectPos.x = fData + 18.f;;

					Transform()->SetLocalPos(m_ObjectPos);

					AreaLeftCancel = true;

					return;
				}

				m_ObjectPos = Transform()->GetLocalPos();
				Transform()->SetLocalPos(m_ObjectPos);
			}
		}
	}
}

void CAreaScript::OnCollision(CCollider * _pOther)
{
	if (GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetIdx())
	{
		for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetObjectVec().size(); ++i)
		{
			if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetObjectVec()[i])
			{
				m_TerrainvPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
				m_TerrainvScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

				m_ObjectPos = Transform()->GetLocalPos();

				if (m_TerrainvPos.x >= Transform()->GetLocalPos().x)
				{
					float fData = m_TerrainvPos.x - m_TerrainvScale.x * 0.5f - Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x * 0.5f;

					m_ObjectPos.x = fData - 18.f;

					Transform()->SetLocalPos(m_ObjectPos);

					AreaRightCancel = true;

					return;
				}

				if (m_TerrainvPos.x <= Transform()->GetLocalPos().x)
				{
					float fData = m_TerrainvPos.x + m_TerrainvScale.x * 0.5f + Transform()->GetLocalScale().x * Collider()->GetOffsetScale().x * 0.5f;

					m_ObjectPos.x = fData + 18.f;;

					Transform()->SetLocalPos(m_ObjectPos);

					AreaLeftCancel = true;

					return;
				}

				m_ObjectPos = Transform()->GetLocalPos();
				Transform()->SetLocalPos(m_ObjectPos);
			}
		}
	}
}

void CAreaScript::OnCollisionExit(CCollider * _pOther)
{
}

void CAreaScript::awake()
{

}

int CAreaScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"PLAYER", L"AREA");

	if (GetGameObject()->GetName() == L"Player")
	{
		m_ObjectPos = Transform()->GetLocalPos();
		Transform()->SetLocalPos(m_ObjectPos);
	}
	else
	{
		if (CAreaScript::AreaPos == (Vec3)(0.f, 0.f, 0.f))
		{
			if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"CANVAS") != nullptr)
			{
				for (size_t i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"CANVAS")->GetObjectVec().size(); ++i)
				{
					if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"CANVAS")->GetObjectVec()[i]->GetName() == L"Canvas")
					{
						AreaPos = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"CANVAS")->GetObjectVec()[i]->Transform()->GetLocalPos();
						AreaScale = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"CANVAS")->GetObjectVec()[i]->Transform()->GetLocalScale();
					}
				}
			}

			if (GetGameObject()->GetName() == L"Area Left")
			{
				m_ObjectPos = AreaPos;
				m_ObjectPos.x = AreaPos.x - (AreaScale.x * 0.5f) - (GetGameObject()->Transform()->GetLocalScale().x * 0.5f);

				Transform()->SetLocalPos(m_ObjectPos);

				m_ObjectScale = GetGameObject()->Transform()->GetLocalScale();
				m_ObjectScale.y = AreaScale.y;

				Transform()->SetLocalScale(m_ObjectScale);
			}
			if (GetGameObject()->GetName() == L"Area Right")
			{
				m_ObjectPos = AreaPos;
				m_ObjectPos.x = AreaPos.x + (AreaScale.x * 0.5f) + (GetGameObject()->Transform()->GetLocalScale().x * 0.5f);

				Transform()->SetLocalPos(m_ObjectPos);

				m_ObjectScale = GetGameObject()->Transform()->GetLocalScale();
				m_ObjectScale.y = AreaScale.y;

				Transform()->SetLocalScale(m_ObjectScale);
			}
			if (GetGameObject()->GetName() == L"Area Up")
			{
				m_ObjectPos = AreaPos;
				m_ObjectPos.y = AreaPos.y + (AreaScale.y * 0.5f) + (GetGameObject()->Transform()->GetLocalScale().y * 0.5f);

				Transform()->SetLocalPos(m_ObjectPos);

				m_ObjectScale = GetGameObject()->Transform()->GetLocalScale();
				m_ObjectScale.x = AreaScale.x;

				Transform()->SetLocalScale(m_ObjectScale);

				FallenPos = m_ObjectPos;
			}
			if (GetGameObject()->GetName() == L"Area Down")
			{
				m_ObjectPos = AreaPos;
				m_ObjectPos.y = AreaPos.y - (AreaScale.y * 0.5f) - (GetGameObject()->Transform()->GetLocalScale().y * 0.5f);

				Transform()->SetLocalPos(m_ObjectPos);

				m_ObjectScale = GetGameObject()->Transform()->GetLocalScale();
				m_ObjectScale.x = AreaScale.x;
				m_ObjectScale.y = 100.f;

				Transform()->SetLocalScale(m_ObjectScale);
			}
		}
	}
	//else if (GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREA")->GetIdx())
	//{
	//	if (GetGameObject()->GetName() == L"AREALEFT")
	//	{
	//	//	GetGameObject()->Transform()->SetLocalPos = ;
	//	}
	//	else if (GetGameObject()->GetName() == L"AREARIGHT")
	//	{

	//	}
	//}
	//else if (GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"AREAUP")->GetIdx())
	//{

	//}

	return 0;
}