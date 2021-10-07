// BtnView.cpp: 구현 파일
//

#include "stdafx.h"
#include "BtnView.h"
#include "resource.h"

#include "AnrealEngine/GameObject.h"
#include "AnrealEngine/ResMgr.h"
#include "AnrealEngine/SceneMgr.h"

#include "HierachyView.h"
#include "MainFrm.h"


#include "AnrealEngine/Layer.h"


#include "AnrealEngine/EventMgr.h"
//#include "AnrealEngine/MeshRender.h"
//#include "AnrealEngine/Mesh.h"
//#include "AnrealEngine/Material.h"

IMPLEMENT_DYNCREATE(CBtnView, CFormView)

int CBtnView::PrefabLayer = -1;

CBtnView::CBtnView()
	: CFormView(IDD_BTNVIEW)
	, pObjectPrefab (nullptr)
	, m_pTargetObj (nullptr)
{

}

CBtnView::~CBtnView()
{
}

BEGIN_MESSAGE_MAP(CBtnView, CFormView)
	ON_BN_CLICKED(IDC_BUTTON7, &CBtnView::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON3, &CBtnView::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON5, &CBtnView::OnBnClickedButton5)
END_MESSAGE_MAP()


// CBtnView 그리기

void CBtnView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CBtnView 진단

#ifdef _DEBUG
void CBtnView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CBtnView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CBtnView 메시지 처리기


void CBtnView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(0, 0));
}


void CBtnView::OnBnClickedButton7()
{
	if (m_pTargetObj == nullptr)
	{
		return;
	}
	pObjectPrefab = m_pTargetObj;

	CResMgr::GetInst()->AddResource<CPrefab>(m_pTargetObj->GetName(), new CPrefab(pObjectPrefab->Clone()));

	PrefabLayer = m_pTargetObj->GetLayerIdx();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CBtnView::OnBnClickedButton3()
{
	// Prefab기능
	if (pObjectPrefab == nullptr)
	{
		return;
	}

	Ptr<CPrefab> PrefabObject = CResMgr::GetInst()->Load<CPrefab>(pObjectPrefab->GetName());

	/*if (PrefabObject == nullptr)
	{
		return;
	}*/
	CSceneMgr::GetInst()->GetCurScene()->AddGameObject(PrefabLayer, PrefabObject->Instantiate());

	((CHierachyView*)(((CMainFrame*)AfxGetMainWnd())->GetHierachyView()))->Reset();




	/*for (size_t i = 0; i < 299; ++i)
	{
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJECT, (INT_PTR)(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"EFFECT_LAYER")->GetObjectVec()[0]), NULL });
	}*/

	/*for (size_t i = 0; i < 300; ++i)
	{
		CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"EFFECT_LAYER")->GetObjectVec()[0]->Dele;
	}*/


	/*int i = 0;
	while (true)
	{
		if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"EFFECT_LAYER")->GetObjectVec()[0]->GetName() == L"EFFECT_OBJECT")
		{
			CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"EFFECT_DUMMY")->GetObjectVec()[0]->AddChild(CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"EFFECT_LAYER")->GetObjectVec()[0]);
			++i;
		}

		if (i == 300)
		{
			((CHierachyView*)(((CMainFrame*)AfxGetMainWnd())->GetHierachyView()))->Reset();
			break;
		}
	}*/



	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CBtnView::OnBnClickedButton5()
{
	if (CSceneMgr::GetInst()->GetCurScene()->SetUpdatePlay == true)
	{
		CSceneMgr::GetInst()->GetCurScene()->SetUpdatePlay = false;
	}
	else
	{
		CSceneMgr::GetInst()->GetCurScene()->SetUpdatePlay = true;
	}
	
}
