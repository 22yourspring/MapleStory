#include "stdafx.h"
#include "PlayerColScript.h"

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

CPlayerColScript::CPlayerColScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::PLAYERCOLSCRIPT);
}

CPlayerColScript::~CPlayerColScript()
{
}

void CPlayerColScript::awake()
{
}


int CPlayerColScript::update()
{
	SkillPos();
	return 0;
}

void CPlayerColScript::SkillPos()
{
	vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();

	Transform()->SetLocalPos(vPos);
}
