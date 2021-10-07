// ComponentView.cpp: 구현 파일
//

#include "stdafx.h"
#include "ComponentView.h"

#include "resource.h"
#include "TransformDlg.h"
#include "MeshRenderDlg.h"
#include "ColliderDlg.h"
#include "CameraDlg.h"
#include "ScriptDlg.h"
#include "Animator2DDlg.h"

#include "AnrealEngine/SceneMgr.h"

#include "AnrealEngine/ResMgr.h"

#include "AnrealEngine/GameObject.h"

#include "AnrealEngine/Animator2D.h"


#include "AnrealEngine/Transform.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Collider.h"
#include "AnrealEngine/Camera.h"
#include "AnrealEngine/Animation2D.h"


// CComponentView

IMPLEMENT_DYNCREATE(CComponentView, CFormView)

CComponentView::CComponentView()
	: CFormView(IDD_COMVIEW)
	, m_pTargetObj(nullptr)
	, m_arrComDlg{}
{	
}

CComponentView::~CComponentView()

{
}

BEGIN_MESSAGE_MAP(CComponentView, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_BUTTON1, &CComponentView::AddComponent)
END_MESSAGE_MAP()


// CComponentView 그리기

void CComponentView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CComponentView 진단

#ifdef _DEBUG
void CComponentView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CComponentView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


void CComponentView::init()
{
	m_arrComDlg[(UINT)COMDLG_TYPE::CAMERA]->init();

	ComponentComboBox.InsertString(0, L"TRANSFORM");
	ComponentComboBox.InsertString(1, L"MESHRENDER");
	ComponentComboBox.InsertString(2, L"COLLIDER");
	ComponentComboBox.InsertString(3, L"CAMERA");
	ComponentComboBox.InsertString(4, L"ANIMATOR 2D");
	ComponentComboBox.InsertString(5, L"ANIMATOR 3D");


	// 스크롤바 없애기
	CSize size;
	size = GetTotalSize();
	size.cx = 0;
	size.cy = 0;
	SetScrollSizes(MM_TEXT, size);

}

void CComponentView::update()
{
	// 타겟 오브젝트가 없으면, 아무것도 보이지 않게 한다.

	if (nullptr == m_pTargetObj)
	{
		for (UINT i = 0; i < (UINT)COMDLG_TYPE::END; ++i)
		{
			if(nullptr != m_arrComDlg[i])
				m_arrComDlg[i]->ShowWindow(false);
		}
		return;
	}

	if (m_pTargetObj->Confirm_Life() == false)
	{
		m_pTargetObj = nullptr;
		return;
	}

	// ComponentDlg 위치 지정
	ComponentDlgSort();

	// 정보 갱신
	for (UINT i = 0; i < (UINT)COMDLG_TYPE::END; ++i)
	{
		if (i == (UINT)COMDLG_TYPE::SCRIPT)
		{
			m_arrComDlg[i]->update(m_pTargetObj);
			continue;
		}		

		if (nullptr != m_arrComDlg[i] && m_pTargetObj->GetComponent((COMPONENT_TYPE)i) != nullptr)
		{			
			m_arrComDlg[i]->update(m_pTargetObj);
		}
	}	
}

void CComponentView::Lastinit()
{
	m_arrComDlg[(UINT)COMDLG_TYPE::CAMERA]->init();
}

void CComponentView::ComponentDlgSort()
{
	UINT iCurY = 0;
	CRect rt = {};
	GetWindowRect(&rt);

	for (UINT i = 0; i < (UINT)COMDLG_TYPE::END; ++i)
	{
		if (i != (UINT)COMDLG_TYPE::SCRIPT)
		{
			CComponent* pComponent = m_pTargetObj->GetComponent((COMPONENT_TYPE)i);

			if (nullptr == m_arrComDlg[i])
				continue;
			else if (nullptr == pComponent)
			{
				m_arrComDlg[i]->ShowWindow(false);
				continue;
			}
		}		

		CRect rtDLG;
		m_arrComDlg[i]->GetWindowRect(&rtDLG);

		m_arrComDlg[i]->ShowWindow(true);
		m_arrComDlg[i]->SetWindowPos(nullptr, 0, iCurY, rt.Width(), rtDLG.Height(), 0);
		iCurY += rtDLG.Height() - 1;
	}

	CRect rt1, rt2;
	GetWindowRect(&rt1);
	m_btnAddCom.GetWindowRect(&rt2);
	m_btnAddCom.SetWindowPos(nullptr, rt1.Width() / 2 + 80 - rt2.Width() / 2, iCurY + 30, rt2.Width(), rt2.Height(), 0);



	ComponentComboBox.GetWindowRect(&rt2);
	ComponentComboBox.SetWindowPos(nullptr, rt1.Width() / 3 - rt2.Width() / 2 - 13, iCurY + 30, rt2.Width(), rt2.Height(), 0);
}

// CComponentView 메시지 처리기
void CComponentView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	


}


