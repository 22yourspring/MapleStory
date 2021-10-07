#include "Layer.h"

#include "GameObject.h"

CLayer::CLayer()
{
	m_iLayerIdx = -1;
}

CLayer::~CLayer()
{
	Safe_Delete_Vec(m_ParentList);
}

void CLayer::awake()
{
	for (auto& pObj : m_ParentList)
	{
		pObj->awake();
	}
}

void CLayer::start()
{
	for (auto& pObj : m_ParentList)
	{
		pObj->start();
	}
}

int CLayer::update()
{
	for (auto& pObj : m_ParentList)
	{
		pObj->update();
	}
	return 0;
}

int CLayer::lateupdate()
{
	for (auto& pObj : m_ParentList)
	{
		pObj->lateupdate();
	}
	return 0;
}

int CLayer::finalupdate()
{
	for (auto& pObj : m_ParentList)
	{
		pObj->finalupdate();
	}
	return 0;
}

void CLayer::render()
{
	for (auto& pObj : m_ParentList)
	{
		pObj->render();
	}
}

void CLayer::AddGameObject(CGameObject * _pObj)
{
	m_ParentList.push_back(_pObj);
	_pObj->SetLayerIdx(m_iLayerIdx);
}

void CLayer::PopParentObj(CGameObject* _pObj)
{
	vector<CGameObject*>::iterator iter = m_ParentList.begin();
	for (; iter != m_ParentList.end(); ++iter)
	{
		if (*iter == _pObj)
		{
			m_ParentList.erase(iter);
			return;
		}
	}

	assert(NULL);
}