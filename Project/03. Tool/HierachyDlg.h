#pragma once

// CHierachyDlg 대화 상자

class CGameObject;

class CHierachyDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CHierachyDlg)

public:
	CHierachyDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CHierachyDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_HIERACHYDLG };
#endif
	
	// Hierachy
public:
	void AddGameObject(CGameObject* _pObj, HTREEITEM _hParent);

public:
	void Reset();
	void CHierachyDlg::OnOK() {}
	void CHierachyDlg::OnCancel() {}

	// TreeDlg
private:
	HTREEITEM	m_hDragItem; // Drag 중인 아이텝
	HTREEITEM	m_hTargetItem; // Drop 예정 아이템


public:
	CTreeCtrl m_ctrlTree;	
public:
	virtual void init();
	HTREEITEM AddItem(CString _strText, DWORD_PTR _dwData, HTREEITEM _hParent);
	void FocusGameView();
	void MoveItem(HTREEITEM _hDragItem, HTREEITEM _hTargetItem);
	void GetChildItem(vector<HTREEITEM>& _vec, HTREEITEM _hItem);






protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual void PostNcDestroy();
public:
	virtual BOOL OnInitDialog();

	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTvnSelchanged(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	//afx_msg void OnUpdateObjectcontrolCreateobject(CCmdUI *pCmdUI);
	afx_msg void OnNMDblclkTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTvnEndlabeleditTree1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
