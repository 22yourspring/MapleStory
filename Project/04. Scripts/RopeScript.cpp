#include "stdafx.h"
#include "RopeScript.h"

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
#include "GravityScript.h"

CRopeScript::CRopeScript()
	: GS(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::ROPESCRIPT);
}


CRopeScript::~CRopeScript()
{
}


void CRopeScript::OnCollisionEnter(CCollider * _pOther)
{
	
}

void CRopeScript::OnCollision(CCollider * _pOther)
{
	
}



void CRopeScript::OnCollisionExit(CCollider * _pOther)
{
	
}

void CRopeScript::SetGravity(bool _SetGravity)
{
	if (!m_isHangOn)
	{
		return;
	}

	if (GS == nullptr)
	{
		for (UINT i = 0; i < CPlayerScript::PlayerObject->GetScript().size(); ++i)
		{
			if ((SCRIPT_TYPE)(CPlayerScript::PlayerObject->GetScript()[i]->GetScriptType()) == SCRIPT_TYPE::GRAVITYSCRIPT)
			{
				GS = (CGravityScript*)(CPlayerScript::PlayerObject->GetScript()[i]);
			}
		}
	}
	GS->SetGravity(_SetGravity);
	
}

void CRopeScript::awake()
{

}

int CRopeScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"PLAYER", L"ROPE");

	

	m_ObjectPos = Transform()->GetLocalPos();
	Transform()->SetLocalPos(m_ObjectPos);

	return 0;
}