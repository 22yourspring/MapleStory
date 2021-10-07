#pragma once


// CListDlg 대화 상자

class CListDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CListDlg)
private:
	CString			m_strCaption;
	CString			m_strClick;
	vector<CString> m_vecData;

	bool m_bMouseFocus = false;

public:
	CListDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CListDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LISTDLG };
#endif

public:
	void SetCaption(const CString& _strCaption)
	{
		m_strCaption = _strCaption;
	}
	
	void AddData(const CString& _strData)
	{
		m_vecData.push_back(_strData);
	}

	const CString& GetClickItem() { return m_strClick; };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	DECLARE_MESSAGE_MAP()
	virtual void OnCancel();
	virtual void OnOK();
public:	
	virtual BOOL OnInitDialog();
private:
	CListCtrl m_ctrlList;
public:
	afx_msg void OnNMDblclk(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMRClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMSetfocusList1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMKillfocusList1(NMHDR *pNMHDR, LRESULT *pResult);
};
