#include "stdafx.h"
#include "LadderScript.h"

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

CLadderScript::CLadderScript()
	: GS(nullptr)
{
	SetScriptType((UINT)SCRIPT_TYPE::LADDERSCRIPT);
}


CLadderScript::~CLadderScript()
{
}


void CLadderScript::OnCollisionEnter(CCollider * _pOther)
{
	
}

void CLadderScript::OnCollision(CCollider * _pOther)
{
	/*for (UINT i = 0; i < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"LADDER")->GetObjectVec().size(); ++i)
	{
		if (_pOther->GetGameObject() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"LADDER")->GetObjectVec()[i])
		{
			m_TerrainvPos = _pOther->GetGameObject()->Transform()->GetLocalPos();
			m_TerrainvScale = _pOther->GetGameObject()->Transform()->GetLocalScale();

			m_ObjectPos = Transform()->GetLocalPos();
			if (KEYHOLD(KEY_TYPE::KEY_UP) || KEYHOLD(KEY_TYPE::KEY_DOWN))
			{
				m_isHangOn = true;
				
				if (m_TerrainvPos.x <= m_ObjectPos.x)
				{
					m_ObjectPos.x = m_TerrainvPos.x;
					Transform()->SetLocalPos(m_ObjectPos);

				}
				else if (m_TerrainvPos.x >= m_ObjectPos.x)
				{
					m_ObjectPos.x = m_TerrainvPos.x;

					Transform()->SetLocalPos(m_ObjectPos);
				}
				
				for (UINT j = 0; j < CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->GetScript().size(); ++j)
				{
					if ((SCRIPT_TYPE)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->GetScript()[j]->GetScriptType()) == SCRIPT_TYPE::PLAYERSCRIPT)
					{
						PS = (CPlayerScript*)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"PLAYER")->GetObjectVec()[0]->GetScript()[j]);
						PlayerState = PS->GetPlayerState();
						if (PlayerState == FSM::LADDER)
						{
							if (KEYHOLD(KEY_TYPE::KEY_UP))
							{
								m_ObjectPos = Transform()->GetLocalPos();
								m_ObjectPos.y += 300.f * DT;

								Transform()->SetLocalPos(m_ObjectPos);
							}

							else if (KEYHOLD(KEY_TYPE::KEY_DOWN))
							{
								m_ObjectPos = Transform()->GetLocalPos();
								m_ObjectPos.y -= 300.f * DT;

								Transform()->SetLocalPos(m_ObjectPos);

							}
						}					
					}
				}
			}
		}
	}*/
}



void CLadderScript::OnCollisionExit(CCollider * _pOther)
{
	
}

void CLadderScript::SetGravity(bool	_SetGravity)
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

void CLadderScript::awake()
{

}

int CLadderScript::update()
{
	CCollisionMgr::GetInst()->CollisionCheck(L"PLAYER", L"LADDER");

	

	m_ObjectPos = Transform()->GetLocalPos();
	Transform()->SetLocalPos(m_ObjectPos);

	return 0;
}