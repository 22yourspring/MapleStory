// CTransformDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "TransformDlg.h"
#include "afxdialogex.h"

#include "AnrealEngine/func.h"

#include "resource.h"
#include "AnrealEngine/GameObject.h"
#include "AnrealEngine/Transform.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Layer.h"

// CTransformDlg 대화 상자

IMPLEMENT_DYNAMIC(CTransformDlg, CDialogEx)

CTransformDlg::CTransformDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_TRANSFORMDLG, pParent)
	, LayerCheck(false)
{

}

CTransformDlg::~CTransformDlg()
{
}


void CTransformDlg::update(CGameObject * _pTargetObj)
{
	LayerNameCheck();

	if (m_bEditFocus)
		return;


	m_pTargetObj = _pTargetObj;
	assert(_pTargetObj->Transform());
	
	Vec3 vPos = _pTargetObj->Transform()->GetLocalPos();
	Vec3 vScale = _pTargetObj->Transform()->GetLocalScale();
	Vec3 vRot = _pTargetObj->Transform()->GetLocalRot();
	
	m_bCtrlClicked = false;

	CString strTrans;
	for (int i = 0; i < 3; ++i)
	{
		strTrans.Format(L"%f", vPos[i]);
		m_arrTrans[i].SetWindowTextW(strTrans);
	}

	for (int i = 0; i < 3; ++i)
	{
		strTrans.Format(L"%f", vScale[i]);
		m_arrTrans[i + 3].SetWindowTextW(strTrans);
	}

	for (int i = 0; i < 3; ++i)
	{
		//vRot[i] = GetDegree(vRot[i]);
		(float)vRot[i] *= _RADIAN;		// 소수점 오차율 표기 때문에 함수 사용 X
		strTrans.Format(L"%f", vRot[i]);
		m_arrTrans[i + 6].SetWindowTextW(strTrans);
	}

	if (!LayerCheck)
	{
		m_LayerName.SetWindowTextW(CSceneMgr::GetInst()->GetCurScene()->GetLayer(_pTargetObj->GetLayerIdx())->GetName().c_str());
	}

	m_bCtrlClicked = true;
}

void CTransformDlg::LayerNameCheck()
{
	static float AccNameTime = 0.f;

	AccNameTime += 0.0001f;

	if (AccNameTime < 0.8)
	{
		return;
	}

	AccNameTime = 0.f;
	LayerCheck = false;
}

void CTransformDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 9; ++i)
	{
		DDX_Control(pDX, 2000 + i, m_arrTrans[i]);
	}
	DDX_Control(pDX, IDC_COMBO2, m_LayerList);
	DDX_Control(pDX, IDC_EDIT11, m_LayerName);
}


BEGIN_MESSAGE_MAP(CTransformDlg, CDialogEx)
	ON_CONTROL_RANGE(EN_CHANGE, IDC_EDIT1, IDC_EDIT9, &CTransformDlg::OnCtrlChange)
	ON_CONTROL_RANGE(EN_SETFOCUS, IDC_EDIT1, IDC_EDIT9, &CComponentDlg::OnEditFocused)	
	ON_CONTROL_RANGE(EN_KILLFOCUS, IDC_EDIT1, IDC_EDIT9, &CComponentDlg::OnEditKillFocused)
	ON_CBN_SELCHANGE(IDC_COMBO2, &CTransformDlg::ShowLayer)
	ON_EN_KILLFOCUS(IDC_EDIT11, &CTransformDlg::OnEnKillfocusEdit11)
	ON_BN_CLICKED(IDC_BUTTON1, &CTransformDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CTransformDlg::OnBnClickedButton2)
	ON_CBN_SETFOCUS(IDC_COMBO2, &CTransformDlg::OnCbnSetfocusCombo2)
	ON_EN_SETFOCUS(IDC_EDIT11, &CTransformDlg::OnEnSetfocusEdit11)
END_MESSAGE_MAP()

