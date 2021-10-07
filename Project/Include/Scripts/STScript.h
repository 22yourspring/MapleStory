#pragma once

#include "AnrealEngine\Script.h"
#include "AnrealEngine\SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/Layer.h"
#include "AnrealEngine/GameObject.h"
#include "AnrealEngine/EventMgr.h"

template<typename T>
class CSTScript : public CScript
{
private:
	static T*			m_pThis;

public:
	static T* GetInst()
	{
		// MgrScript�� ������ �Ǿ������� ��ȯ
		if (nullptr != m_pThis)
			return m_pThis;

		// MgrScript �� �����Ǿ����� ������ ���� DummyObject �� �߰��� ��ȯ
		CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
		CGameObject* pMgrObj = pCurScene->FindObject(L"MgrObject");

		// DummyObject �� ���� ������ �ȵȰ��
		if (nullptr == pMgrObj)
		{
			pMgrObj = new CGameObject;
			pMgrObj->SetName(L"MgrObject");

			int iIdx = CSceneMgr::GetInst()->GetLayer(L"Default")->GetIdx();

			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJECT, (INT_PTR)pMgrObj, iIdx });
		}

		m_pThis = new T;
		pMgrObj->AddComponent(m_pThis);

		return m_pThis;
	}

protected:
	CSTScript()
	{

	}
	~CSTScript()
	{
		m_pThis = nullptr;
	}
};

template<typename T>
T* CSTScript<T>::m_pThis = nullptr;