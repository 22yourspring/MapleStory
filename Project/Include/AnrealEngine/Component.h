#pragma once
#include "GameObject.h"

class CTransform;
class CMeshRender;
class CCamera;
class CCollider;
class CAnimator2D;
class CAnimator3D;

class CComponent : public CEntity
{
private:
	COMPONENT_TYPE	m_eType;
	CGameObject*	m_pObject;
	bool			m_bActive;

protected:
	void SetComponentType(COMPONENT_TYPE _eType){m_eType = _eType;}
	CTransform* Transform() { return m_pObject->Transform(); }
	CMeshRender* MeshRender() { return m_pObject->MeshRender(); }
	CCamera* Camera() { return m_pObject->Camera(); }
	CCollider* Collider() { return m_pObject->Collider(); }
	CAnimator2D* Animator2D() { return m_pObject->Animator2D(); }
	CAnimator3D* Animator3D() { return m_pObject->Animator3D(); }
	CGameObject* Object() { return m_pObject; }
	vector<CGameObject*> GetChild() {	return m_pObject->GetChild(); }
	CGameObject* ParentObject() { return m_pObject->GetParent(); }

private:
	void SetGameObject(CGameObject* _pObject) { m_pObject = _pObject; }
public:
	CGameObject* GetGameObject() { return m_pObject; }
public:
	COMPONENT_TYPE GetComponentType() {return m_eType;}
	virtual void Save(FILE* _pFile); 
	virtual void Load(FILE* _pFile); 

public:
	CComponent();
	CComponent(const CComponent& _other)
		: m_eType(_other.m_eType)
		, m_pObject(nullptr)
		, m_bActive(true)
	{}
	virtual ~CComponent();

	friend class CGameObject;
};

