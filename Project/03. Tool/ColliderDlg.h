#pragma once

#include "ComponentDlg.h"
// CColliderDlg 대화 상자

class CColliderDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CColliderDlg)

public:
	CColliderDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CColliderDlg();

private:
	bool		 m_bColUpdate;

public:
	void update(CGameObject* _pTargetObj);


	// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_COLLIDERDLG };
#endif


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()


public:
	CEdit	m_arrColl[6];
	afx_msg void OnCtrlChange(UINT _id);
	afx_msg void OnBnClickedButton3();
	CComboBox m_ListColliderTYPE;
	virtual BOOL OnInitDialog();
	afx_msg void OnCbnCloseupCombo2();
};
	