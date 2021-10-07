#pragma once


// CBtnView 보기
class CGameObject;
class CBtnView : public CFormView
{
	DECLARE_DYNCREATE(CBtnView)

protected:
	CBtnView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CBtnView();
	
public:
	CGameObject*	m_pTargetObj;
	CGameObject*	pObjectPrefab;
	void SetTargetObject(CGameObject* _pTarget) { m_pTargetObj = _pTarget; }
	static int PrefabLayer;

#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BTNVIEW };
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
	virtual void OnInitialUpdate();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton5();
};


