
// MainFrm.cpp: CMainFrame 클래스의 구현
//

#include "stdafx.h"
#include "Tool.h"

#include "MainFrm.h"
#include "BtnView.h"
#include "DBGView.h"
#include "GameView.h"
#include "HierachyView.h"
#include "ComponentView.h"

#include "AnrealEngine/TimeMgr.h"

#include "resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
	ON_WM_SETFOCUS()
	ON_COMMAND_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnApplicationLook)
	ON_UPDATE_COMMAND_UI_RANGE(ID_VIEW_APPLOOK_WIN_2000, ID_VIEW_APPLOOK_WINDOWS_7, &CMainFrame::OnUpdateApplicationLook)
	ON_COMMAND(ID_GAMEOBJECT_CREATEEMPTYOBJECT, &CMainFrame::CreateEmptyObject)
	ON_COMMAND(ID_32771, &CMainFrame::SceneSave)
	ON_COMMAND(ID_32772, &CMainFrame::SceneLoad)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_OBJECT_RESET, &CMainFrame::HierachyReset)
END_MESSAGE_MAP()

// CMainFrame 생성/소멸

CMainFrame::CMainFrame() noexcept
{
	// TODO: 여기에 멤버 초기화 코드를 추가합니다.
}

CMainFrame::~CMainFrame()
{
	//CSceneMgr::GetInst()->GetCurScene();
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;		

	RECT rt = { 0, 0, FRAME_X, FRAME_Y };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, true);
	SetWindowPos(nullptr, 10, 10, rt.right - rt.left, rt.bottom - rt.top, 0);
		
	// 1행 3열
	m_MainSplitter.CreateStatic(this, 1, 3);
	m_MainSplitter.SetColumnInfo(0, GAMEVIEW_X, GAMEVIEW_X);
	m_MainSplitter.SetBorder(1, 1);
		
	m_MainSplitter.CreateView(0, 1, RUNTIME_CLASS(CHierachyView), CSize(HIERACHYVIEW_X, FRAME_Y), nullptr);
	m_MainSplitter.CreateView(0, 2, RUNTIME_CLASS(CComponentView), CSize(COMVIEW_X, FRAME_Y), nullptr);

	m_SubSlpitter.CreateStatic(&m_MainSplitter, 3, 1, WS_CHILD | WS_VISIBLE, m_MainSplitter.IdFromRowCol(0, 0));
	m_SubSlpitter.SetBorder(1, 1);

	m_SubSlpitter.CreateView(0, 0, RUNTIME_CLASS(CBtnView), CSize(BTNVIEW_X, BTNVIEW_Y), nullptr);
	m_SubSlpitter.CreateView(1, 0, RUNTIME_CLASS(CGameView), CSize(GAMEVIEW_X, GAMEVIEW_Y), nullptr);
	m_SubSlpitter.CreateView(2, 0, RUNTIME_CLASS(CDbgView), CSize(DBGVIEW_X, DBGVIEW_Y), nullptr);
	   	 
	GetGameView()->SetFocus();

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.lpszClass = AfxRegisterWndClass(0);
	return TRUE;
}

// CMainFrame 진단

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG

void CMainFrame::init()
{
	// GameView

	// ComponentView
	CComponentView* pComView = (CComponentView*)GetComponentView();
	pComView->init();


	// HierachyView
	CHierachyView* pHView = (CHierachyView*)GetHierachyView();
	pHView->init();
}

void CMainFrame::update()
{
	CHierachyView* pHierachyView = (CHierachyView*)GetHierachyView();
	pHierachyView->update();

	// 0.02 초마다 Tool Update
	static float fAcc = 0.f;
	fAcc += DT;
	if (fAcc < 0.02f)
		return;	

	std::cout << CTimeMgr::GetInst()->FPS() << endl;

	fAcc = 0.f;

	CComponentView* pComView = (CComponentView*)GetComponentView();
	pComView->update();
}

// CMainFrame 메시지 처리기

void CMainFrame::OnSetFocus(CWnd* /*pOldWnd*/)
{
	// 뷰 창으로 포커스를 이동합니다.
	
}

BOOL CMainFrame::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo)
{
	
	// 그렇지 않으면 기본 처리합니다.
	return CFrameWnd::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}

