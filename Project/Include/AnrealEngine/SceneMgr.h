#pragma once
#include "global.h"

#include "Scene.h"

class CCamera;
class CGameObject;

enum FADE
{
	FADEIN,
	NOTHING,
	FADEOUT,
};

class CSceneMgr
{
	SINGLE(CSceneMgr)
private:
	CScene*		m_pCurScene;
	
public:
	CLayer* GetLayer(int _iIdx);
	CLayer* GetLayer(const wstring& _strLayerName);
	void RegisterCamera(CCamera* _pCam); // 현재 Scene 에 카메라를 등록시킨다.
	void AddGameObject(const wstring& _strLayerName, CGameObject* _pObj);
	void AddGameObject(UINT _iLayerIdx, CGameObject* _pObj);

	CScene* GetCurScene() { return m_pCurScene; }
	void ChangeScene(CScene* _pScene);
	CGameObject* FindObject(const wstring& _strName) { assert(m_pCurScene);  return m_pCurScene->FindObject(_strName); }

	static FADE m_bFadeIn;
public:
	void init();
	int update();
	void render();
};

