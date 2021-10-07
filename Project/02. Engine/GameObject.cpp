#include "GameObject.h"

#include "Component.h"
#include "Transform.h"
#include "MeshRender.h"
#include "Collider.h"
#include "Camera.h"
#include "Animator2D.h"
// #include "Animator3D.h"
#include "Script.h"

#include "SceneMgr.h"
#include "Layer.h"
#include "Material.h"
#include "Mesh.h"

CGameObject::CGameObject()
	: m_pParent(NULL)
	, m_arrCom{}
	, m_iLayerIdx(-1)
	, m_bActive(true)
	, m_IsDead(false)
{
	AddComponent(new CTransform);
	
}

CGameObject::CGameObject(const CGameObject & _other)
	: CEntity(_other)
	, m_pParent(nullptr)
	, m_arrCom{}
	, m_iLayerIdx(-1)
	, m_bActive(true)
{
	// 컴포넌트(Script 포함) 깊은복사
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (NULL != _other.m_arrCom[i])
		{
			AddComponent((CComponent*)_other.m_arrCom[i]->Clone());
		}		
	}

	for (UINT i = 0; i < _other.m_vecScript.size(); ++i)
	{
		AddComponent((CComponent*)_other.m_vecScript[i]->Clone());
	}

	// 자식 오브젝트 깊은복사
	for (UINT i = 0; i < _other.m_vecChild.size(); ++i)
	{
		AddChild(_other.m_vecChild[i]->Clone());
	}
}

CGameObject::~CGameObject()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		SAFE_DELETE(m_arrCom[i]);
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		SAFE_DELETE(m_vecChild[i]);
	}

	Safe_Delete_Vec(m_vecScript);
}

void CGameObject::awake()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (NULL != m_arrCom[i])
		{
			m_arrCom[i]->awake();
		}
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->awake();
	}
}

void CGameObject::start()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (NULL != m_arrCom[i])
		{
			m_arrCom[i]->start();
		}
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->start();
	}
}

int CGameObject::update()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (NULL != m_arrCom[i])
		{
			m_arrCom[i]->update();
		}
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->update();
	}
	
	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->update();
	}

	
	
	return 0;
}

int CGameObject::lateupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (NULL != m_arrCom[i])
		{
			m_arrCom[i]->lateupdate();
		}
	}

	for (UINT i = 0; i < m_vecScript.size(); ++i)
	{
		m_vecScript[i]->lateupdate();
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->lateupdate();
	}
	
	return 0;
}

int CGameObject::finalupdate()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (NULL != m_arrCom[i])
		{
			m_arrCom[i]->finalupdate();
		}
	}

	for (UINT i = 0; i < m_vecChild.size(); ++i)
	{
		m_vecChild[i]->finalupdate();
	}
	return 0;
}

void CGameObject::render()
{
	if (MeshRender() == NULL)
		return;
	
	if (m_bActive)
	{
		MeshRender()->render();
	}
}

void CGameObject::AddChild(CGameObject * _pChild)
{
	assert(!(IsAncestor(_pChild)));

	if (NULL == _pChild->GetParent())
	{
		// 자식으로 들어온 오브젝트가 최상위 부모였다면
		// Layer 에서 최상위 오브젝트 해제
		CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(_pChild->GetLayerIdx());
		if(nullptr != pLayer)
			pLayer->PopParentObj(_pChild);
	}
	else
	{
		// 이미 부모가 있었다.
		// 이전 부모와 관계를 정리한다.
		_pChild->DisconnectParent_To_be_child();
	}
	
	m_vecChild.push_back(_pChild);
	_pChild->SetParent(this);
}

void CGameObject::AddChild_Load(CGameObject * _pChild)
{
	assert(!(IsAncestor(_pChild)));
	m_vecChild.push_back(_pChild);
	_pChild->SetParent(this);
}

int CGameObject::AddComponent(CComponent* _pComponent)
{
	COMPONENT_TYPE eType = _pComponent->GetComponentType();
		
	if (COMPONENT_TYPE::SCRIPT == eType)
	{
		m_vecScript.push_back((CScript*)_pComponent);
	}
	else
	{
		// 이미 해당 컴포넌트가 있다.
		if (NULL != m_arrCom[(UINT)eType])
		{
			MessageBox(NULL, L"This component already exists.", L"Error Page.", MB_OK);
			return	RET_FAILED;
		}

		m_arrCom[(UINT)eType] = _pComponent;		
	}

	_pComponent->SetGameObject(this);

	return RET_SUCCESS;
}


