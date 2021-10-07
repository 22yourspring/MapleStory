#include "stdafx.h"
#include "MouseScript.h"

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
#include "UIScript.h"
#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/Layer.h"

CURSOR CMouseScript::Finger = CURSOR::IDLE;
CURSOR CMouseScript::PREV_Figner = CURSOR::END;

Vec3 CMouseScript::CurMoving = { 0.f, 0.f, 0.f };
Vec3 CMouseScript::PrevMousePos = { 0.f, 0.f, 0.f };

bool CMouseScript::CurUIMoving = false;

CMouseScript::CMouseScript()
	: vPos(0.f, 0.f, 0.f)
	, UISelect(false)
	, ItemSelect(false)
	, SelectItem(nullptr)
	, PrevSocket(nullptr)
	, NextSelect(nullptr)
	, MouseUp(true)
{
	SetScriptType((UINT)SCRIPT_TYPE::MOUSESCRIPT);
}


CMouseScript::~CMouseScript()
{
}




void CMouseScript::awake()
{
	m_pSharedMtrl = MeshRender()->GetSharedMaterial();
	m_pCloneMtrl = MeshRender()->GetCloneMaterial();
}


void CMouseScript::ProcessPos()
{
	POINT ptPos = {};
	GetCursorPos(&ptPos);

	ScreenToClient(CCore::GetInst()->GetWndFocus(), &ptPos);

	/*vPos.x = (float)(ptPos.x - 960);
	vPos.y = (float)(-(ptPos.y - 540));
	vPos.z = 0.;*/


	RECT ptPos2;
	GetClientRect(CCore::GetInst()->GetWndFocus(), &ptPos2);

	tResolution tRect = CDevice::GetInst()->GetResolution();

	Vec2 temp1;
	
	temp1.x = tRect.x / ptPos2.right;
	temp1.y = tRect.y / ptPos2.bottom;


	//vPos.x = (ptPos.x - ptPos2.right * 0.5f) * CCameraScript::CamScale;

	/*vPos.x = (ptPos.x - (1920 * 0.5f)) * CCameraScript::CamScale;
	vPos.y = -(ptPos.y - (1080 * 0.5f)) * CCameraScript::CamScale;
	vPos.z = 0;*/

	vPos.x = (ptPos.x - ptPos2.right * 0.5f) * CCameraScript::CamScale * temp1.x;
	vPos.y = (-(ptPos.y - ptPos2.bottom * 0.5f)) * CCameraScript::CamScale * temp1.y;
	vPos.z = 0;

	Transform()->SetLocalPos(vPos);
}


void CMouseScript::CursorChange()
{
	if (KEYHOLD(KEY_TYPE::KEY_LBTN))
	{
		Finger = CURSOR::CLICKED;
	}
	if (KEYAWAY(KEY_TYPE::KEY_LBTN))
	{
		Finger = CURSOR::IDLE;
	}
}

void CMouseScript::UIMoving()
{
//	if (CurUIMoving)
	{
		CurMoving = Transform()->GetLocalPos();
	}
}

void CMouseScript::UIrelease()
{
	if (KEYAWAY(KEY_TYPE::KEY_LBTN))
	{
		CurUIMoving = false;
	}
}

void CMouseScript::OnCollisionEnter(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"Inventory_Inner")
	{
		UISelect = true;
	}
}

