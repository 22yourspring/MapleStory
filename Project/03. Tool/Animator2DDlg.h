#pragma once


#include "ComponentDlg.h"


// CAnimator2DDlg 대화 상자

class CTexture;
class CAnimator2DDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CAnimator2DDlg)

public:
	CAnimator2DDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CAnimator2DDlg();
	virtual void init();

	virtual void update(CGameObject* _pTargetObj);

	void Renewal();

	void resizeObj(CTexture* _pTargetImage, POINT _SIZE);
	
	POINT startLocation(CTexture* _pTex, int _quantityCount, int _animLoc);

public:
	wstring strAnimPath;
	wstring strAnimKey;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ANIMATOR2DDLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CComboBox FRAME_Anim;

	virtual BOOL OnInitDialog();
	afx_msg void OpenDirectory();

	afx_msg void AddAnim();
	CEdit AccTime;
	CEdit KeyValue;
	CEdit AnimKey;

	CListBox listAnim;
	afx_msg void DelAnim();
	afx_msg void OnAnimPlay();
	afx_msg void GetCurItem();
	CEdit FrameCount;
	CEdit QuantityCount;
	afx_msg void OnModificAcc();
	CEdit AnimLocation;
	CEdit AnimValue;	
	CButton m_FullAtlas;
	CButton RepeatChk;
	CListBox AnimVec;
	afx_msg void AnimVecCursel();
	CEdit OffsetX;
	CEdit OffsetY;
	afx_msg void ReSetClicked();
};
