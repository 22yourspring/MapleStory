#pragma once


// CHierachyView 보기
class CHierachyDlg;
class CResDlg;
class CGameObject;

class CHierachyView : public CFormView
{
	DECLARE_DYNCREATE(CHierachyView)

protected:
	CHierachyView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CHierachyView();

private:
	CHierachyDlg* m_pHierachyDlg;
	CResDlg*	  m_pResDlg;

	CGameObject*   m_pTargetObj;

public:
	void SetTargetObject(CGameObject* _pTarget) { m_pTargetObj = _pTarget; }

public:
	void init();
	void update();
	void Reset();
	
	CHierachyDlg* GetHierachyDlg() { return m_pHierachyDlg; }
	
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIERACHYVIEW };
#endif
public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	virtual void OnInitialUpdate();
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg void DeleteObject();
};


