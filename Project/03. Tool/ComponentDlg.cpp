#include "stdafx.h"
#include "ComponentDlg.h"

#include "MainFrm.h"

void CComponentDlg::FocusGameView()
{
	::SetFocus(((CMainFrame*)AfxGetMainWnd())->GetGameViewHwnd());
}

CComponentDlg::CComponentDlg(UINT _id, CWnd * _pParent)
	: CDialogEx(_id, _pParent)
	, m_pTargetObj(nullptr)
	, m_bCtrlClicked(false)
	, m_bEditFocus(false)
{
}

CComponentDlg::~CComponentDlg()
{
}


void CComponentDlg::OnOK(){}
void CComponentDlg::OnCancel(){}


void CComponentDlg::PostNcDestroy()
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	delete this;
	CDialogEx::PostNcDestroy();
}