int CGameObject::AddComponent(COMPONENT_TYPE _eType, CComponent * _pComponent)
{
	if (COMPONENT_TYPE::SCRIPT == _eType)
	{
		m_vecScript.push_back((CScript*)_pComponent);
	}
	else
	{
		// 이미 해당 컴포넌트가 있다.
		if (NULL != m_arrCom[(UINT)_eType])
			assert(NULL);

		m_arrCom[(UINT)_eType] = _pComponent;
	}

	_pComponent->SetGameObject(this);

	return RET_SUCCESS;
}	


bool CGameObject::IsAncestor(CGameObject* _pObject)
{
	assert(_pObject);

	CGameObject *pParent = m_pParent;
	while (pParent)
	{
		if (_pObject == pParent)
			return true;

		pParent = pParent->m_pParent;
	}

	return false;
}

void CGameObject::DisconnectParent()
{
	//assert(m_pParent);
	if (nullptr == m_pParent)
	{
		return;
	}


	// Layer 의 최상의 부모 오브젝트로 등록
	CSceneMgr::GetInst()->GetLayer(m_iLayerIdx)->AddGameObject(this);


	// 부모 오브젝트의 ChildList 에서 본인을 제거
	for (UINT i = 0; i < m_pParent->m_vecChild.size(); ++i)
	{
		if (this == m_pParent->m_vecChild[i])
		{
			m_pParent->m_vecChild.erase(m_pParent->m_vecChild.begin() + i);
			m_pParent = nullptr;
			return;
		}
	}


	assert(nullptr);
}

void CGameObject::DisconnectParent_To_be_child()
{
	//assert(m_pParent);
	if (nullptr == m_pParent)
	{
		return;
	}


	// 부모 오브젝트의 ChildList 에서 본인을 제거
	for (UINT i = 0; i < m_pParent->m_vecChild.size(); ++i)
	{
		if (this == m_pParent->m_vecChild[i])
		{
			m_pParent->m_vecChild.erase(m_pParent->m_vecChild.begin() + i);
			m_pParent = nullptr;
			return;
		}
	}


	assert(nullptr);
}

void CGameObject::Isolate()
{
	// 자신이 최상위 오브젝트일 경우에만
	if (!m_pParent)
		CSceneMgr::GetInst()->GetLayer(m_iLayerIdx)->PopParentObj(this);
	else
		DisconnectParent();
}

void CGameObject::Save(FILE * _pFile)
{
	SaveWString(GetName(), _pFile);
	fwrite(&m_bActive, sizeof(bool), 1, _pFile);
	fwrite(&m_iLayerIdx, sizeof(int), 1, _pFile);
	// Component 저장
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (nullptr != m_arrCom[i])
		{
			fwrite(&i, sizeof(UINT), 1, _pFile);
			m_arrCom[i]->Save(_pFile);
		}
	}

	int End = (UINT)COMPONENT_TYPE::END;
	fwrite(&End, sizeof(UINT), 1, _pFile);
}

void CGameObject::Load(FILE * _pFile)
{
	SetName(LoadWString(_pFile));
	
	/*if (GetName() == L"EFFECT_OBJECT")
	{
		CSceneMgr::GetInst()->AddGameObject(L"Default", this);		//
	}*/


	fread(&m_bActive, sizeof(bool), 1, _pFile);
	fread(&m_iLayerIdx, sizeof(int), 1, _pFile);


	// Component 불러오기
	int iComType = -1;
	CComponent* pCom = nullptr;
	bool bEnd = false;

	while (!bEnd)
	{
		fread(&iComType, sizeof(UINT), 1, _pFile);

		switch ((COMPONENT_TYPE)iComType)
		{
		case COMPONENT_TYPE::TRANSFORM:
			m_arrCom[(UINT)COMPONENT_TYPE::TRANSFORM]->Load(_pFile);			
			break;
		case COMPONENT_TYPE::MESHRENDER:
			pCom = new CMeshRender();
			pCom->Load(_pFile);
			AddComponent(pCom);
			break;
		case COMPONENT_TYPE::COLLIDER:
			pCom = new CCollider();
			pCom->Load(_pFile);
			AddComponent(pCom);
			break;
		case COMPONENT_TYPE::CAMERA:
			pCom = new CCamera();
			pCom->Load(_pFile);
			AddComponent(pCom);
			break;
		case COMPONENT_TYPE::ANIMATOR2D:
			pCom = new CAnimator2D();
			pCom->Load(_pFile);
			AddComponent(pCom);
			break;
		case COMPONENT_TYPE::ANIMATOR3D:
			
			break;	
		case COMPONENT_TYPE::END:
			bEnd = true;
			continue;
			break;
		default:
			assert(nullptr);
			break;
		}		
	}	
}
