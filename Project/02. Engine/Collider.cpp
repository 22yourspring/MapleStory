#include "Collider.h"

#include "Transform.h"

#include "ResMgr.h"
#include "ConstBuffer.h"
#include "Device.h"

#include "CollisionMgr.h"
#include "GameObject.h"

#include "Script.h"
#include "Material.h"
#include "Mesh.h"

#include "SceneMgr.h"
#include "Layer.h"

DWORD CCollider::g_iID = 0;

CCollider::CCollider()
	: m_dwID(g_iID++)
	, m_fRadius(1.f)
	, m_fWorldRadius(0.f)		
	, m_pMtrl(nullptr)
	, m_pCloneMtrl(nullptr)
	, m_pMesh(nullptr)
{
	SetComponentType(COMPONENT_TYPE::COLLIDER);
	m_pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"ColliderMtrl");	
}

CCollider::CCollider(const CCollider & _other)
{
	*this = _other;
	m_dwID = g_iID++;
}

CCollider::~CCollider()
{
}

int CCollider::update()
{
	return 0;
}

int CCollider::finalupdate()
{
	// Collider 의 월드 행렬 계산하기
	CalColliderMat();
	
	// CollisionMgr 에 등록
	CCollisionMgr::GetInst()->AddCollider(Object()->GetLayerIdx(), this);

	return 0;
}

void CCollider::CalColliderMat()
{
	Matrix matScale, matTrans;

	// 이동 행렬	
	matTrans = XMMatrixTranslation(m_vOffsetPos.x, m_vOffsetPos.y, m_vOffsetPos.z);

	switch (m_eType)
	{
	case COLLIDER_TYPE::COLLIDER_2D_BOX:
	case COLLIDER_TYPE::COLLIDER_3D_CUBE:
		// 크기 행렬
		matScale = XMMatrixScaling(m_vOffsetScale.x, m_vOffsetScale.y, m_vOffsetScale.z);
		
		// 월드행렬
		m_matCol = matScale * matTrans;
		m_matCol *= Transform()->GetWorldMat();

		break;
	case COLLIDER_TYPE::COLLIDER_2D_CIRCLE:		
	case COLLIDER_TYPE::COLLIDER_3D_SHPERE:
	{
		matScale = XMMatrixScaling(m_fRadius, m_fRadius, m_fRadius);
		m_matCol = matScale * matTrans;

		Vec3 vScale, vPos;
		Quaternion qRot;
		Matrix matObjWorld = Transform()->GetWorldMat();
		matObjWorld.Decompose(vScale, qRot, vPos);

		float fMaxScale = max(vScale.x, vScale.y);

		if (COLLIDER_TYPE::COLLIDER_3D_SHPERE == m_eType)
		{
			fMaxScale = max(fMaxScale, vScale.z); 
		}

		Matrix matObjTrans = XMMatrixTranslation(vPos.x, vPos.y, vPos.z);
		Matrix matObjScale = XMMatrixScaling(fMaxScale, fMaxScale, fMaxScale);

		m_matCol *= matObjScale * matObjTrans;

		m_fWorldRadius = m_fRadius * fMaxScale;
	}
		break;
	default:
		break;
	}	
}

void CCollider::render()
{
	if (NULL == m_pMesh || m_pMtrl == NULL)
		return;

	// Collider 행렬정보를 버퍼에 업데이트 시켜야 한다.
	/*g_transform.matWorld = m_matCol;
	g_transform.matWV = g_transform.matWorld * g_transform.matView;
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;*/


	g_transform.matWorld = m_matCol;

	if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"UI") != nullptr)
	{
		if (GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"UI")->GetIdx())
		{
			g_transform.matWV = g_transform.matWorld;
		}
		else
		{
			g_transform.matWV = g_transform.matWorld * g_transform.matView;
		}
	}
	else
	{
		g_transform.matWV = g_transform.matWorld * g_transform.matView;
	}


	g_transform.matWVP = g_transform.matWV * g_transform.matProj;



	static CConstBuffer* pBuffer = CDevice::GetInst()->FindCBuffer(L"Transform");

	pBuffer->UpdateData(&g_transform, sizeof(tTransformMatrix));
	pBuffer->UpdateRegister(SHADER_TYPE::ST_VERTEX);

	// Material 정보를 업데이트 시킨다.
	CMaterial* pMtrl = nullptr;
	if (m_pCloneMtrl)
		pMtrl = m_pCloneMtrl;
	else
		pMtrl = m_pMtrl;

	pMtrl->UpdateData();

	m_pMesh->SetShader(pMtrl->GetShader());
	m_pMesh->render();
}

void CCollider::OnCollisionEnter(CCollider * _pOther)
{
	int i = 1;
	if (!m_pCloneMtrl)
		m_pCloneMtrl = m_pMtrl->Clone();

	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &i);

	vector<CScript*>& vecScript = Object()->GetScript();
	for (auto pScript : vecScript)
	{
		pScript->OnCollisionEnter(_pOther);
	}
}

void CCollider::OnCollision(CCollider * _pOther)
{
	vector<CScript*>& vecScript = Object()->GetScript();
	for (auto pScript : vecScript)
	{
		pScript->OnCollision(_pOther);
	}
}

void CCollider::OnCollisionExit(CCollider * _pOther)
{
	int i = 0;
	m_pCloneMtrl->SetData(SHADER_PARAM::INT_0, &i);

	vector<CScript*>& vecScript = Object()->GetScript();
	for (auto pScript : vecScript)
	{
		pScript->OnCollisionExit(_pOther);
	}
}

void CCollider::SetColliderType(COLLIDER_TYPE _eType)
{
	m_eType = _eType;

	switch (m_eType)
	{
	case COLLIDER_TYPE::COLLIDER_2D_BOX:
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"ColliderRectMesh");
		break;
	case COLLIDER_TYPE::COLLIDER_2D_CIRCLE:
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"ColliderCircleMesh");
		break;
	case COLLIDER_TYPE::COLLIDER_3D_CUBE:
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"CubeMesh");
		break;
	case COLLIDER_TYPE::COLLIDER_3D_SHPERE:
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(L"SphereMesh");
		break;	
	}
}

void CCollider::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_eType, sizeof(UINT), 1, _pFile); 			// 충돌체 타입	
	fwrite(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);		// 상대적인 위치
	fwrite(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);	// 충돌체 크기 절대값(부모 오브젝트의 영향을 받지 않는다.)
	
	bool bMtrl = false;
	if (nullptr != m_pMtrl)
		bMtrl = true;

	fwrite(&bMtrl, 1, 1, _pFile);

	if(bMtrl)
		SaveWString(m_pMtrl->GetName(), _pFile);	
	
	fwrite(&m_fRadius, sizeof(float), 1, _pFile);	
}

void CCollider::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);

	fread(&m_eType, sizeof(UINT), 1, _pFile); 		// 충돌체 타입
	SetColliderType(m_eType);

	fread(&m_vOffsetPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vOffsetScale, sizeof(Vec3), 1, _pFile);

	bool bMtrl = false;
	fread(&bMtrl, 1, 1, _pFile);

	if (bMtrl)
	{
		wstring strKey = LoadWString(_pFile);
		m_pMtrl = CResMgr::GetInst()->Load<CMaterial>(strKey);
	}

	fread(&m_fRadius, sizeof(float), 1, _pFile);
} 