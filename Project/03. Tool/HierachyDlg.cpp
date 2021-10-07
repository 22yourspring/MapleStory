// HierachyDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "HierachyDlg.h"
#include "afxdialogex.h"

#include "resource.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/Layer.h"
#include "AnrealEngine/GameObject.h"
#include "AnrealEngine/EventMgr.h"

#include "MainFrm.h"
#include "ComponentView.h"
#include "BtnView.h"
#include "HierachyView.h"

#include "Animator2DDlg.h"

// CHierachyDlg 대화 상자

IMPLEMENT_DYNAMIC(CHierachyDlg, CDialogEx)

CHierachyDlg::CHierachyDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_HIERACHYDLG, pParent)
	, m_hDragItem(nullptr)
	, m_hTargetItem(nullptr)
{

}

CHierachyDlg::~CHierachyDlg()
{
}

void CHierachyDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TREE1, m_ctrlTree);	
}

void CHierachyDlg::Reset()
{
	// GameObject 정보를 받아와서 TreeCtrl 에 넣는다.
	m_ctrlTree.DeleteAllItems();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(i);
		if (nullptr == pLayer)
			break;

		vector<CGameObject*>& vecObj = pLayer->GetObjectVec();
		for (auto pObj : vecObj)
		{
			AddGameObject(pObj, nullptr);
		}
	}

	/*CComponentView* pView = (CComponentView*)((CMainFrame*)AfxGetMainWnd())->GetComponentView();

	((CAnimator2DDlg*)((CComponentDlg*)pView->GetArrComDlg((UINT)COMDLG_TYPE::ANIMATOR2D)))->Renewal();
*/
}

void CHierachyDlg::init()
{
	Reset();
}

void CHierachyDlg::AddGameObject(CGameObject * _pObj, HTREEITEM _hParent)
{
	HTREEITEM hItem = AddItem(_pObj->GetName().c_str(), (DWORD_PTR)_pObj, _hParent);

	vector<CGameObject*>& vecChild = _pObj->GetChild();

	for (auto pChildObj : vecChild)
	{
		AddGameObject(pChildObj, hItem);
	}
}


HTREEITEM CHierachyDlg::AddItem(CString _strText, DWORD_PTR _dwData, HTREEITEM _hParent)
{
	TVINSERTSTRUCT st = {};
	st.hParent = _hParent;
	st.hInsertAfter = TVI_LAST;
	st.item.pszText = _strText.GetBuffer();
	st.item.cchTextMax = _strText.GetLength();
	st.item.iImage = 0;
	st.item.iSelectedImage = 0;
	st.item.mask = TVIF_TEXT;

	HTREEITEM hItem = m_ctrlTree.InsertItem(&st);
	m_ctrlTree.SetItemData(hItem, _dwData);

	return hItem;
}

void CHierachyDlg::FocusGameView()
{
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->GetGameViewHwnd());
}

void CHierachyDlg::MoveItem(HTREEITEM _hDragItem, HTREEITEM _hTargetItem)
{
	// 옮길 정보를 다시 추출한다.
	wchar_t szText[255] = {};
	TV_ITEM item = {};
	item.hItem = _hDragItem;
	item.pszText = szText;
	item.cchTextMax = 255;
	item.mask = TVIF_TEXT;

	m_ctrlTree.GetItem(&item);

	DWORD_PTR dwData = m_ctrlTree.GetItemData(_hDragItem);

	// 추출한 정보를 토대로 _hTargetItem 에 새롭게 아이템을 넣는다.
	HTREEITEM hCopyItem = AddItem(szText, dwData, _hTargetItem);

	// 원본 아이템의 자식아이템을 다 뽑아온다.
	vector<HTREEITEM> vecChild;
	GetChildItem(vecChild, _hDragItem);

	for (UINT i = 0; i < vecChild.size(); ++i)
	{
		MoveItem(vecChild[i], hCopyItem);
	}

	// 원본이 펼쳐진 상태면, 복제본도 똑같이 펼쳐둔다.
	if (item.state & TVIS_EXPANDED)
		m_ctrlTree.Expand(hCopyItem, TVE_EXPAND);

	// 목적이 아이템도 펼쳐놓는다.
	m_ctrlTree.Expand(_hTargetItem, TVE_EXPAND);

	// 원본 아이템 삭제
	m_ctrlTree.DeleteItem(_hDragItem);

	// 이동한 아이템(복제버전)을 선택
	m_ctrlTree.SelectItem(hCopyItem);
}

