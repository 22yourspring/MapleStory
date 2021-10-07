// CameraDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "CameraDlg.h"
#include "afxdialogex.h"

#include "resource.h"
#include "MainFrm.h"

#include "AnrealEngine/GameObject.h"
#include "AnrealEngine/Camera.h"

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Layer.h"


IMPLEMENT_DYNAMIC(CCameraDlg, CDialogEx)

CCameraDlg::CCameraDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_CAMERADLG, pParent)
	, m_bCBFocus(false)
{

}

CCameraDlg::~CCameraDlg()
{
}

void CCameraDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, m_cbProject);
	DDX_Control(pDX, IDC_EDIT1, m_Far);
	DDX_Control(pDX, IDC_EDIT2, m_FOV);
	DDX_Control(pDX, IDC_EDIT3, m_Scale);
	DDX_Control(pDX, IDC_COMBOBOXEX1, m_cbCullingMask);

	m_cbProject.AddString(L"Orthographic");
	m_cbProject.AddString(L"Perspective");


	m_imglist.Create(16, 16, ILC_COLOR, 2, 2);
	m_imglist.Add(AfxGetApp()->LoadIcon(IDI_ICON1));
	m_imglist.Add(AfxGetApp()->LoadIcon(IDR_MAINFRAME));
	   

	m_bCBFocus = false;
	
}

void CCameraDlg::SetEverything()
{
	m_pTargetObj->Camera()->CheckLayerAll();

	wchar_t szText[255] = {};
	COMBOBOXEXITEM  item{};
	item.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	item.pszText = szText;
	item.cchTextMax = 255;	

	item.iItem = 0;
	m_cbCullingMask.GetItem(&item);
	item.iImage = 0;
	m_cbCullingMask.SetItem(&item);

	item.iItem = 2;
	m_cbCullingMask.GetItem(&item);
	item.iImage = 0;
	m_cbCullingMask.SetItem(&item);

	int iMaxCount = m_cbCullingMask.GetCount();
	for (int i = 1; i < iMaxCount; ++i)
	{
		item.iItem = i;
		m_cbCullingMask.GetItem(&item);
		item.iImage = 1;
		m_cbCullingMask.SetItem(&item);
	}

	m_cbCullingMask.SetCurSel(1);
}

void CCameraDlg::SetNothing()
{
	m_pTargetObj->Camera()->CheckLayerClear();

	wchar_t szText[255] = {};
	COMBOBOXEXITEM  item{};
	item.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	item.pszText = szText;
	item.cchTextMax = 255;

	item.iItem = 0;
	m_cbCullingMask.GetItem(&item);
	item.iImage = 1;
	m_cbCullingMask.SetItem(&item);

	int iMaxCount = m_cbCullingMask.GetCount();
	for (int i = 1; i < iMaxCount; ++i)
	{
		item.iItem = i;
		m_cbCullingMask.GetItem(&item);
		item.iImage = 0;
		m_cbCullingMask.SetItem(&item);
	}

	m_cbCullingMask.SetCurSel(0);
}

void CCameraDlg::init()
{
	m_cbCullingMask.ResetContent();
	m_cbCullingMask.SetImageList(&m_imglist);

	COMBOBOXEXITEM  item;
	ZeroMemory(&item, sizeof(item));
	item.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY |
		CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	item.iItem = 0;
	item.pszText = L"Nothing";
	item.cchTextMax = (int)wcslen(item.pszText);
	item.iImage = 0;
	item.iSelectedImage = 1;
	item.iOverlay = 0;
	m_cbCullingMask.InsertItem(&item);
	
	item.iItem = 1;
	item.pszText = L"Everything";
	m_cbCullingMask.InsertItem(&item);

	item.iItem = 2;
	item.pszText = L"Mixed ...";
	m_cbCullingMask.InsertItem(&item);
	
	for (INT i = 0; i < MAX_LAYER; ++i)
	{
		if (nullptr == CSceneMgr::GetInst()->GetLayer(i))
			break;
		
		COMBOBOXEXITEM  item;
		ZeroMemory(&item, sizeof(item));
		item.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY |
						CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
		item.iItem = i + 3;		
		item.pszText = (wchar_t*)CSceneMgr::GetInst()->GetLayer(i)->GetName().c_str();
		item.cchTextMax = (int)wcslen(item.pszText);
		item.iImage = 0;		
		item.iSelectedImage = 1;
		item.iOverlay = 0;

		int iResult = m_cbCullingMask.InsertItem(&item);
		int a = 10;
	}	

	m_cbCullingMask.SetCurSel(0);
}

