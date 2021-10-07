#include "stdafx.h"
#include "ItemScript.h"

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

CItemScript::CItemScript()
	: m_bSelect(false)
{
	SetScriptType((UINT)SCRIPT_TYPE::ITEMSCRIPT);
}


CItemScript::~CItemScript()
{
}



void CItemScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}

void CItemScript::Skillstate()
{
	/*switch (ItemType)
	{
	case ITEM_TYPE::ELIXIR:
		switch (OBJECT_State)
		{
		case ITEM_STATE::DROP:
			break;
		case ITEM_STATE::INVENTORY:
			break;
		case ITEM_STATE::MONSTER:
			break;
		case ITEM_STATE::END:
			break;
		default:
			break;
		}
		break;
	case ITEM_TYPE::VIKING:
		switch (OBJECT_State)
		{
		case ITEM_STATE::DROP:
			break;
		case ITEM_STATE::INVENTORY:
			break;
		case ITEM_STATE::MONSTER:
			break;
		case ITEM_STATE::END:
			break;
		default:
			break;
		}
		break;
	case ITEM_TYPE::KEEPER:
		switch (OBJECT_State)
		{
		case ITEM_STATE::DROP:
			break;
		case ITEM_STATE::INVENTORY:
			break;
		case ITEM_STATE::MONSTER:
			break;
		case ITEM_STATE::END:
			break;
		default:
			break;
		}
		break;
	case ITEM_TYPE::G_WATCH:
		switch (OBJECT_State)
		{
		case ITEM_STATE::DROP:
			break;
		case ITEM_STATE::INVENTORY:
			break;
		case ITEM_STATE::MONSTER:
			break;
		case ITEM_STATE::END:
			break;
		default:
			break;
		}
		break;
	case ITEM_TYPE::CRACK:
		switch (OBJECT_State)
		{
		case ITEM_STATE::DROP:
			break;
		case ITEM_STATE::INVENTORY:
			break;
		case ITEM_STATE::MONSTER:
			break;
		case ITEM_STATE::END:
			break;
		default:
			break;
		}
		break;
	case ITEM_TYPE::END:
		break;
	default:
		break;
	}*/
}

void CItemScript::SkillPos()
{
	if (GetGameObject()->GetParent() != nullptr)
	{
		vPos = GetGameObject()->GetParent()->Transform()->GetLocalPos();
		Transform()->SetLocalPos(vPos);
	}
}


void CItemScript::SkillChangeState()
{
	
	return;
}


int CItemScript::update()
{
	if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"ITEM") != nullptr)
	{
		CCollisionMgr::GetInst()->CollisionCheck(L"ITEM", L"TERRAIN");
		CCollisionMgr::GetInst()->CollisionCheck(L"ITEM", L"PLAYER");
	}

	SkillPos();
	SkillChangeState();

	if (PREV_OBJECTState != OBJECT_State)
	{
		Skillstate();
	}
	PREV_OBJECTState = OBJECT_State;

	return 0;
}
