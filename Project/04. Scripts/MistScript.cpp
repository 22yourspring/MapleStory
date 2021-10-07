#include "stdafx.h"
#include "MistScript.h"

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

#include "CameraScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

CMistScript::CMistScript()
	: MIST_STATE(0)
	, Mist_Identifier(0)
	, MistIdentifierChecker(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::MISTSCRIPT);
}

CMistScript::~CMistScript()
{
}


void CMistScript::awake()
{
}

void CMistScript::MistCheck()
{
	if (MistIdentifierChecker)
	{
		if (GetGameObject()->GetName() == L"CANVAS_IMAGE1" || GetGameObject()->GetName() == L"Canvas_Image1")
		{
			Mist_Identifier = 0;
		}
		else if (GetGameObject()->GetName() == L"CANVAS_IMAGE2" || GetGameObject()->GetName() == L"Canvas_Image2")
		{
			Mist_Identifier = 1;
		}
		MistIdentifierChecker = false;
	}
}


void CMistScript::Skillstate()
{
	if (Mist_Identifier == 0)
	{
		Animator2D()->PlayAnim(L"MIST_1");
	}
	else if (Mist_Identifier == 1)
	{
		Animator2D()->PlayAnim(L"MIST_2");
	}
}

void CMistScript::SkillPos()
{
	if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"CAM_COLL") == nullptr)
	{
		return;
	}
	CameraPos = CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"CAM_COLL")->GetObjectVec()[0]->Transform()->GetLocalPos();

	vPos = Transform()->GetLocalPos();
	vPos.y += DT * 4.f;
	vPos.x = CameraPos.x * 0.08f - 0.4f;

	Transform()->SetLocalPos(vPos);
	return;
}

int CMistScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"CANVAS", L"AREAUP");

	MistCheck();
	SkillPos();

	if (MIST_STATE != PREV_MIST_STATE)
	{
		Skillstate();
	}
	PREV_MIST_STATE = MIST_STATE;

	return 0;
}

void CMistScript::OnCollisionEnter(CCollider * _pOther)
{
}

void CMistScript::OnCollision(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"AREAUP" || _pOther->GetGameObject()->GetName() == L"Area Up")
	{
		vPos = Transform()->GetLocalPos();
		
		if (vPos.y >= _pOther->GetGameObject()->Transform()->GetLocalPos().y)
		{
			if (Mist_Identifier == 0)
			{
				vPos = GetGameObject()->GetParent()->GetChild()[1]->Transform()->GetLocalPos();
			}
			else if (Mist_Identifier == 1)
			{
				vPos = GetGameObject()->GetParent()->GetChild()[0]->Transform()->GetLocalPos();
			}

			vPos.y -= 600.f;

			Transform()->SetLocalPos(vPos);
		}
	}
}

void CMistScript::OnCollisionExit(CCollider * _pOther)
{
}