void CMainFrame::OnApplicationLook(UINT id)
{
	CWaitCursor wait;

	theApp.m_nAppLook = id;

	switch (theApp.m_nAppLook)
	{
	case ID_VIEW_APPLOOK_WIN_2000:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManager));
		break;

	case ID_VIEW_APPLOOK_OFF_XP:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOfficeXP));
		break;

	case ID_VIEW_APPLOOK_WIN_XP:
		CMFCVisualManagerWindows::m_b3DTabsXPTheme = TRUE;
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));
		break;

	case ID_VIEW_APPLOOK_OFF_2003:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2003));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2005:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2005));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_VS_2008:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerVS2008));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	case ID_VIEW_APPLOOK_WINDOWS_7:
		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows7));
		CDockingManager::SetDockingMode(DT_SMART);
		break;

	default:
		switch (theApp.m_nAppLook)
		{
		case ID_VIEW_APPLOOK_OFF_2007_BLUE:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_LunaBlue);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_BLACK:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_ObsidianBlack);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_SILVER:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Silver);
			break;

		case ID_VIEW_APPLOOK_OFF_2007_AQUA:
			CMFCVisualManagerOffice2007::SetStyle(CMFCVisualManagerOffice2007::Office2007_Aqua);
			break;
		}

		CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerOffice2007));
		CDockingManager::SetDockingMode(DT_SMART);
	}

	RedrawWindow(nullptr, nullptr, RDW_ALLCHILDREN | RDW_INVALIDATE | RDW_UPDATENOW | RDW_FRAME | RDW_ERASE);

}

void CMainFrame::OnUpdateApplicationLook(CCmdUI* pCmdUI)
{
	pCmdUI->SetRadio(theApp.m_nAppLook == pCmdUI->m_nID);
}

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/GameObject.h"
#include "AnrealEngine/PathMgr.h"
#include "SaveLoadMgr.h"


#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Transform.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/ResMgr.h"

void CMainFrame::CreateEmptyObject()
{
	CGameObject* pNewObject = new CGameObject;
	pNewObject->SetName(L"Object");
	CSceneMgr::GetInst()->AddGameObject(L"Default", pNewObject);




	((CHierachyView*)(((CMainFrame*)AfxGetMainWnd())->GetHierachyView()))->Reset();

//	((CHierachyView*)GetHierachyView())->Reset();

	/*delete pDefault;
	delete pMtrl;

	pDefault = nullptr;
	pMtrl = nullptr;*/
}


void CMainFrame::SceneSave()
{
	wstring strScenePath = CPathMgr::GetResPath();
	strScenePath += L"Scene\\";

	// FileDialog
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(FALSE, L"Scene", L"*.scene",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strScenePath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	strScenePath = dlg.GetPathName();
	
	// strPath 경로로 파일 생성
	CSaveLoadMgr::SaveScene(strScenePath);
}

void CMainFrame::SceneLoad()
{
	wstring strScenePath = CPathMgr::GetResPath();
	strScenePath += L"Scene\\";

	// FileDialog
	wchar_t szFilter[50] = L"";
	CFileDialog dlg(TRUE, L"Scene", L"*.scene",
		OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

	dlg.m_ofn.lpstrInitialDir = strScenePath.c_str();

	if (dlg.DoModal() != IDOK)
		return;

	strScenePath = dlg.GetPathName();

	// strPath 경로로 파일 생성
	CSaveLoadMgr::LoadScene(strScenePath);
	((CComponentView*)GetComponentView())->SetTargetObject(nullptr);
}


BOOL CMainFrame::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_ESCAPE) {
		return TRUE;
	}
	else if (pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		return TRUE;
	}
	else if (pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) {
		return TRUE;
	}
	else if (pMsg->message == WM_SYSKEYDOWN)
	{
		return TRUE;
	}
	
	return CFrameWnd::PreTranslateMessage(pMsg);
}


void CMainFrame::HierachyReset()
{
	((CHierachyView*)(((CMainFrame*)AfxGetMainWnd())->GetHierachyView()))->Reset();
	// TODO: 여기에 명령 처리기 코드를 추가합니다.
}
