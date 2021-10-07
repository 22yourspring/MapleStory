
// MainFrm.h: CMainFrame 클래스의 인터페이스
//

#pragma once

#include "MySplitterWnd.h"

class CMainFrame : public CFrameWnd
{
	
public:
	CMainFrame() noexcept;
protected: 
	DECLARE_DYNAMIC(CMainFrame)

// 특성입니다.
private:
	CMySplitterWnd	m_MainSplitter;
	CMySplitterWnd  m_SubSlpitter;


// 작업입니다.
public:

// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual BOOL OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

public:
	HWND GetGameViewHwnd() { return m_SubSlpitter.GetPane(1, 0)->m_hWnd; }

	CWnd* GetGameView() { return m_SubSlpitter.GetPane(1, 0); }
	CWnd* GetComponentView() { return m_MainSplitter.GetPane(0, 2); }
	CWnd* GetHierachyView() { return m_MainSplitter.GetPane(0, 1); }
	CWnd* GetBtnView() { return m_SubSlpitter.GetPane(0, 0); }
	CWnd* GetDbgView() { return m_SubSlpitter.GetPane(2, 0); }

	void init();
	void update();






// 생성된 메시지 맵 함수
protected:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSetFocus(CWnd *pOldWnd);
	afx_msg void OnApplicationLook(UINT id);
	afx_msg void OnUpdateApplicationLook(CCmdUI* pCmdUI);
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void CreateEmptyObject();
	afx_msg void SceneSave();
	afx_msg void SceneLoad();

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void HierachyReset();
};