void CCameraDlg::update(CGameObject * _pTargetObj)
{
	m_pTargetObj = _pTargetObj;
	CCamera* pCam = _pTargetObj->Camera();
	
	if (m_bEditFocus || m_bCBFocus || m_bCBLayerMaskFocus)
		return;

	CString str;
	str.Format(L"%f", pCam->GetFar());	
	m_Far.SetWindowTextW(str);

	str.Format(L"%f", pCam->GetFOV());
	m_FOV.SetWindowTextW(str);

	str.Format(L"%f", pCam->GetScale());
	m_Scale.SetWindowTextW(str);
	
	if (pCam->IsPerspective())
	{
		m_cbProject.SetCurSel(1);
		m_FOV.EnableWindow(true);
		m_Scale.EnableWindow(false);
	}
	else
	{
		m_cbProject.SetCurSel(0);
		m_FOV.EnableWindow(false);
		m_Scale.EnableWindow(true);
	}

	// LayerMask 정보 Update
	UINT iLayerMask = m_pTargetObj->Camera()->GetLayerMask();

	wchar_t szText[255] = {};
	COMBOBOXEXITEM  item{};
	item.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
	item.pszText = szText;
	item.cchTextMax = 255;

	if (iLayerMask == 0xffffffff)
	{
		SetEverything(); // Everything
	}
	else if (iLayerMask == 0) // Nothing
	{
		SetNothing();
	}
	else // Mixed
	{
		item.iItem = 0;
		m_cbCullingMask.GetItem(&item);
		item.iImage = 0;
		m_cbCullingMask.SetItem(&item);

		item.iItem = 1;
		m_cbCullingMask.GetItem(&item);
		item.iImage = 0;
		m_cbCullingMask.SetItem(&item);

		for (int i = 0; i < MAX_LAYER; ++i)
		{
			if (iLayerMask & 1 << i)
			{
				item.iItem = i + 3;
				m_cbCullingMask.GetItem(&item);
				item.iImage = 1;
				m_cbCullingMask.SetItem(&item);
			}
		}		
		m_cbCullingMask.SetCurSel(2);
		//m_cbCullingMask.SetWindowTextW(L"Mixed ...");	
	}	
}

BEGIN_MESSAGE_MAP(CCameraDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT3, &CComponentDlg::OnEditFocused)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT3, &CComponentDlg::OnEditKillFocused)
	
	ON_CBN_KILLFOCUS(IDC_COMBO1, &CCameraDlg::OnCbnKillfocusCombo1)
	ON_CBN_SETFOCUS(IDC_COMBO1, &CCameraDlg::OnCbnSetfocusCombo1)

	ON_CBN_KILLFOCUS(IDC_COMBOBOXEX1, &CCameraDlg::OnLayerMaskKillFocus)
	ON_CBN_SETFOCUS(IDC_COMBOBOXEX1, &CCameraDlg::OnLayerMaskSetFocus)

	ON_CBN_SELCHANGE(IDC_COMBO1, &CCameraDlg::OnCbnSelchangeCombo1)
	ON_CBN_SELCHANGE(IDC_COMBOBOXEX1, &CCameraDlg::OnLayerMaskChange)
	ON_CBN_DROPDOWN(IDC_COMBOBOXEX1, &CCameraDlg::OnCbnDropdownComboboxex1)
END_MESSAGE_MAP()

BOOL CCameraDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();	

	return TRUE;				  
}

void CCameraDlg::OnCbnSetfocusCombo1()
{
	m_bCBFocus = true;
}

void CCameraDlg::OnLayerMaskKillFocus()
{
	m_bCBLayerMaskFocus = false;
}

void CCameraDlg::OnLayerMaskSetFocus()
{
	m_bCBLayerMaskFocus = true;
}

void CCameraDlg::OnCbnKillfocusCombo1()
{
	m_bCBFocus = false;
}

void CCameraDlg::OnCbnSelchangeCombo1()
{
	// 0 -> 직교, 1 -> 원근
	int iIdx = m_cbProject.GetCurSel();

	if (0 == iIdx)
		m_pTargetObj->Camera()->SetPerspective(false);
	else
		m_pTargetObj->Camera()->SetPerspective(true);
	
	FocusGameView();
}

void CCameraDlg::OnLayerMaskChange()
{
	assert(m_pTargetObj);

	// 촬영 Layer 변경 시
	// 0 : Nothing
	// 1 : Everything
	// 
	int iIdx = m_cbCullingMask.GetCurSel();
	
	if (0 == iIdx)
	{
		SetNothing();
	}
	else if (1 == iIdx)
	{
		SetEverything();
	}	
	else
	{
		m_pTargetObj->Camera()->CheckLayer(iIdx - 3);
		
		wchar_t szText[255] = {};
		COMBOBOXEXITEM  item{};
		item.mask = CBEIF_IMAGE | CBEIF_INDENT | CBEIF_OVERLAY | CBEIF_SELECTEDIMAGE | CBEIF_TEXT;
		item.pszText = szText;
		item.cchTextMax = 255;

		item.iItem = iIdx;
		m_cbCullingMask.GetItem(&item);

		if (0 == item.iImage)
			item.iImage = 1;
		else
			item.iImage = 0;

		m_cbCullingMask.SetItem(&item);
		m_cbCullingMask.SetCurSel(iIdx);
	}

	FocusGameView();
}

void CCameraDlg::OnCbnDropdownComboboxex1()
{
	init();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