void CTransformDlg::OnCtrlChange(UINT _id)
{
	if (!m_bCtrlClicked)
		return;

	CString str;
	Vec3 vData[3];

	for (int i = 0; i < 9; ++i)
	{
		m_arrTrans[i].GetWindowTextW(str);
		vData[i/3][i%3] = (float)_wtof(str.GetBuffer());
	}	
	
	
	m_pTargetObj->Transform()->SetLocalPos(vData[0]);
	m_pTargetObj->Transform()->SetLocalScale(vData[1]);
	
	//vData[2].x *= PI / 180.f;
	//vData[2].y *= PI / 180.f;
	//vData[2].z *= PI / 180.f;

	vData[2].x *= _DEGREE;
	vData[2].y *= _DEGREE;
	vData[2].z *= _DEGREE;

	//(vData[2].y) *= (3.14 / 180);
	//(vData[2].z) *= (3.14 / 180);

	m_pTargetObj->Transform()->SetLocalRot(vData[2]);
}

BOOL CTransformDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	



	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CTransformDlg::ShowLayer()
{
	if (m_pTargetObj == nullptr)
	{
		SetWindowTextW(L"");

		return;
	}

	//m_pTargetObj->GetLayerIdx();


	
	if (m_pTargetObj->GetParent() == nullptr)		//최상위 부모일 때만
	{
		m_LayerList.SetWindowTextW(CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx())->GetName().c_str());
	}
	

	
	if (m_pTargetObj->GetParent() == nullptr)
	{
		vector<CGameObject*> &LAYEROBJ = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx())->GetObjectVec();
		vector<CGameObject*> ::iterator iter = LAYEROBJ.begin();
		CGameObject* tempLayerObj = nullptr;


		//값이 변경되었을 때

		for (iter; iter != LAYEROBJ.end(); ++iter)
		{
			if (m_pTargetObj == *iter)
			{
				tempLayerObj = *iter;
				LAYEROBJ.erase(iter);
				break;
			}
		}


		CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_LayerList.GetCurSel())->AddGameObject(tempLayerObj);
		m_LayerName.SetWindowTextW((CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx()))->GetName().c_str());

	}
	//else
	//{

	//	CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_LayerList.GetCurSel())->AddGameObject(m_pTargetObj);

	//	wstring names = (CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx()))->GetName().c_str();


	//	m_LayerName.SetWindowTextW((CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx()))->GetName().c_str());
	//}
}



void CTransformDlg::OnEnKillfocusEdit11()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTransformDlg::OnBnClickedButton1()
{
	CString LayerName;
	m_LayerName.GetWindowTextW(LayerName);

	if (LayerName == L"")
	{
		return;
	}

	if (CSceneMgr::GetInst()->GetCurScene()->GetLayer((wstring)LayerName) != nullptr)
	{
		return;
	}

	CSceneMgr::GetInst()->GetCurScene()->AddLayer((wstring)LayerName);
	
	m_LayerName.SetWindowTextW(LayerName);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTransformDlg::OnBnClickedButton2()
{
	//wstring str = CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx())->GetName().c_str();
	
	if (nullptr == m_pTargetObj)
	{
		return;
	}

	if (m_pTargetObj->GetParent() == nullptr)
	{
		m_LayerName.SetWindowTextW(CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx())->GetName().c_str());
	}

	//m_LayerName.SetWindowTextW(CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx())->GetName().c_str());

	else
	{
		if (m_pTargetObj->GetLayerIdx() == -1)
		{
			m_LayerName.SetWindowTextW(L"");
			return;
		}
		m_LayerName.SetWindowTextW(CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx())->GetName().c_str());
	}
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}




void CTransformDlg::OnCbnSetfocusCombo2()
{
	wstring LayerName;

	UINT LayerCount = CSceneMgr::GetInst()->GetCurScene()->GetLayerCount();


	for (UINT i = 0; i < LayerCount; ++i)
	{
		m_LayerList.DeleteString(0);
	}

	for (UINT i = 0; i < LayerCount; ++i)
	{
		LayerName = CSceneMgr::GetInst()->GetCurScene()->GetLayer(i)->GetName();
		m_LayerList.InsertString(i, LayerName.c_str());
	}

	if (m_pTargetObj == nullptr)
	{
		return;
	}


	if (m_pTargetObj->GetLayerIdx() != NULL && m_pTargetObj->GetLayerIdx() != -1)
	{
		m_LayerName.SetWindowTextW((CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx()))->GetName().c_str());
	}

	//m_LayerName.SetWindowTextW(CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx())->GetName().c_str());

	

	//m_LayerName.SetWindowTextW(CSceneMgr::GetInst()->GetCurScene()->GetLayer(m_pTargetObj->GetLayerIdx())->GetName().c_str());
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CTransformDlg::OnEnSetfocusEdit11()
{
	LayerCheck = true;
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
