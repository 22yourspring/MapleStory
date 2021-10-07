#pragma once


#include "ComponentDlg.h"

class CMeshRenderDlg : public CComponentDlg
{
	DECLARE_DYNAMIC(CMeshRenderDlg)

public:
	CMeshRenderDlg(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CMeshRenderDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MESHRENDERDLG };
#endif

public:
	virtual void update(CGameObject* _pTargetObj);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	CEdit m_EditMesh;
	CEdit m_EditMtrl;
	afx_msg void OnChangeMeshBtn();
	afx_msg void OnChangeMtrlBtn();
};
