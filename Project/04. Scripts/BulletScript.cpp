#include "stdafx.h"
#include "BulletScript.h"

#include "TimeMgr.h"
#include "Transform.h"
#include "ScriptMgr.h"




CBulletScript::CBulletScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::BULLETSCRIPT);
}

CBulletScript::~CBulletScript()
{
}

int CBulletScript::update()
{
	Vec3 temp;

	temp = Transform()->GetLocalPos();

	temp.y += 2.0f * DT;

	Transform()->SetLocalPos(temp);

	return 0;
}


