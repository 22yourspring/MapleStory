#include "stdafx.h"
#include "FingerScript.h"

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

#include "AnrealEngine/Core.h"
#include "CameraScript.h"

#include "AnrealEngine/Device.h"
#include "MouseScript.h"

CFingerScript::CFingerScript()
	:vPos(0.f, 0.f, 0.f)
{
	SetScriptType((UINT)SCRIPT_TYPE::FINGERSCRIPT);
}


CFingerScript::~CFingerScript()
{
}




void CFingerScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}


void CFingerScript::ProcessPos()
{
	vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();
	vPos.x += Transform()->GetLocalScale().x * 0.5f - 3.5f;
	vPos.y -= Transform()->GetLocalScale().y * 0.5f;
	vPos.y += 4.f;
	Transform()->SetLocalPos(vPos);
}

void CFingerScript::state()
{
	switch (CMouseScript::Finger)
	{
	case CURSOR::CLICKED:
		Animator2D()->PlayAnim(L"clicked");
		break;
	case CURSOR::CLICK_IDLE:
		Animator2D()->PlayAnim(L"ClickIdle");
		break;
	case CURSOR::IDLE:
		Animator2D()->PlayAnim(L"Idle");
		break;
	case CURSOR::IVENTORY_IDLE:
		Animator2D()->PlayAnim(L"InventoryIdle");
		break;
	case CURSOR::SCROLL_IDLE:
		Animator2D()->PlayAnim(L"ScrollIdle");
		break;
	case CURSOR::TAKEIT:
		Animator2D()->PlayAnim(L"Takeit");
		break;
	default:
		break;
	}
}


int CFingerScript::update()
{
	ProcessPos();

	if (CMouseScript::Finger != CMouseScript::PREV_Figner)
	{
		state();
	}
	CMouseScript::PREV_Figner = CMouseScript::Finger;
	return 0;
}