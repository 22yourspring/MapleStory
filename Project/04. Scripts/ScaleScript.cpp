#include "stdafx.h"
#include "ScaleScript.h"

#include "TimeMgr.h"
#include "Transform.h"
#include "ScriptMgr.h"

#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"
#include "AnrealEngine/Texture.h"


CScaleScript::CScaleScript()
{
	SetScriptType((UINT)SCRIPT_TYPE::SCALESCRIPT);
}

CScaleScript::~CScaleScript()
{
}

int CScaleScript::update()
{
	Vec3 m_applyFC;

	map<wstring, CAnimation2D*>& mapAnim = Animator2D()->GetAnimMap();

	int CurFrm = Animator2D()->GetCurAnim()->GetCurFrm();
	
	
	m_applyFC.x = Animator2D()->GetCurAnim()->GetpTex(CurFrm)->GetWidth();
	m_applyFC.y = Animator2D()->GetCurAnim()->GetpTex(CurFrm)->GetHeight();
	m_applyFC.z = 100.f;
	
	GetGameObject()->Transform()->SetLocalScale(m_applyFC);


	return 0;
}


