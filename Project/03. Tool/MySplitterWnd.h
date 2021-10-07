#pragma once
#include <afxext.h>

class CMySplitterWnd :
	public CSplitterWnd
{
public:
	void SetBorder(int _iGapX, int _iGapY);

public:
	CMySplitterWnd();
	virtual ~CMySplitterWnd();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};

