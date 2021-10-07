#pragma once
#include "Entity.h"

class CLayer;
class CCamera;
class CGameObject;

class CScene :
	public CEntity
{
private:
	CLayer*				m_arrLayer[MAX_LAYER];
	UINT				m_iLayerCount;	

	vector<CCamera*>	m_vecCam;
	wstring				m_SceneName;

public:
	virtual void awake();
	virtual void start();
	virtual int update();
	virtual int lateupdate();
	virtual int finalupdate();

	void render();
	
public:
	void AddLayer(const wstring& _strLayerName);
	void AddLayer(CLayer* _pLayer);
	void AddGameObject(const wstring& _strLayerName, CGameObject* _pObj);	
	void AddGameObject(UINT _iLayerIdx, CGameObject* _pObj);

	CLayer* GetLayer(int _iIdx) {if (_iIdx == -1) return nullptr; return m_arrLayer[_iIdx];}
	CLayer* GetLayer(const wstring& _strLayerName);
	UINT GetLayerCount() { return m_iLayerCount; }
	void RegisterCamera(CCamera* _pCam);

	bool SetUpdatePlay;
	wstring GetSceneName() { return m_SceneName; }
	void SetSceneName(const wstring& _SceneName) {	m_SceneName = _SceneName;}

	CGameObject* FindObject(const wstring& _strName);
public:
	CLONE(CScene);

public:
	CScene();
	CScene(int _m_iLayerCount);
	virtual ~CScene();
};

