#pragma once


#include "ComponentDlg.h"

// CTransformDlg 대화 상자
class CGameObject;

class CTransformDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CTransformDlg)

public:
	CTransformDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CTransformDlg();

private:	
	bool		 m_bObjUpdate;	

public:
	void update(CGameObject* _pTargetObj);

	bool LayerCheck;

	void LayerNameCheck();
	
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TRANSFORMDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()	
public:
	CEdit	m_arrTrans[9];	
	afx_msg void OnCtrlChange(UINT _id);	
	CComboBox m_LayerList;
	CEdit m_LayerName;
	virtual BOOL OnInitDialog();
	afx_msg void ShowLayer();
	afx_msg void OnEnKillfocusEdit11();
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnCbnSetfocusCombo2();
	afx_msg void OnEnSetfocusEdit11();
};
