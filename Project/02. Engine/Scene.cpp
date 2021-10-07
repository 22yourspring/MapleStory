#include "Scene.h"

#include "Layer.h"
#include "Camera.h"

#include "SceneMgr.h"

CScene::CScene()
	: m_iLayerCount(0)
	, m_arrLayer{}
	, SetUpdatePlay(true)
{
	AddLayer(L"Default");
}

CScene::CScene(int _m_iLayerCount)
	: m_iLayerCount(_m_iLayerCount)
	, m_arrLayer{}
	, SetUpdatePlay(true)
{
	CSceneMgr::m_bFadeIn = FADE::FADEIN;
}

CScene::~CScene()
{
	CSceneMgr::m_bFadeIn = FADE::FADEOUT;

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		SAFE_DELETE(m_arrLayer[i]);
	}
}

void CScene::awake()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_arrLayer[i])
			m_arrLayer[i]->awake();
	}
}

void CScene::start()
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_arrLayer[i])
			m_arrLayer[i]->start();
	}
}

int CScene::update()
{
	if (!SetUpdatePlay)
	{
		return 0;
	}

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_arrLayer[i])
			m_arrLayer[i]->update();
	}
	return 0;
}

int CScene::lateupdate()
{
	if (!SetUpdatePlay)
	{
		return 0;
	}
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_arrLayer[i])
			m_arrLayer[i]->lateupdate();
	}
	return 0;
}

int CScene::finalupdate()
{
	if (!SetUpdatePlay)
	{
		return 0;
	}
	m_vecCam.clear();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_arrLayer[i])
			m_arrLayer[i]->finalupdate();
	}
	return 0;
}

void CScene::render()
{
	// 카메라가 기준이 되어서 Layer 를 그린다.
	// 카메라는 찍고 싶은 Layer 만 골라서 찍을수 있다.	

	for (UINT i = 0; i < m_vecCam.size(); ++i)
	{
		m_vecCam[i]->render(m_arrLayer);
	}
}

void CScene::AddLayer(const wstring & _strLayerName)
{
	if (m_iLayerCount >= MAX_LAYER)
	{
		MessageBox(NULL, L"Currently overhangs of Layer", L"Error Page.", MB_OK);
		return;
	}

	CLayer* pNewLayer = new CLayer;
	pNewLayer->SetName(_strLayerName);
	pNewLayer->SetIdx(m_iLayerCount);

	m_arrLayer[m_iLayerCount++] = pNewLayer;	
}

void CScene::AddLayer(CLayer * _pLayer)
{
	if (m_iLayerCount >= MAX_LAYER)
	{
		assert(NULL);
	}		
	_pLayer->SetIdx(m_iLayerCount);
	m_arrLayer[m_iLayerCount++] = _pLayer;
}

void CScene::AddGameObject(const wstring & _strLayerName, CGameObject * _pObj)
{
	for (UINT i = 0; i < m_iLayerCount; ++i)
	{
		if (m_arrLayer[i]->GetName() == _strLayerName)
		{
			m_arrLayer[i]->AddGameObject(_pObj);
			return;
		}
	}	

	assert(NULL);
}

void CScene::AddGameObject(UINT _iLayerIdx, CGameObject * _pObj)
{
	assert(_iLayerIdx < 32);
	m_arrLayer[_iLayerIdx]->AddGameObject(_pObj);	
}

CLayer * CScene::GetLayer(const wstring & _strLayerName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL != m_arrLayer[i] && m_arrLayer[i]->GetName() == _strLayerName)
			return m_arrLayer[i];
	}
//	assert(NULL); // 해당 이름의 레이어가 없다.
	return nullptr;
}

void CScene::RegisterCamera(CCamera * _pCam)
{
	m_vecCam.push_back(_pCam);
}

CGameObject * CScene::FindObject(const wstring & _strName)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == m_arrLayer[i])
			return nullptr;

		vector<CGameObject*>& vecObj = m_arrLayer[i]->GetObjectVec();
		for (auto pObj : vecObj)
		{
			if (pObj->GetName() == _strName)
			{
				return pObj;
			}
		}
	}	

	return nullptr;
}


