// ResDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ResDlg.h"
#include "afxdialogex.h"


#include "resource.h"

// CResDlg 대화 상자

IMPLEMENT_DYNAMIC(CResDlg, CDialogEx)

CResDlg::CResDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_RESDLG, pParent)
{

}

CResDlg::~CResDlg()
{
}

void CResDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CResDlg, CDialogEx)
END_MESSAGE_MAP()


// CResDlg 메시지 처리기
