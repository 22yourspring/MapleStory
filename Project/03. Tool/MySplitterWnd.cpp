#include "stdafx.h"
#include "MySplitterWnd.h"

CMySplitterWnd::CMySplitterWnd()
{
}

CMySplitterWnd::~CMySplitterWnd()
{
}


void CMySplitterWnd::SetBorder(int _iGapX, int _iGapY)
{
	m_cxSplitterGap = _iGapX;
	m_cySplitterGap = _iGapY;

	m_cxBorder = 0;
	m_cyBorder = 0;

	m_cxSplitter = 0;
	m_cySplitter = 0;

	m_cxBorderShare = 0;
	m_cyBorderShare = 0;
}


BOOL CMySplitterWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

	cs.dwExStyle &= ~WS_EX_CLIENTEDGE;
	cs.style &= ~WS_BORDER;
	cs.style &= ~WS_THICKFRAME;

	return TRUE;
}
