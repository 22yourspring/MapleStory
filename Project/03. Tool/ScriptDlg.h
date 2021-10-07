#pragma once

#include "ComponentDlg.h"

class CScriptDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CScriptDlg)

public:
	CScriptDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CScriptDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_SCRIPTDLG };
#endif

public:
	virtual void update(CGameObject* _pTargetObj);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	// 모든 스크립트 항목 리스트
	CComboBox m_cbScripts;	
	CListCtrl m_ctrlScript;

	virtual BOOL OnInitDialog();
	afx_msg void OnBtnAddScript();
};
