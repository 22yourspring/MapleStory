#pragma once


// CComponentView 보기
class CTransformDlg;
class CGameObject;
class CComponentDlg;

enum class COMDLG_TYPE
{
	TRANSFORM,
	MESHRENDER,
	COLLIDER,
	CAMERA,
	ANIMATOR2D,
	ANIMATOR3D,	
	SCRIPT,
	END,
};

class CComponentView : public CFormView
{
	DECLARE_DYNCREATE(CComponentView)

protected:
	CComponentView();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~CComponentView();
	
public:
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COMVIEW };
#endif
public:
	virtual void OnDraw(CDC* pDC);      // 이 뷰를 그리기 위해 재정의되었습니다.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	CGameObject*   m_pTargetObj;
	CComponentDlg* m_arrComDlg[(UINT)COMDLG_TYPE::END];
	CButton		   m_btnAddCom;


public:
	void init();
	void update();

	void Lastinit();

	CComponentDlg* GetArrComDlg(int _COMDLG_TYPE = 0) { return m_arrComDlg[_COMDLG_TYPE]; }

public:
	void SetTargetObject(CGameObject* _pTarget) { m_pTargetObj = _pTarget; }

private:
	void ComponentDlgSort();

protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	virtual void DoDataExchange(CDataExchange* pDX);
	afx_msg void AddComponent();

	CComboBox ComponentComboBox;
};


