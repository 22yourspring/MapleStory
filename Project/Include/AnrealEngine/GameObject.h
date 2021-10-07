#pragma once
#include "Entity.h"


template<typename T>
struct ID
{
	typedef T Type;
};

class CComponent;
class CTransform;
class CMeshRender;
class CScript;
class CCamera;
class CCollider;
class CAnimator2D;
class CAnimator3D;
class CLayer;
class CGameObject :
	public CEntity
{
private:
	CGameObject*			m_pParent;
	CComponent*				m_arrCom[(UINT)COMPONENT_TYPE::END];
	vector<CScript*>		m_vecScript;
	vector<CGameObject*>	m_vecChild;

	int						m_iLayerIdx;
	bool					m_bActive;
	bool					m_IsDead;


public:
	virtual void awake();
	virtual void start();
	virtual int update();
	virtual int lateupdate();
	virtual int finalupdate();
	void render();

public:
	void AddChild(CGameObject* _pChild);
	void AddChild_Load(CGameObject* _pChild);

	int AddComponent(CComponent* _pComponent);
	int AddComponent(COMPONENT_TYPE _eType, CComponent* _pComponent);

	/*template<typename T>
	int AddComponent(T* _pComponent)
	{
		AddComponent(_pComponent, ID<T>());
		return 0;
	}

private:
	template<typename T>
	int AddComponent(T* _pComponent, ID<T> _type);
	int AddComponent(CScript* _pScript, ID<CScript> _type);*/

public:
 	CComponent* GetComponent(COMPONENT_TYPE _eType) { return m_arrCom[(UINT)_eType]; }
	CTransform* Transform() { return (CTransform*)GetComponent(COMPONENT_TYPE::TRANSFORM); }
	CMeshRender* MeshRender() { return (CMeshRender*)GetComponent(COMPONENT_TYPE::MESHRENDER); }
	CCamera* Camera() { return (CCamera*)GetComponent(COMPONENT_TYPE::CAMERA); }
	CCollider* Collider() { return (CCollider*)GetComponent(COMPONENT_TYPE::COLLIDER); }
	CAnimator2D* Animator2D() { return (CAnimator2D*)GetComponent(COMPONENT_TYPE::ANIMATOR2D); }
	CAnimator3D* Animator3D() { return (CAnimator3D*)GetComponent(COMPONENT_TYPE::ANIMATOR3D); }

	int GetLayerIdx() { return m_iLayerIdx; }
	CGameObject* GetParent() { return m_pParent; }

	vector<CScript*>& GetScript() { return m_vecScript; }
	vector<CGameObject*>& GetChild() { return m_vecChild; }
	bool IsAncestor(CGameObject* _pObject);

private:
	void SetParent(CGameObject* _pParent) { m_pParent = _pParent; }
public:
	void SetLayerIdx(int _iIdx) { m_iLayerIdx = _iIdx; }

public:
	void DisconnectParent();
	void DisconnectParent_To_be_child();
	void Isolate();
	void Kill() { m_IsDead = true; }

	void Regeneration() { m_IsDead = false; }

	bool Confirm_Life() { return !m_IsDead; }

	void SetActive(bool _m_bActive) { m_bActive = _m_bActive; }
	bool GetActive() { return m_bActive; }

public:
	CLONE(CGameObject);
	void Save(FILE* _pFile);
	void Load(FILE* _pFile);


public:
	CGameObject();
	CGameObject(const CGameObject& _other);
	virtual ~CGameObject();

	friend class CLayer;
	friend class CEventMgr;
};
//
//template<typename T>
//inline int CGameObject::AddComponent(T * _pComponent, ID<T> _type)
//{
//	static const type_info& info = typeid(T);	
//	static const type_info& transform = typeid(CTransform);
//	static const type_info& meshrender = typeid(CMeshRender);
//	static const type_info& camera = typeid(CCamera);
//	
//	COMPONENT_TYPE eType = COMPONENT_TYPE::END;
//	if (&transform == &info)
//	{
//		eType = COMPONENT_TYPE::TRANSFORM;		
//	}
//	else if (&meshrender == &info)
//	{
//		eType = COMPONENT_TYPE::MESHRENDER;		
//	}	
//	else if (&camera == &info)
//	{
//		eType = COMPONENT_TYPE::CAMERA;
//	}
//
//	if (NULL != m_arrCom[(UINT)eType])
//		return RET_FAILED;
//
//	m_arrCom[(UINT)eType] = _pComponent;
//	_pComponent->SetGameObject(this);
//
//	return RET_SUCCESS;
//}
