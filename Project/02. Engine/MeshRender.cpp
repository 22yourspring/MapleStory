#include "MeshRender.h"

#include "Device.h"
#include "Mesh.h"
#include "Material.h"
#include "Transform.h"
#include "Collider.h"
#include "Animator2D.h"

#include "ResMgr.h"



CMeshRender::CMeshRender()
	: m_pMesh(nullptr)
	, m_pMtrl(nullptr)
{
	SetComponentType(COMPONENT_TYPE::MESHRENDER);
}

CMeshRender::~CMeshRender()
{
}

CMaterial * CMeshRender::GetCloneMaterial()
{
	m_pMtrl = m_pMtrl->Clone();
	return m_pMtrl;
}

int CMeshRender::update()
{
	return 0;
}

void CMeshRender::render()
{
	if (NULL == m_pMesh || m_pMtrl == NULL)
		return;
		
	// Transform ��������� ���ۿ� ������Ʈ ���Ѿ� �Ѵ�.
	Transform()->UpdateData();

	// Animation Component �� ������, ���� �����͸� ������Ʈ ��Ų��.
	if (Animator2D())
	{
		Animator2D()->UpdateData();
	}
	else
	{
		CAnimator2D::ClearData();
	}
	
	// Material ������ ������Ʈ ��Ų��.
	m_pMtrl->UpdateData();
	
	m_pMesh->SetShader(m_pMtrl->GetShader());
	m_pMesh->render();

	// Collider �� �ִٸ�
	if (Collider())
	{
		Collider()->render();
	}

	if (!GetChild().empty())
	{
		for (UINT i = 0; i < GetChild().size(); ++i)
		{
			GetChild()[i]->render();
		}
	}

}

void CMeshRender::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	bool bMesh = false;
	bool bMtrl = false;

	if (nullptr != m_pMesh)
		bMesh = true;
	if (nullptr != m_pMtrl)
		bMtrl = true;

	fwrite(&bMesh, 1, 1, _pFile);
	fwrite(&bMtrl, 1, 1, _pFile);

	if(bMesh)
		SaveWString(m_pMesh->GetName(), _pFile);
	if(bMtrl)
		SaveWString(m_pMtrl->GetName(), _pFile);	
}

void CMeshRender::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);

	bool bMesh = false;
	bool bMtrl = false;

	fread(&bMesh, 1, 1, _pFile);
	fread(&bMtrl, 1, 1, _pFile);

	wstring strMesh, strMtrl;

	if (bMesh)
	{
		strMesh = LoadWString(_pFile);
		m_pMesh = CResMgr::GetInst()->Load<CMesh>(strMesh);
	}
		
	if (bMtrl)
	{
		strMtrl = LoadWString(_pFile);
		m_pMtrl = CResMgr::GetInst()->Load<CMaterial>(strMtrl);
	}
}
