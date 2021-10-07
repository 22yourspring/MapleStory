#pragma once
#include "Entity.h"

class CGameObject;

class CLayer :
	public CEntity
{
private:
	vector<CGameObject*>		m_ParentList;
	//vector<CGameObject*>		m_AllObject;
	int							m_iLayerIdx;

public:
	void SetIdx(UINT _iIdx) { m_iLayerIdx = _iIdx; }
	UINT GetIdx() { return m_iLayerIdx; };
	void AddGameObject(CGameObject* _pObj);
	void PopParentObj(CGameObject* _pObj); // 부모 오브젝트 리스트에서 제거(삭제 X)
	vector<CGameObject*>& GetObjectVec() { return m_ParentList;}


public:
	virtual void awake();
	virtual void start();
	virtual int update();
	virtual int lateupdate();
	virtual int finalupdate();
	void render();

public:
	CLONE(CLayer);

public:
	CLayer();
	virtual ~CLayer();
};

