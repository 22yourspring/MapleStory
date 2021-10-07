#include "stdafx.h"
#include "UICamScript.h"
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

CUICamScript::CUICamScript()
	: m_pCamTarget(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::UICAMSCRIPT);
}

CUICamScript::~CUICamScript()
{
}

void CUICamScript::awake()
{
}

int CUICamScript::update()
{
	Camera()->SetScale(0.85f);
	return 0;
}


void CUICamScript::holdPosition()
{
	vPos = { 0.f, 0.f, 0.f };
	Transform()->SetLocalPos(vPos);
}

void CUICamScript::OnCollisionEnter(CCollider * _pOther)
{

}

void CUICamScript::OnCollision(CCollider * _pOther)
{
	
}

void CUICamScript::OnCollisionExit(CCollider * _pOther)
{
	
}
