// ColliderDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "ColliderDlg.h"
#include "afxdialogex.h"

#include "resource.h"
#include "AnrealEngine/Collider.h"
#include "AnrealEngine/GameObject.h"


// CColliderDlg 대화 상자

IMPLEMENT_DYNAMIC(CColliderDlg, CDialogEx)

CColliderDlg::CColliderDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_COLLIDERDLG, pParent)
{

}

CColliderDlg::~CColliderDlg()
{
}


void CColliderDlg::update(CGameObject * _pTargetObj)
{
	if (m_bEditFocus)
		return;

	m_pTargetObj = _pTargetObj;
	assert(_pTargetObj->Transform());

	Vec3 vPos = _pTargetObj->Collider()->GetOffsetPos();
	Vec3 vScale = _pTargetObj->Collider()->GetOffsetScale();



	m_bCtrlClicked = false;

	CString strCollider;
	for (int i = 0; i < 3; ++i)
	{
		strCollider.Format(L"%f", vPos[i]);
		m_arrColl[i].SetWindowTextW(strCollider);
	}

	for (int i = 0; i < 3; ++i)
	{
		strCollider.Format(L"%f", vScale[i]);
		m_arrColl[i + 3].SetWindowTextW(strCollider);
	}


	m_bCtrlClicked = true;
}



void CColliderDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 6; ++i)
	{
		DDX_Control(pDX, 2000 + i, m_arrColl[i]);
	}
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO2, m_ListColliderTYPE);
}


BEGIN_MESSAGE_MAP(CColliderDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT6, &CColliderDlg::OnCtrlChange)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT6, &CComponentDlg::OnEditFocused)
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT6, &CComponentDlg::OnEditKillFocused)
	ON_BN_CLICKED(IDC_BUTTON3, &CColliderDlg::OnBnClickedButton3)
	ON_CBN_CLOSEUP(IDC_COMBO2, &CColliderDlg::OnCbnCloseupCombo2)
END_MESSAGE_MAP()


// CColliderDlg 메시지 처리기


void CColliderDlg::OnCtrlChange(UINT _id)
{
	if (!m_bCtrlClicked)
		return;

	CString str;
	Vec3 vData[2];

	//vData[0]
	// x y z
	
	//vData[1]
	// x y z

	for (int i = 0; i < 6; ++i)
	{
		m_arrColl[i].GetWindowTextW(str);
		vData[i / 3][i % 3] = (float)_wtof(str.GetBuffer());

	}

	m_pTargetObj->Collider()->SetOffsetPos(vData[0]);
	m_pTargetObj->Collider()->SetOffsetScale(vData[1]);

}



void CColliderDlg::OnBnClickedButton3()
{
	m_pTargetObj->Collider()->SetColliderType(COLLIDER_TYPE::COLLIDER_2D_BOX);
	m_ListColliderTYPE.SetWindowTextW(L"COLLIDER_2D_BOX");
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


BOOL CColliderDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	m_ListColliderTYPE.InsertString(0, L"COLLIDER_2D_BOX");
	m_ListColliderTYPE.InsertString(1, L"COLLIDER_2D_CIRCLE");
	m_ListColliderTYPE.InsertString(1, L"COLLIDER_3D_CUBE");
	m_ListColliderTYPE.InsertString(1, L"COLLIDER_3D_SHPERE");
	
	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CColliderDlg::OnCbnCloseupCombo2()
{
	if (m_pTargetObj == nullptr)
	{
		return;
	}

	COLLIDER_TYPE ColliderEnum = m_pTargetObj->Collider()->GetColliderType();

	if (ColliderEnum == COLLIDER_TYPE::COLLIDER_2D_BOX)
	{
		m_ListColliderTYPE.SetWindowTextW(L"COLLIDER_2D_BOX");
	}
	else if (ColliderEnum == COLLIDER_TYPE::COLLIDER_2D_CIRCLE)
	{
		m_ListColliderTYPE.SetWindowTextW(L"COLLIDER_2D_CIRCLE");
	}
	else if (ColliderEnum == COLLIDER_TYPE::COLLIDER_3D_CUBE)
	{
		m_ListColliderTYPE.SetWindowTextW(L"COLLIDER_3D_CUBE");

	}
	else if (ColliderEnum == COLLIDER_TYPE::COLLIDER_3D_SHPERE)
	{
		m_ListColliderTYPE.SetWindowTextW(L"COLLIDER_3D_SHPERE");
	}
	else
	{
		m_ListColliderTYPE.SetWindowTextW(L"");
	}
	


	//값이 변경되었을 때
	if (0 == m_ListColliderTYPE.GetCurSel())
	{
		m_pTargetObj->Collider()->SetColliderType(COLLIDER_TYPE::COLLIDER_2D_BOX);
	}
	else if (1 == m_ListColliderTYPE.GetCurSel())
	{
		m_pTargetObj->Collider()->SetColliderType(COLLIDER_TYPE::COLLIDER_2D_CIRCLE);
	}
	else if (2 == m_ListColliderTYPE.GetCurSel())
	{
		m_pTargetObj->Collider()->SetColliderType(COLLIDER_TYPE::COLLIDER_3D_CUBE);
	}
	else if (3 == m_ListColliderTYPE.GetCurSel())
	{
		m_pTargetObj->Collider()->SetColliderType(COLLIDER_TYPE::COLLIDER_3D_SHPERE);
	}
	else
	{
		return;
	}

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