int CComponentView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;

	m_arrComDlg[(UINT)COMDLG_TYPE::TRANSFORM] = new CTransformDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::TRANSFORM]->Create(IDD_TRANSFORMDLG, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::TRANSFORM]->ShowWindow(false);

	m_arrComDlg[(UINT)COMDLG_TYPE::MESHRENDER] = new CMeshRenderDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::MESHRENDER]->Create(IDD_MESHRENDERDLG, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::MESHRENDER]->ShowWindow(false);

	m_arrComDlg[(UINT)COMDLG_TYPE::COLLIDER] = new CColliderDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::COLLIDER]->Create(IDD_COLLIDERDLG,this);
	m_arrComDlg[(UINT)COMDLG_TYPE::COLLIDER]->ShowWindow(false);

	m_arrComDlg[(UINT)COMDLG_TYPE::CAMERA] = new CCameraDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::CAMERA]->Create(IDD_CAMERADLG, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::CAMERA]->ShowWindow(false);

	m_arrComDlg[(UINT)COMDLG_TYPE::SCRIPT] = new CScriptDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::SCRIPT]->Create(IDD_SCRIPTDLG, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::SCRIPT]->ShowWindow(false);

	m_arrComDlg[(UINT)COMDLG_TYPE::ANIMATOR2D] = new CAnimator2DDlg(this);
	m_arrComDlg[(UINT)COMDLG_TYPE::ANIMATOR2D]->Create(IDD_ANIMATOR2DDLG, this);
	m_arrComDlg[(UINT)COMDLG_TYPE::ANIMATOR2D]->ShowWindow(false);

	return 0;
}

void CComponentView::OnDestroy()
{
	CFormView::OnDestroy();

	for (UINT i = 0; i < (UINT)COMDLG_TYPE::END; ++i)
	{
		if(nullptr != m_arrComDlg[i])
			m_arrComDlg[i]->DestroyWindow();
	}	
}


void CComponentView::DoDataExchange(CDataExchange* pDX)
{
	DDX_Control(pDX, IDC_BUTTON1, m_btnAddCom);

	CRect rt1, rt2;
	GetWindowRect(&rt1);
	m_btnAddCom.GetWindowRect(&rt2);
	m_btnAddCom.SetWindowPos(nullptr, rt1.Width() / 2 + 80 - rt2.Width() / 2, 30, rt2.Width(), rt2.Height(), 0);

	CFormView::DoDataExchange(pDX);

	DDX_Control(pDX, IDC_COMBO2, ComponentComboBox);
	ComponentComboBox.GetWindowRect(&rt2);
	ComponentComboBox.SetWindowPos(nullptr, rt1.Width() / 3 - rt2.Width() / 2 - 13 , 30, rt2.Width(), rt2.Height(), 0);

}


void CComponentView::AddComponent()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	if (m_pTargetObj == nullptr)
	{
		MessageBox(_T("The selected object does not exist."), _T("Error"));
		return;
	}


	if (0 == ComponentComboBox.GetCurSel())
	{
		m_pTargetObj->AddComponent(new CTransform);
	}

	else if (1 == ComponentComboBox.GetCurSel())
	{
		m_pTargetObj->AddComponent(new CMeshRender);

		Ptr<CTexture> pPlayerTex = CResMgr::GetInst()->Load<CTexture>(L"Default", L"Texture\\00.png");
		Ptr<CMaterial> pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Std2DMtrl");
		pMtrl->SetData(SHADER_PARAM::TEX_0, pPlayerTex);
		int i = 1;
		pMtrl->SetData(SHADER_PARAM::INT_0, &i);

		m_pTargetObj->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
		m_pTargetObj->MeshRender()->SetMaterial(pMtrl);

	}

	else if (2 == ComponentComboBox.GetCurSel())
	{
		m_pTargetObj->AddComponent(new CCollider);
	}

	else if (3 == ComponentComboBox.GetCurSel())
	{
		m_pTargetObj->AddComponent(new CCamera);
	}

	else if (4 == ComponentComboBox.GetCurSel())
	{
		m_pTargetObj->AddComponent(new CAnimator2D);
	}

	else if (5 == ComponentComboBox.GetCurSel())
	{
		MessageBox(_T("3D Animator component has not yet been implemented."), _T("Error"));
	}

	else
	{
		MessageBox(_T("The selected COMPONENET does not exist."), _T("Error"));
		return;
	}

	/*CString str;
	CString strComponent;

	CListBox *p_list = (CListBox *)GetDlgItem(IDC_LIST1);

	int index = p_list->GetCurSel();

	if (index != LB_ERR)
	{
		p_list->GetText(index, str);
	}

	KeyValue.SetWindowTextW(str);


	POINTX.Format(_T("%d"), m_pTargetObj->Animator2D()->FindPOINT((wstring)str).x);
	POINTY.Format(_T("%d"), m_pTargetObj->Animator2D()->FindPOINT((wstring)str).y);




	map<wstring, CAnimation2D*>& mapAnim = m_pTargetObj->Animator2D()->GetAnimMap();
	map<wstring, CAnimation2D*> ::iterator iter = mapAnim.find((wstring)str);

	float fAccTime = iter->second->GetDuration();

	strAccTime.Format(L"%f", fAccTime);

	m_pTargetObj->Animator2D()->GetAnimMap();

	FrameCount.SetWindowTextW(POINTX);
	QuantityCount.SetWindowTextW(POINTY);
	AccTime.SetWindowTextW(strAccTime);


	AccTime.SetWindowTextW(strAccTime);




	UINT ComponentCount = (UINT)m_pTargetObj->GetComponent(COMPONENT_TYPE::END);*/

	//
	//for (size_t i = 0; i < ComponentCount; ++i)
	//{
	//	ComponentComboBox.DeleteItem();
	//}

	/*const map<wstring, CRes*>& mapMesh = CResMgr::GetInst()->GetResourceMap(RESOURCE_TYPE::MESH);

	CListDlg listdlg;
	listdlg.SetCaption(L"Mesh");

	for (auto pair : mapMesh)
	{
		listdlg.AddData(pair.second->GetName().c_str());
	}

	UINT id = listdlg.DoModal();

	if (IDOK == id)
	{
		CString strMesh = listdlg.GetClickItem();
		m_pTargetObj->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(strMesh.GetBuffer()));
	}*/



	
	//m_pTargetObj->AddComponent(new CAnimator2D);


};
