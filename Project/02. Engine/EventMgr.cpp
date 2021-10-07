#include "EventMgr.h"

#include "SceneMgr.h"
#include "GameObject.h"

CEventMgr::CEventMgr()
{
}

CEventMgr::~CEventMgr()
{
}

int CEventMgr::update()
{	
	// 삭제예정 오브젝트 처리	
	for (UINT i = 0; i < m_vecDelObj.size(); ++i)
	{
		m_vecDelObj[i]->Isolate();
		SAFE_DELETE(m_vecDelObj[i]);
	}
	m_vecDelObj.clear();
	
	// 등록된 이벤트 처리
	for (UINT i = 0; i < m_vecEvent.size(); ++i)
	{
		Execute(m_vecEvent[i]);
	}
	m_vecEventCopy = m_vecEvent;
	m_vecEvent.clear();
	
	return 0;
}

void CEventMgr::Execute(const tEvent & _event)
{
	switch (_event.eType)
	{
	case EVENT_TYPE::ADD_CHILD:
		((CGameObject*)_event.wParam)->AddChild(((CGameObject*)_event.lParam));		
		break;
	case EVENT_TYPE::DISCONNECT_PARENT:
		((CGameObject*)_event.wParam)->DisconnectParent();
		break;
	case EVENT_TYPE::DELETE_OBJECT:
	{
		((CGameObject*)_event.wParam)->Kill();
		bool bFind = false;
		for (UINT i = 0; i < m_vecDelObj.size(); ++i)
		{
			if ((INT_PTR)m_vecDelObj[i] == _event.wParam)
			{
				bFind = true;
				break;
			}			
		}
		if (!bFind)
			m_vecDelObj.push_back(((CGameObject*)_event.wParam));
	}
		break;
	case EVENT_TYPE::CREATE_OBJECT:
		CSceneMgr::GetInst()->AddGameObject((UINT)_event.lParam, ((CGameObject*)_event.wParam));		
		break;
	case EVENT_TYPE::CHANGE_RESOLUTION:

		break;
	case EVENT_TYPE::END:
		break;
	default:
		break;
	}
}
