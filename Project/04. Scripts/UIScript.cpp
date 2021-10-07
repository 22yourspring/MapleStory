#include "stdafx.h"
#include "UIScript.h"

#include "AnrealEngine/CollisionMgr.h"
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

#include "AreaScript.h"
#include "ScriptMgr.h"

#include "MouseScript.h"

CUIScript::CUIScript()
	: HasItem(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::UISCRIPT);
}


CUIScript::~CUIScript()
{
}




void CUIScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}


void CUIScript::ProcessPos()
{
	vPos = Transform()->GetLocalPos();

	if (GetGameObject()->GetName() == L"EXP")
	{
		vPos.y -= 459;
		vPos.y += Transform()->GetLocalScale().y * 0.5f;
	}
	else if (GetGameObject()->GetName() == L"QuickSlot")
	{

	}
	else if (GetGameObject()->GetName() == L"Inventory")
	{
		if (CMouseScript::CurUIMoving)
		{
			vPos += CMouseScript::CurMoving - CMouseScript::PrevMousePos;
			vPos.z = 10.f;
		}
	}
	else if (GetGameObject()->GetName() == L"Item_Socket1")
	{
		vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();
		vPos.x -= 60.f;
		vPos.y += 101.f;
		vPos.z = 0.f;
	}
	else if (GetGameObject()->GetName() == L"Item_Socket2")
	{
		vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();
		vPos.x -= 24.f;
		vPos.y += 101.f;
		vPos.z = 0.f;
	}
	else if (GetGameObject()->GetName() == L"Inventory_Inner")
	{
		vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();
		vPos.y -= 4.f;
		vPos.z = 0.f;
	}
	Transform()->SetLocalPos(vPos);
}

void CUIScript::SetVisible()
{
	if (GetGameObject()->GetName() == L"Inventory" && GetGameObject()->GetActive())
	{
		if (KEYTAB(KEY_TYPE::KEY_I))
		{
			Animator2D()->PlayAnim(L"blink");
			GetGameObject()->SetActive(false);
		}
	}
	else if (GetGameObject()->GetName() == L"Inventory" && !GetGameObject()->GetActive())
	{
		if (KEYTAB(KEY_TYPE::KEY_I)) 
		{
			Animator2D()->PlayAnim(L"Inven_Mim");
			GetGameObject()->SetActive(true);
		}
	}
}


int CUIScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"UI", L"UI");

	ProcessPos();
	SetVisible();

	if (GetGameObject()->GetChild().size() != 0)
	{
		HasItem = true;
	}
	else
	{
		HasItem = false;
	}

	return 0;
}