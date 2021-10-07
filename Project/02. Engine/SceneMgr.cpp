#include "SceneMgr.h"
#include "Scene.h"

#include "GameObject.h"
#include "MeshRender.h"
#include "Transform.h"
#include "Collider.h"
#include "Animator2D.h"
#include "Camera.h"

#include "Mesh.h"
#include "Prefab.h"
#include "Material.h"
#include "Shader.h"

#include "ResMgr.h"
#include "ShaderMgr.h"
#include "Device.h"

#include "CollisionMgr.h"

FADE CSceneMgr::m_bFadeIn = FADE::NOTHING;

CSceneMgr::CSceneMgr()
	: m_pCurScene(NULL)
{
}

CSceneMgr::~CSceneMgr()
{
	SAFE_DELETE(m_pCurScene);
}

void CSceneMgr::init()
{
	CDevice::GetInst()->SetRasteriazeState(RASTERIZER_TYPE::CULL_NONE);
	m_pCurScene = new CScene;
}

int CSceneMgr::update()
{
	m_pCurScene->update();
	m_pCurScene->lateupdate();
	m_pCurScene->finalupdate();

	CCollisionMgr::GetInst()->update();

	return 0;
}

void CSceneMgr::render()
{
	m_pCurScene->render();
}


CLayer * CSceneMgr::GetLayer(int _iIdx)
{
	assert(m_pCurScene);
	return m_pCurScene->GetLayer(_iIdx);
}

CLayer * CSceneMgr::GetLayer(const wstring & _strLayerName)
{
	assert(m_pCurScene);
	return m_pCurScene->GetLayer(_strLayerName);
}

void CSceneMgr::RegisterCamera(CCamera * _pCam)
{
	assert(m_pCurScene);
	m_pCurScene->RegisterCamera(_pCam);
}

void CSceneMgr::AddGameObject(const wstring & _strLayerName, CGameObject * _pObj)
{
	assert(m_pCurScene);
	m_pCurScene->AddGameObject(_strLayerName, _pObj);
}

void CSceneMgr::AddGameObject(UINT _iLayerIdx, CGameObject * _pObj)
{
	assert(m_pCurScene);
	m_pCurScene->AddGameObject(_iLayerIdx, _pObj);
}

void CSceneMgr::ChangeScene(CScene * _pScene)
{
	assert(m_pCurScene);
	SAFE_DELETE(m_pCurScene);
	m_pCurScene = _pScene;

	//m_bFadeIn = FADE::FADEIN;
}
