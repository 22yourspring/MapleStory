// ListDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ListDlg.h"
#include "afxdialogex.h"


#include "resource.h"

// CListDlg 대화 상자

IMPLEMENT_DYNAMIC(CListDlg, CDialogEx)

CListDlg::CListDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_LISTDLG, pParent)
{

}

CListDlg::~CListDlg()
{
}

void CListDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST1, m_ctrlList);
}


BEGIN_MESSAGE_MAP(CListDlg, CDialogEx)
	ON_WM_CREATE()
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, &CListDlg::OnNMDblclk)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CListDlg::OnLvnItemchangedList1)
	ON_NOTIFY(NM_RCLICK, IDC_LIST1, &CListDlg::OnNMRClickList1)
	ON_NOTIFY(NM_SETFOCUS, IDC_LIST1, &CListDlg::OnNMSetfocusList1)
	ON_NOTIFY(NM_KILLFOCUS, IDC_LIST1, &CListDlg::OnNMKillfocusList1)
END_MESSAGE_MAP()


void CListDlg::OnCancel()
{
	EndDialog(IDCANCEL);
}
void CListDlg::OnOK(){}


BOOL CListDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 받아온 문자열로 캡션 설정
	SetWindowText(m_strCaption);

	m_ctrlList.InsertColumn(0, L"");

	// 받아온 목록 문자열 정보를 토대로 리스트컨트롤 초기화
	for (UINT i = 0; i < m_vecData.size(); ++i)
	{
		m_ctrlList.InsertItem(0, m_vecData[i], 0);
	}	
	return TRUE; 				 
}


void CListDlg::OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	m_strClick = m_ctrlList.GetItemText(pNMItemActivate->iItem, 0);
	*pResult = 0;

	EndDialog(IDOK);
}


void CListDlg::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;


	
}


void CListDlg::OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	
	UNREFERENCED_PARAMETER(pNMItemActivate);



	CListDlg listdlg;
	if (m_bMouseFocus)
	{
	//	listdlg.OnDeleteItem();
	}
	
	
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;

}


void CListDlg::OnNMSetfocusList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_bMouseFocus = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}


void CListDlg::OnNMKillfocusList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	m_bMouseFocus = false;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	*pResult = 0;
}
