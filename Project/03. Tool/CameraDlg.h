#pragma once


#include "ComponentDlg.h"

class CCameraDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CCameraDlg)

private:
	bool			m_bCBFocus;
	bool			m_bCBLayerMaskFocus;

public:
	virtual void init();
	virtual void update(CGameObject* _pTargetObj);

public:
	CCameraDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CCameraDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CAMERADLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

private:
	void SetEverything();
	void SetNothing();	


private:
	CComboBox m_cbProject;
public:
	virtual BOOL OnInitDialog();
private:
	CEdit m_Far;
	CEdit m_FOV;
	CEdit m_Scale;
public:
	afx_msg void OnCbnKillfocusCombo1();
	afx_msg void OnCbnSetfocusCombo1();

	afx_msg void OnLayerMaskKillFocus();
	afx_msg void OnLayerMaskSetFocus();



	afx_msg void OnCbnSelchangeCombo1();
private:	
	CImageList  m_imglist;
	CComboBoxEx m_cbCullingMask;
public:
	afx_msg void OnLayerMaskChange();
	afx_msg void OnCbnDropdownComboboxex1();
};