void CHierachyDlg::GetChildItem(vector<HTREEITEM>& _vec, HTREEITEM _hItem)
{
	// 자식이 없으면 리턴
	if (!m_ctrlTree.ItemHasChildren(_hItem))
		return;

	HTREEITEM hChild = m_ctrlTree.GetChildItem(_hItem);

	while (true)
	{
		_vec.push_back(hChild);
		hChild = m_ctrlTree.GetNextSiblingItem(hChild);

		if (nullptr == hChild)
			break;
	}	
}

BEGIN_MESSAGE_MAP(CHierachyDlg, CDialogEx)
	ON_WM_SIZE()
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, &CHierachyDlg::OnTvnSelchanged)
	ON_NOTIFY(TVN_BEGINDRAG, IDC_TREE1, &CHierachyDlg::OnTvnBegindrag)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()

	//ON_UPDATE_COMMAND_UI(ID_OBJECTCONTROL_CREATEOBJECT, &CHierachyDlg::OnUpdateObjectcontrolCreateobject)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, &CHierachyDlg::OnNMDblclkTree1)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, &CHierachyDlg::OnTvnEndlabeleditTree1)
	ON_NOTIFY(NM_KILLFOCUS, IDC_TREE1, &CHierachyDlg::OnNMKillfocusTree1)
END_MESSAGE_MAP()


// CHierachyDlg 메시지 처리기


void CHierachyDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;

	CDialogEx::PostNcDestroy();
}

BOOL CHierachyDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();		
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CHierachyDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);

	if (nullptr == m_ctrlTree.m_hWnd)
		return;

	CRect rt;
	GetClientRect(rt);
	m_ctrlTree.SetWindowPos(nullptr, 0, 0, rt.Width(), rt.Height(), 0);
}


void CHierachyDlg::OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	if (NULL != (pNMTreeView->itemNew.hItem))
	{
		CGameObject* pTargetObj = (CGameObject*)m_ctrlTree.GetItemData(pNMTreeView->itemNew.hItem);

		CComponentView* pView = (CComponentView*)((CMainFrame*)AfxGetMainWnd())->GetComponentView();

		CBtnView* pBtn = (CBtnView*)((CMainFrame*)AfxGetMainWnd())->GetBtnView();

		CHierachyView* pHieView = (CHierachyView*)((CMainFrame*)AfxGetMainWnd())->GetHierachyView();
		
		pView->SetTargetObject(pTargetObj);
		pBtn->SetTargetObject(pTargetObj);
		pHieView->SetTargetObject(pTargetObj);
	}

	*pResult = 0;
}


void CHierachyDlg::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_hDragItem = pNMTreeView->itemNew.hItem;
	   
	SetCapture(); // 자식 윈도우(트리컨트롤) 이 아닌, HierachyDlg 가 마우스 이벤트 독점하기 위해

	*pResult = 0;
}

void CHierachyDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nullptr != m_hDragItem)
	{
		::ClientToScreen(m_hWnd, &point); // Dlg 기준 좌표로 변경
		m_ctrlTree.ScreenToClient(&point);// 다시 treectrl 기준 좌표로 변경

		m_hTargetItem = m_ctrlTree.HitTest(point);

		if (m_hTargetItem == m_hDragItem)
			m_hTargetItem = nullptr;

		if (nullptr != m_hTargetItem)
		{
			m_ctrlTree.SelectDropTarget(m_hTargetItem);
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}

void CHierachyDlg::OnLButtonUp(UINT nFlags, CPoint point)
{	
	CGameObject* pDragObj = nullptr;
	CGameObject * pTargetObj = nullptr;

	ReleaseCapture(); // HierachyDlg 가 가져간 이벤트 권한을 다시 풀어둔다.
	m_ctrlTree.SelectDropTarget(nullptr);
	FocusGameView();

	if (nullptr != m_hDragItem)
	{		
		// 예외처리
		pDragObj = (CGameObject*)m_ctrlTree.GetItemData(m_hDragItem);
		if(nullptr != m_hTargetItem)
			pTargetObj = (CGameObject*)m_ctrlTree.GetItemData(m_hTargetItem);

		if (nullptr == pTargetObj)
		{			
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DISCONNECT_PARENT, (INT_PTR)pDragObj, 0});
			MoveItem(m_hDragItem, m_hTargetItem);
		}
		else if (!pTargetObj->IsAncestor(pDragObj))
		{
			CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::ADD_CHILD, (INT_PTR)pTargetObj, (INT_PTR)pDragObj });
			MoveItem(m_hDragItem, m_hTargetItem);
		}
		else
			int a = 10;
	}
	   	  
	m_hDragItem = nullptr;
	m_hTargetItem = nullptr;
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CHierachyDlg::OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	m_ctrlTree.EditLabel(m_ctrlTree.GetSelectedItem());

	*pResult = 0;
}




