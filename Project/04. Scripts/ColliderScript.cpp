#include "stdafx.h"
#include "ColliderScript.h"

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

#include "ScriptMgr.h"
#include "QuestMgr.h"

CColliderScript::CColliderScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::COLLIDERSCRIPT);
}

CColliderScript::~CColliderScript()
{
}

void CColliderScript::awake()
{
}


int CColliderScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"LEFT_LASER", L"PLAYER");
	CCollisionMgr::GetInst()->CollisionCheck(L"RIGHT_LASER", L"PLAYER");

	SkillPos();
	return 0;
}

void CColliderScript::SkillPos()
{
	vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();

	Transform()->SetLocalPos(vPos);
}
