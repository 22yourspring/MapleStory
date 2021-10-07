// HierachyView.cpp: 구현 파일
//

#include "stdafx.h"
#include "HierachyView.h"
#include "resource.h"

#include "HierachyDlg.h"
#include "ResDlg.h"

#include "MainFrm.h"
#include "GameView.h"
#include "AnrealEngine/EventMgr.h"
#include "AnrealEngine/TimeMgr.h"

#include "AnrealEngine/SceneMgr.h"

#include "AnrealEngine/EventMgr.h"
#include "AnrealEngine/GameObject.h"
// CHierachyView

IMPLEMENT_DYNCREATE(CHierachyView, CFormView)

CHierachyView::CHierachyView()
	: CFormView(IDD_HIERACHYVIEW)
	, m_pHierachyDlg(nullptr)
	, m_pResDlg(nullptr)
{

}

CHierachyView::~CHierachyView()
{

}

void CHierachyView::init()
{
	m_pHierachyDlg->init();
	// m_pResDlg->init();
}



void CHierachyView::update()
{
	vector<tEvent>& vecEvent = CEventMgr::GetInst()->GetEventList();

	for (UINT i = 0; i < vecEvent.size(); ++i)
	{
		if (EVENT_TYPE::CREATE_OBJECT == vecEvent[i].eType)
		{
			m_pHierachyDlg->AddGameObject((CGameObject*)vecEvent[i].wParam, nullptr);
		}
	}

	static float m_fAccTime = 0.f;
	
	m_fAccTime += DT;

	/*if (m_fAccTime >= 30.f)
	{
		m_fAccTime = 0.f;
		
		m_pHierachyDlg->Reset();
	}*/
}



void CHierachyView::Reset()
{
	m_pHierachyDlg->Reset();
}


BEGIN_MESSAGE_MAP(CHierachyView, CFormView)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_GAMEOBJECT_CREATEEMPTYOBJECT, &CMainFrame::CreateEmptyObject)
	ON_COMMAND(ID_OBJECT_DELETEOBJECT, &CHierachyView::DeleteObject)
	ON_COMMAND(ID_OBJECT_RESET, &CMainFrame::HierachyReset)

END_MESSAGE_MAP()


// CHierachyView 그리기

void CHierachyView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CHierachyView 진단

#ifdef _DEBUG
void CHierachyView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CHierachyView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CHierachyView 메시지 처리기
int CHierachyView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFormView::OnCreate(lpCreateStruct) == -1)
		return -1;


	// HierachyDlg 생성
	m_pHierachyDlg = new CHierachyDlg;
	m_pHierachyDlg->Create(IDD_HIERACHYDLG, this);
	m_pHierachyDlg->ShowWindow(true);

	// ResDlg 생성
	//m_pResDlg = new CResDlg;
	//m_pResDlg->Create(IDD_RESDLG, this);
	//m_pResDlg->ShowWindow(false);

	return 0;
}

void CHierachyView::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(0, 0));
}

void CHierachyView::OnDestroy()
{
	CFormView::OnDestroy();

	m_pHierachyDlg->DestroyWindow();
	//m_pResDlg->DestroyWindow();
}

void CHierachyView::OnSize(UINT nType, int cx, int cy)
{
	CFormView::OnSize(nType, cx, cy);
	m_pHierachyDlg->SetWindowPos(nullptr, 1, 0, cx - 1, cy / 2, 0);
}


void CHierachyView::OnContextMenu(CWnd* _pWnd, CPoint _point)
{
	CMenu popup;
	CMenu* pMenu;

	popup.LoadMenuW(IDR_MAINFRAME);
	pMenu = popup.GetSubMenu(1);

	pMenu->TrackPopupMenu(TPM_LEFTALIGN || TPM_RIGHTBUTTON, _point.x, _point.y, this);

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CHierachyView::DeleteObject()
{
	if (m_pTargetObj != nullptr)
	{
		CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJECT, (INT_PTR)m_pTargetObj, NULL });
	}

	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
