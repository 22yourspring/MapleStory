#pragma once

class CGameObject;

class CComponentDlg : public CDialogEx
{
protected:
	CGameObject*		m_pTargetObj;
	bool				m_bCtrlClicked;
	bool				m_bEditFocus;

public:
	virtual void init() {}
	virtual void update(CGameObject* _pTargetObj) = 0;
	
protected:
	void FocusGameView();

public:
	CComponentDlg(UINT _id, CWnd* _pParent);
	virtual ~CComponentDlg();
	virtual void OnOK();
	virtual void OnCancel();
	virtual void PostNcDestroy();

	afx_msg void OnEditFocused(UINT _id) { m_bEditFocus = true; }
	afx_msg void OnEditKillFocused(UINT _id) { m_bEditFocus = false; }
};