void CHierachyDlg::OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMTVDISPINFO pTVDispInfo = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	

	CEdit* edit = m_ctrlTree.GetEditControl();
	HTREEITEM hItem = pTVDispInfo->item.hItem;
	CString str;

	edit->GetWindowTextW(str);
	m_ctrlTree.SetItemText(pTVDispInfo->item.hItem, str);
	CGameObject* pObj = (CGameObject*)m_ctrlTree.GetItemData(hItem);
	pObj->SetName(str.GetBuffer());



	*pResult = 0;

		//for (UINT i = 0; i < MAX_LAYER; ++i)
		//{
		//	CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(i);
		//	if (nullptr == pLayer)
		//		break;

		//	vector<CGameObject*>& vecObj = pLayer->GetObjectVec();
		//	for (auto pObj : vecObj)
		//	{
		//		/*if (selectedItem == (DWORD_PTR)pObj)
		//		{
		//			pObj->SetName(wcsName);
		//			m_ctrlTree.SetItemText(hSelectedItem, wcsName.c_str());
		//		}*/
		//		//AddGameObject(pObj, nullptr);
		//	}
		//}






	//DWORD_PTR selectedItem;
	//HTREEITEM hSelectedItem = m_ctrlTree.GetSelectedItem();

	//if (hSelectedItem)
	//{
	//	selectedItem = m_ctrlTree.GetItemData(hSelectedItem);
	//}


	//wstring wcsName;
	//
	//if (pTVDispInfo->item.pszText)
	//{
	//	wcsName = pTVDispInfo->item.pszText;

	//	

	//	for (UINT i = 0; i < MAX_LAYER; ++i)
	//	{
	//		CLayer* pLayer = CSceneMgr::GetInst()->GetLayer(i);
	//		if (nullptr == pLayer)
	//			break;

	//		vector<CGameObject*>& vecObj = pLayer->GetObjectVec();
	//		for (auto pObj : vecObj)
	//		{
	//			if (selectedItem == (DWORD_PTR)pObj)
	//			{
	//				pObj->SetName(wcsName);
	//				m_ctrlTree.SetItemText(hSelectedItem, wcsName.c_str());
	//			}
	//			//AddGameObject(pObj, nullptr);
	//		}
	//	}

	//}



	//void CHierachyDlg::OnEditLabel(NMHDR *pNMHDR, LRESULT *pResult)
	//{
	//	LPNMTVDISPINFO pTV = reinterpret_cast<LPNMTVDISPINFO>(pNMHDR);
	//	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	//	CEdit* edit = m_ctrlTree.GetEditControl();
	//	HTREEITEM hItem = pTV->item.hItem;
	//	CString str;

	//	edit->GetWindowTextW(str);
	//	m_ctrlTree.SetItemText(pTV->item.hItem, str);
	//	CGameObject* pObj = (CGameObject*)m_ctrlTree.GetItemData(hItem);
	//	pObj->SetName(str.GetBuffer());

	//	*pResult = 0;
	//}



/*
	CHierachyDlg* pView = (CHierachyDlg*)((CMainFrame*)AfxGetMainWnd())->GetHierachyView();
	pView->
*/
	if (hItem)
	{
		CGameObject* pTargetObj = (CGameObject*)m_ctrlTree.GetItemData(hItem);
		CComponentView* pView = (CComponentView*)((CMainFrame*)AfxGetMainWnd())->GetComponentView();

		pView->SetTargetObject(pTargetObj);
	}
	
	*pResult = 0;

}

void CHierachyDlg::OnNMKillfocusTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	//	m_ctrlTree.SelectItem(nullptr); //마우스 포커스를 잃었을 때 0번째 자동 선택

	*pResult = 0;
}
