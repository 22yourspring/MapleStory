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
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
	delete this;
	CDialogEx::PostNcDestroy();
}