void CMouseScript::OnCollision(CCollider * _pOther)
{
	if (KEYAWAY(KEY_TYPE::KEY_LBTN))
	{
		MouseUp = true;
	}

	if (_pOther->GetGameObject()->GetName() == L"Inventory_Inner")
	{
		UISelect = true;
		PrevMousePos = Transform()->GetLocalPos();
	}

	if (_pOther->GetGameObject()->GetName() == L"Inventory" && !UISelect)
	{
		if (KEYHOLD(KEY_TYPE::KEY_LBTN))
		{
			CurUIMoving = true;
			PrevMousePos = Transform()->GetLocalPos();
		}
	}

	if (_pOther->GetGameObject()->GetName() == L"Item_Socket1")
	{
		if (KEYAWAY(KEY_TYPE::KEY_LBTN))
		{
			MouseUp = true;
		}

		Finger = CURSOR::IVENTORY_IDLE;
		if (((CUIScript*)(_pOther->GetGameObject()->GetScript()[0]))->HasItem && !ItemSelect && MouseUp)
		{
			if (KEYTAB(KEY_TYPE::KEY_LBTN))
			{
				Finger = CURSOR::TAKEIT;
				ItemSelect = true;

				PrevSocket = _pOther->GetGameObject();
				SelectItem = _pOther->GetGameObject()->GetChild()[0];
				/*SelectItem->DisconnectParent_To_be_child();*/

				MouseUp = false;

				return;
			}
		}
		else if (((CUIScript*)(_pOther->GetGameObject()->GetScript()[0]))->HasItem && ItemSelect && MouseUp)
		{
			if (KEYTAB(KEY_TYPE::KEY_LBTN))
			{
				Finger = CURSOR::IVENTORY_IDLE;

				if (_pOther->GetGameObject() == PrevSocket)
				{
					return;
				}

				if (_pOther->GetGameObject()->GetChild().size() != 0)
				{
					NextSelect = _pOther->GetGameObject()->GetChild()[0];
					NextSelect->DisconnectParent_To_be_child();
					
					SelectItem->DisconnectParent_To_be_child();
					PrevSocket->AddChild(NextSelect);


					_pOther->GetGameObject()->AddChild(SelectItem);

					SelectItem = nullptr;
					PrevSocket = nullptr;
					NextSelect = nullptr;

					ItemSelect = false;

					MouseUp = false;
				
				}

				return;
			}
		}
		/*else if (!((CUIScript*)(_pOther->GetGameObject()->GetScript()[0]))->HasItem && !ItemSelect && MouseUp)
		{
			Finger = CURSOR::IVENTORY_IDLE;
			ItemSelect = false;		

			MouseUp = false;
			return;
		}*/
		else if (!((CUIScript*)(_pOther->GetGameObject()->GetScript()[0]))->HasItem && ItemSelect && MouseUp)
		{
			if (KEYTAB(KEY_TYPE::KEY_LBTN))
			{
				Finger = CURSOR::IVENTORY_IDLE;
				_pOther->GetGameObject()->AddChild(SelectItem);
				SelectItem = nullptr;
				ItemSelect = false;

				MouseUp = false;

				return;
			}
		}
	}
	if (_pOther->GetGameObject()->GetName() == L"Item_Socket2")
	{
		if (KEYAWAY(KEY_TYPE::KEY_LBTN))
		{
			MouseUp = true;
		}

		Finger = CURSOR::IVENTORY_IDLE;
		if (((CUIScript*)(_pOther->GetGameObject()->GetScript()[0]))->HasItem && !ItemSelect && MouseUp)
		{
			if (KEYTAB(KEY_TYPE::KEY_LBTN))
			{
				Finger = CURSOR::TAKEIT;
				ItemSelect = true;

				PrevSocket = _pOther->GetGameObject();
				SelectItem = _pOther->GetGameObject()->GetChild()[0];
				/*SelectItem->DisconnectParent_To_be_child();*/

				MouseUp = false;

				return;
			}
		}
		else if (((CUIScript*)(_pOther->GetGameObject()->GetScript()[0]))->HasItem && ItemSelect && MouseUp)
		{
			if (KEYTAB(KEY_TYPE::KEY_LBTN))
			{
				Finger = CURSOR::IVENTORY_IDLE;

				if (_pOther->GetGameObject() == PrevSocket)
				{
					return;
				}

				if (_pOther->GetGameObject()->GetChild().size() != 0)
				{
					NextSelect = _pOther->GetGameObject()->GetChild()[0];
					NextSelect->DisconnectParent_To_be_child();

					SelectItem->DisconnectParent_To_be_child();
					PrevSocket->AddChild(NextSelect);


					_pOther->GetGameObject()->AddChild(SelectItem);

					SelectItem = nullptr;
					PrevSocket = nullptr;
					NextSelect = nullptr;

					ItemSelect = false;

					MouseUp = false;

				}

				return;
			}
		}
		/*else if (!((CUIScript*)(_pOther->GetGameObject()->GetScript()[0]))->HasItem && !ItemSelect && MouseUp)
		{
			Finger = CURSOR::IVENTORY_IDLE;
			ItemSelect = false;

			MouseUp = false;
			return;
		}*/
		else if (!((CUIScript*)(_pOther->GetGameObject()->GetScript()[0]))->HasItem && ItemSelect && MouseUp)
		{
			if (KEYTAB(KEY_TYPE::KEY_LBTN))
			{
				Finger = CURSOR::IVENTORY_IDLE;
				_pOther->GetGameObject()->AddChild(SelectItem);
				SelectItem = nullptr;
				ItemSelect = false;

				MouseUp = false;

				return;
			}
		}
	}
}

void CMouseScript::OnCollisionExit(CCollider * _pOther)
{
	if (_pOther->GetGameObject()->GetName() == L"Inventory_Inner")
	{
		UISelect = false;
	}

	if (_pOther->GetGameObject()->GetName() == L"Item_Socket1")
	{
		Finger = CURSOR::IDLE;
	}
	if (_pOther->GetGameObject()->GetName() == L"Item_Socket2")
	{
		Finger = CURSOR::IDLE;
	}

}

int CMouseScript::update()
{	
	CursorChange();
	ProcessPos();
	UIMoving();
	UIrelease();

	return 0;
}