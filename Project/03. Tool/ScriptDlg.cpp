// ScriptDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ScriptDlg.h"
#include "afxdialogex.h"

#include "resource.h"

#include "Scripts/ScriptMgr.h"
#include "AnrealEngine/GameObject.h"
#include "AnrealEngine/Script.h"

// CScriptDlg 대화 상자

IMPLEMENT_DYNAMIC(CScriptDlg, CDialogEx)

CScriptDlg::CScriptDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_SCRIPTDLG, pParent)
{
}

CScriptDlg::~CScriptDlg()
{
}

void CScriptDlg::update(CGameObject * _pTargetObj)
{
	m_pTargetObj = _pTargetObj;

	m_ctrlScript.DeleteAllItems();

	vector<CScript*>& vecScript = m_pTargetObj->GetScript();
	for (UINT i = 0; i < vecScript.size(); ++i)
	{		
		m_ctrlScript.InsertItem(0, CScriptMgr::GetScriptName(vecScript[i]), 0);
	}
}

void CScriptDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbScripts);
	DDX_Control(pDX, IDC_LIST1, m_ctrlScript);
}


BEGIN_MESSAGE_MAP(CScriptDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CScriptDlg::OnBtnAddScript)
END_MESSAGE_MAP()



BOOL CScriptDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	vector<wstring> vecScriptName;
	CScriptMgr::GetScriptInfo(vecScriptName);

	for (UINT i = 0; i < vecScriptName.size(); ++i)
	{
		m_cbScripts.AddString(vecScriptName[i].c_str());
	}	

	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CScriptDlg::OnBtnAddScript()
{		
	CString strScriptName;
	m_cbScripts.GetLBText(m_cbScripts.GetCurSel(), strScriptName);	
	m_pTargetObj->AddComponent(CScriptMgr::GetScript(strScriptName.GetBuffer()));
}