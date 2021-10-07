// CMeshRenderDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "MeshRenderDlg.h"
#include "afxdialogex.h"

#include "Resource.h"

#include "ListDlg.h"

#include "AnrealEngine/Res.h"
#include "AnrealEngine/ResMgr.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Mesh.h"
#include "AnrealEngine/Material.h"

// CMeshRenderDlg 대화 상자
IMPLEMENT_DYNAMIC(CMeshRenderDlg, CDialogEx)

CMeshRenderDlg::CMeshRenderDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_MESHRENDERDLG, pParent)
{

}

CMeshRenderDlg::~CMeshRenderDlg()
{
}

void CMeshRenderDlg::update(CGameObject * _pTargetObj)
{
	m_pTargetObj = _pTargetObj;
		
	CMesh* pMesh = m_pTargetObj->MeshRender()->GetMesh();
	m_EditMesh.SetWindowTextW(pMesh->GetName().c_str());

	CMaterial* pMtrl = _pTargetObj->MeshRender()->GetSharedMaterial();
	m_EditMtrl.SetWindowTextW(pMtrl->GetName().c_str());	
}

void CMeshRenderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT1, m_EditMesh);
	DDX_Control(pDX, IDC_EDIT2, m_EditMtrl);
}


BEGIN_MESSAGE_MAP(CMeshRenderDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CMeshRenderDlg::OnChangeMeshBtn)
	ON_BN_CLICKED(IDC_BUTTON2, &CMeshRenderDlg::OnChangeMtrlBtn)
END_MESSAGE_MAP()


// CMeshRenderDlg 메시지 처리기
void CMeshRenderDlg::OnChangeMeshBtn()
{
	const map<wstring, CRes*>& mapMesh = CResMgr::GetInst()->GetResourceMap(RESOURCE_TYPE::MESH);

	CListDlg listdlg;
	listdlg.SetCaption(L"Mesh");
	
	for (auto pair : mapMesh)
	{		
		listdlg.AddData(pair.second->GetName().c_str());
	}
	
	UINT id = (UINT)listdlg.DoModal();

	if (IDOK == id)
	{
		CString strMesh = listdlg.GetClickItem();
		m_pTargetObj->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(strMesh.GetBuffer()));
	}
}

void CMeshRenderDlg::OnChangeMtrlBtn()
{
	const map<wstring, CRes*>& mapMtrl = CResMgr::GetInst()->GetResourceMap(RESOURCE_TYPE::MATERIAL);
	
	CListDlg listdlg;
	listdlg.SetCaption(L"Material");
	
	for (auto pair : mapMtrl)
	{
		listdlg.AddData(pair.second->GetName().c_str());
	}

	UINT id = (UINT)listdlg.DoModal();
	
	if (IDOK == id)
	{
		CString strMesh = listdlg.GetClickItem();
		m_pTargetObj->MeshRender()->SetMaterial(CResMgr::GetInst()->Load<CMaterial>(strMesh.GetBuffer()));
	}
}