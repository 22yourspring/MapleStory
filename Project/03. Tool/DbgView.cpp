// DBGView.cpp: 구현 파일
//

#include "stdafx.h"
#include "DbgView.h"

#include "resource.h"

// CDBGView

IMPLEMENT_DYNCREATE(CDbgView, CFormView)

CDbgView::CDbgView()
	: CFormView(IDD_DBGVIEW)
{

}

CDbgView::~CDbgView()
{
}

BEGIN_MESSAGE_MAP(CDbgView, CFormView)
END_MESSAGE_MAP()


// CDBGView 그리기

void CDbgView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}


// CDBGView 진단

#ifdef _DEBUG
void CDbgView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CDbgView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CDBGView 메시지 처리기
