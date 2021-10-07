#include "stdafx.h"
#include "WireScript.h"

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

#include "PlayerScript.h"

#include "ScriptMgr.h"
#include "QuestMgr.h"

#include "PapulatusScript.h"

CWireScript::CWireScript()
	: Script_start(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::WIRESCRIPT);
}


CWireScript::~CWireScript()
{
}




void CWireScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

void CWireScript::SkillPos()
{
	vPos = Transform()->GetLocalPos();

	float AREARINGPOS = startPos.y - 59.f;
	float CranePos = GetGameObject()->GetParent()->Transform()->GetLocalPos().y;

	vPos.y = (AREARINGPOS - CranePos) * 0.5f + CranePos + 59.f;
	vPos.z = CPlayerScript::PlayerPos.z + 1.f;
	Transform()->SetLocalPos(vPos);


	vScale = Transform()->GetLocalScale();
	vScale.y = (startPos.y - vPos.y) * 2.f + 15.f;
	Transform()->SetLocalScale(vScale);
}


void CWireScript::saveStartPos()
{
	if (Script_start)
	{
		startPos = Transform()->GetLocalPos();
		Script_start = false;
	}
}


int CWireScript::update()
{
	saveStartPos();

	SkillPos();

	return 0;
}