// Animator2DDlg.cpp: 구현 파일
//

#include "stdafx.h"
#include "Animator2DDlg.h"
#include "afxdialogex.h"


#include "resource.h"
#include "AnrealEngine/PathMgr.h"

#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Animation2D.h"

#include "AnrealEngine/ResMgr.h"
#include "AnrealEngine/Transform.h"






// CAnimator2DDlg 대화 상자

IMPLEMENT_DYNAMIC(CAnimator2DDlg, CDialogEx)

CAnimator2DDlg::CAnimator2DDlg(CWnd* pParent /*=nullptr*/)
	: CComponentDlg(IDD_ANIMATOR2DDLG, pParent)
	
{

}

CAnimator2DDlg::~CAnimator2DDlg()
{
}

void CAnimator2DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO1, FRAME_Anim);
	DDX_Control(pDX, IDC_EDIT1, AccTime);
	DDX_Control(pDX, IDC_EDIT2, KeyValue);
	DDX_Control(pDX, IDC_LIST1, listAnim);
	DDX_Control(pDX, IDC_EDIT3, FrameCount);
	DDX_Control(pDX, IDC_EDIT4, QuantityCount);
	DDX_Control(pDX, IDC_EDIT5, AnimLocation);
	DDX_Control(pDX, IDC_EDIT6, AnimValue);
	DDX_Control(pDX, IDC_CHECK2, m_FullAtlas);
	DDX_Control(pDX, IDC_CHECK1, RepeatChk);
	DDX_Control(pDX, IDC_LIST2, AnimVec);
	DDX_Control(pDX, IDC_EDIT7, OffsetX);
	DDX_Control(pDX, IDC_EDIT8, OffsetY);
	DDX_Control(pDX, IDC_EDIT9, AnimKey);
}

void CAnimator2DDlg::init()
{

	
}

void CAnimator2DDlg::update(CGameObject * _pTargetObj)
{
	m_pTargetObj = _pTargetObj;


	if (00 == FRAME_Anim.GetCurSel())
	{
		FrameCount.EnableWindow(true);
		QuantityCount.EnableWindow(true);
		AnimLocation.EnableWindow(true);
		AnimValue.EnableWindow(true);
		m_FullAtlas.EnableWindow(true);

	}
	else
	{
		FrameCount.EnableWindow(false);
		QuantityCount.EnableWindow(false);
		AnimLocation.EnableWindow(false);
		AnimValue.EnableWindow(false);
		m_FullAtlas.EnableWindow(false);

	}
}

void CAnimator2DDlg::Renewal()
{
	
	UINT Count = listAnim.GetCount();

	UINT VecCount = AnimVec.GetCount();

	// 1. 배열
	// 2. 이터레이터 begin, end

	// 내부에서 지운다, 바꾼다

	//auto& begin;
	//const auto& end;
	//listAnim.DeleteItem
	for (size_t i = 0; i < Count; ++i)
	{
		listAnim.DeleteString(0);
		//listAnim.DeleteString(i);
	}
	Count = listAnim.GetCount();
	//vector<CScript*>& vecScript = m_pTargetObj->GetScript();
	//for (UINT i = 0; i < vecScript.size(); ++i)

	map<wstring, CAnimation2D*>& mapAnim = m_pTargetObj->Animator2D()->GetAnimMap();

	//for (auto iter : mapAnim)
	//{
	//	listAnim.InsertString(0, iter.first.c_str());
	//}

	map<wstring, CAnimation2D*>::iterator iterEnd = mapAnim.end();
	int i = 0;


	for (size_t i = 0; i < VecCount; ++i)
	{
		AnimVec.DeleteString(0);
		//listAnim.DeleteString(i);
	}
	VecCount = AnimVec.GetCount();



	for (map<wstring, CAnimation2D*>::iterator iter = mapAnim.begin(); iter != iterEnd; ++iter)
	{

		//listAnim.InsertString(i, iter->first.c_str());
		listAnim.InsertString(-1, iter->first.c_str());
		++i;
	}
}

void CAnimator2DDlg::resizeObj(CTexture * _pTargetImage, POINT _SIZE)
{
	Vec3 OriginSize = { _pTargetImage->GetWidth() / _SIZE.x , _pTargetImage->GetHeight() / _SIZE.y , 0.f };


	m_pTargetObj->Transform()->SetLocalScale(OriginSize);
}

POINT CAnimator2DDlg::startLocation(CTexture* _pTex, int _quantityCount, int _animLoc)
{	
	
	return POINT{(long)0 ,(long)_pTex->GetHeight() / _quantityCount * (_animLoc - 1) };
}



BEGIN_MESSAGE_MAP(CAnimator2DDlg, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &CAnimator2DDlg::OpenDirectory)
	ON_BN_CLICKED(IDC_BUTTON2, &CAnimator2DDlg::AddAnim)
	ON_BN_CLICKED(IDC_BUTTON3, &CAnimator2DDlg::DelAnim)
	ON_BN_CLICKED(IDC_BUTTON4, &CAnimator2DDlg::OnAnimPlay)
	ON_LBN_SELCHANGE(IDC_LIST1, &CAnimator2DDlg::GetCurItem)
	ON_BN_CLICKED(IDC_BUTTON5, &CAnimator2DDlg::OnModificAcc)
	ON_LBN_SELCHANGE(IDC_LIST2, &CAnimator2DDlg::AnimVecCursel)
	ON_BN_CLICKED(IDC_BUTTON6, &CAnimator2DDlg::ReSetClicked)
END_MESSAGE_MAP()


// CAnimator2DDlg 메시지 처리기


BOOL CAnimator2DDlg::OnInitDialog()
{
	CComponentDlg::OnInitDialog();

	FRAME_Anim.InsertString(0, L"FRAME_ATLAS");
	FRAME_Anim.InsertString(1, L"FRAME_PATH");

	FrameCount.EnableWindow(false);
	QuantityCount.EnableWindow(false);

	// TODO:  여기에 추가 초기화 작업을 추가합니다.


	return TRUE;  // return TRUE unless you set the focus to a control
				  // 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CAnimator2DDlg::OpenDirectory()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	if (0 == FRAME_Anim.GetCurSel())
	{
		Renewal();



		strAnimPath = CPathMgr::GetResPath();
		strAnimPath += L"Texture\\Resources\\";

		// FileDialog
		wchar_t szFilter[50] = L"";

		CFileDialog dlg(TRUE, L"Png", L"*.png",
			OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

		dlg.m_ofn.lpstrInitialDir = strAnimPath.c_str();

		if (dlg.DoModal() != IDOK)
			return;

		strAnimPath = dlg.GetPathName();

		strAnimPath = CPathMgr::GetAtlasPath(strAnimPath);
		strAnimKey = strAnimPath;
	//	strAnimKey = CPathMgr::GetAnimAtlasKey(strAnimPath);


		KeyValue.SetWindowTextW(strAnimKey.c_str());
		
		AnimKey.SetWindowTextW(CPathMgr::GetAnimAtlasKey(strAnimPath).c_str());
	}



	if (1 == FRAME_Anim.GetCurSel())
	{
		Renewal();

		strAnimPath = CPathMgr::GetResPath();
		strAnimPath += L"Texture\\Resources\\";


		// FileDialog
		wchar_t szFilter[50] = L"";
		CFolderPickerDialog Picker(strAnimPath.c_str(), OFN_FILEMUSTEXIST, NULL, 0);

		if (Picker.DoModal() != IDOK)
				return;
		strAnimPath = Picker.GetPathName();
		
		strAnimPath = CPathMgr::GetFramePath(strAnimPath);
		strAnimKey = strAnimPath;
	//	strAnimKey = CPathMgr::GetAnimFrameKey(strAnimPath);


		

		/*
		CFileDialog dlg(TRUE, L"Png", L"*.png",
			OFN_PATHMUSTEXIST | OFN_HIDEREADONLY, szFilter, NULL);

		dlg.m_ofn.lpstrInitialDir = strAnimPath.c_str();

		if (dlg.DoModal() != IDOK)
			return;

		strAnimPath = dlg.GetPathName();*/
	
		/*m_pTargetObj->Animator2D()->AddAnimation(strAnimPath, strAnimKey, 0.02f);
		m_pTargetObj->Animator2D()->PlayAnim(strAnimKey,true);*/


		KeyValue.SetWindowTextW(strAnimKey.c_str());
		AnimKey.SetWindowTextW(CPathMgr::GetAnimAtlasKey(strAnimPath).c_str());
	}
}


void CAnimator2DDlg::AddAnim()
{

	if (strAnimPath == L"")
	{
		MessageBox(_T("No ResourcesPath selected."), _T("Error"));
		
		return;
	}

	if (0 == FRAME_Anim.GetCurSel())
	{
		CString strAccTime;
		CString strKey;
		
		

		AccTime.GetWindowTextW(strAccTime);

		float m_fAccTime = 0.f;

		m_fAccTime = (float)_wtof(strAccTime.GetBuffer());


		KeyValue.GetWindowTextW(strKey);
		strAnimKey = strKey;



		CString strFrame;
		FrameCount.GetWindowTextW(strFrame);
		int m_frameCount = 0;
		m_frameCount = (int)_wtoi(strFrame.GetBuffer());
	
		CString strQuantity;
		int m_quantityCount = 0;
		QuantityCount.GetWindowTextW(strQuantity);
		m_quantityCount = (int)_wtoi(strQuantity.GetBuffer());


		CString strAnimLOC;
		AnimLocation.GetWindowTextW(strAnimLOC);
		int animLoc = 0;
		animLoc = (int)_wtoi(strAnimLOC.GetBuffer());


		CString strAnimValue;
		AnimValue.GetWindowTextW(strAnimValue);
		int animValue;
		animValue = (int)_wtoi(strAnimValue.GetBuffer());

		BOOL bFullAtlasChk;
		bFullAtlasChk = m_FullAtlas.GetCheck();
		
		BOOL bRepeatChk;
		bRepeatChk = RepeatChk.GetCheck();



		if (strKey == L"")
		{
			
			MessageBox(_T("No resources selected."), _T("Error"));

			return;
		}

		else if (strFrame == L"")
		{
			MessageBox(_T("No Frame specified."), _T("Error"));

			return;
		}

		else if (strQuantity == L"")
		{
			MessageBox(_T("No Quantity specified."), _T("Error"));

			return;
		}

		else if (strAnimLOC == L"")
		{
			MessageBox(_T("No Animation Location specified."), _T("Error"));

			return;
		}

		else if (strAnimValue == L"")
		{
			MessageBox(_T("No Animation Value specified."), _T("Error"));

			return;
		}


		CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(strAnimKey, strAnimPath);

		if (pTex == NULL)
		{
			return;
		}

		Vec3 m_applyFC;
		m_applyFC.x = (pTex->GetWidth()) / m_frameCount;
		m_applyFC.y = (pTex->GetHeight()) / m_quantityCount;
		m_applyFC.z = 0.f;

		
		//resizeObj(pTex, POINT{m_frameCount, m_quantityCount});
		
	//	m_pTargetObj->Transform()->SetLocalScale(m_applyFC);


		m_pTargetObj->Animator2D()->AddAnimation(pTex, startLocation(pTex, m_quantityCount, animLoc), (UINT)m_applyFC.x, animValue, POINT{ (int)m_applyFC.x, (int)m_applyFC.y }, strAnimKey, m_fAccTime, { m_frameCount, m_quantityCount }, { animLoc,animValue }, bFullAtlasChk, bRepeatChk);
														//시작점		// 갭   //개수   // 크롭사이즈  
	
														//	갭 = vSacle.widght / 프레임 개수		크롭사이즈 가로 / 프레임개수
														//						크롭사이즈 세로 / 아틀라스 안의 애니메이션 개수


/*
		map<wstring, CAnimation2D*>& mapAnim = m_pTargetObj->Animator2D()->GetAnimMap();
		map<wstring, CAnimation2D*> ::iterator iter = mapAnim.find((wstring)strKey);

		*/

		

	}


	if ((1 == FRAME_Anim.GetCurSel()))
	{
		CString str;
		CString strKey;

		AccTime.GetWindowTextW(str);

		float m_fAccTime = 0.f;

		m_fAccTime = (float)_wtof(str.GetBuffer());


		KeyValue.GetWindowTextW(strKey);

		strAnimKey = strKey;

		
		BOOL bRepeatChk;
		bRepeatChk = RepeatChk.GetCheck();


		


		m_pTargetObj->Animator2D()->AddAnimation(strAnimPath, strAnimKey, m_fAccTime, bRepeatChk);
		/*

		map<wstring, CAnimation2D*>& mapAnim = m_pTargetObj->Animator2D()->GetAnimMap();
		map<wstring, CAnimation2D*> ::iterator iter = mapAnim.find((wstring)strAnimKey);

		*/

		//resizeObj();

	}
	Renewal();

}


void CAnimator2DDlg::DelAnim()
{

	CString str;

	KeyValue.GetWindowTextW(str);

	wstring DelStrKey = str;

	//m_pTargetObj->Animator2D()->DelAnimation(DelStrKey);


	CString NumberList;



	CListBox *p_list2 = (CListBox *)GetDlgItem(IDC_LIST2);

	wstring AppDel;
	
	for (int i = 0; i < p_list2->GetCount(); ++i)
	{
		p_list2->GetCount();
		AppDel = DelStrKey;

		p_list2->GetText(i, NumberList);
		AppDel += NumberList;
		AppDel += L".png";

		m_pTargetObj->Animator2D()->DelAnimation(DelStrKey, AppDel, p_list2->GetCount(), i);
	}


	//int index = listAnim.GetCurSel();

	//if (index != LB_ERR)
	//{
	//	//LB_ERR =-1 이 디파인 된것. 즉 어떤 문자열이 선택 되었을때만 실행
	//	listAnim.DeleteString(index);
	//}

	Renewal();
}


void CAnimator2DDlg::OnAnimPlay()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.

	CString strKey;

	KeyValue.GetWindowTextW(strKey);

	wstring playStrKey = strKey;

	if (strKey == L"")
	{
		return ;
	}




	m_pTargetObj->Animator2D()->PlayAnim(playStrKey/*, true*/);

	CString strAccTime;
	
	AccTime.GetWindowTextW(strAccTime);

	float m_fAccTime = 0.f;

	m_fAccTime = (float)_wtof(strAccTime.GetBuffer());


	KeyValue.GetWindowTextW(strKey);
	strAnimKey = strKey;

	CString strFrame;
	FrameCount.GetWindowTextW(strFrame);
	int m_frameCount = 0;
	m_frameCount = (int)_wtoi(strFrame.GetBuffer());

	CString strQuantity;
	int m_quantityCount = 0;
	QuantityCount.GetWindowTextW(strQuantity);
	m_quantityCount = (int)_wtoi(strQuantity.GetBuffer());


	CString strAnimLOC;
	AnimLocation.GetWindowTextW(strAnimLOC);
	int animLoc = 0;
	animLoc = (int)_wtoi(strAnimLOC.GetBuffer());


	CString strAnimValue;
	AnimValue.GetWindowTextW(strAnimValue);
	int animValue;
	animValue = (int)_wtoi(strAnimValue.GetBuffer());

	BOOL bFullAtlasChk;
	bFullAtlasChk = m_FullAtlas.GetCheck();

	CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(strAnimKey);

	if (1 == FRAME_Anim.GetCurSel())
	{
		m_frameCount = 1;
		m_quantityCount = 1;
	}

	if (pTex == NULL)
	{
		return;
	}

	Vec3 m_applyFC;
	m_applyFC.x = (pTex->GetWidth()) / m_frameCount;
	m_applyFC.y = (pTex->GetHeight()) / m_quantityCount;
	m_applyFC.z = 0.f;


	resizeObj(pTex, POINT{ m_frameCount, m_quantityCount });

}


void CAnimator2DDlg::GetCurItem()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.


	CString str;
	CString POINTX, POINTY;
	CString strAccTime;

	CString strAnimLoc;
	CString strAnimVal;



	CListBox *p_list = (CListBox *)GetDlgItem(IDC_LIST1);
	
	int index = p_list->GetCurSel();

	if (index != LB_ERR)
	{
		p_list->GetText(index, str);
	}
	else
		return;

	KeyValue.SetWindowTextW(str);
	wstring temp = str.operator LPCWSTR();
	AnimKey.SetWindowTextW(CPathMgr::GetAnimAtlasKey(temp).c_str());
	
	POINTX.Format(_T("%d"), m_pTargetObj->Animator2D()->FindPOINT((wstring)str).x);
	POINTY.Format(_T("%d"), m_pTargetObj->Animator2D()->FindPOINT((wstring)str).y);




	map<wstring, CAnimation2D*>& mapAnim = m_pTargetObj->Animator2D()->GetAnimMap();
	map<wstring, CAnimation2D*> ::iterator iter = mapAnim.find((wstring)str);


	if (iter == mapAnim.end())
	{
		return;
	}
	RepeatChk.SetCheck((iter->second->GetAnimRepeat()));

	float fAccTime = iter->second->GetDuration();
	strAccTime.Format(L"%f", fAccTime);
	
	int animloc = iter->second->GetAnimLocVal().x;
	strAnimLoc.Format(L"%d", animloc);

	int animvalue = iter->second->GetAnimLocVal().y;
	strAnimVal.Format(L"%d", animvalue);

	

	FrameCount.SetWindowTextW(POINTX);
	QuantityCount.SetWindowTextW(POINTY);
	AccTime.SetWindowTextW(strAccTime);

	
	
	AnimValue.SetWindowTextW(strAnimVal);
	AnimLocation.SetWindowTextW(strAnimLoc);

	bool checkFullAtlas = iter->second->GetFullAts();
	m_FullAtlas.SetCheck(checkFullAtlas);




	iter = mapAnim.find((wstring)str);

	
	vector<tAnimFrm> &takeVec = iter->second->GetFrmVec();
	

	wstring FrmKey;


	UINT VecCount = AnimVec.GetCount();

	for (size_t i = 0; i < VecCount; ++i)
	{
		AnimVec.DeleteString(0);
	}

	VecCount = AnimVec.GetCount();

	for (size_t i = 0; i < iter->second->GetFrmVec().size(); ++i)
	{
		FrmKey = takeVec[i].pTex->GetName();

		wstring tempKey = CPathMgr::GetAnimAtlasKey(FrmKey);

		AnimVec.InsertString((int)i, CPathMgr::GetAnimAtlasKey(FrmKey).c_str());
	}
}

void CAnimator2DDlg::OnModificAcc()
{
	CString str;

	CString strAccTime;

	CListBox *p_list = (CListBox *)GetDlgItem(IDC_LIST1);

	int index = p_list->GetCurSel();

	if (index != LB_ERR)
	{
		p_list->GetText(index, str);
	}

	KeyValue.SetWindowTextW(str);
	wstring temp = str.operator LPCWSTR();
	AnimKey.SetWindowTextW(CPathMgr::GetAnimAtlasKey(temp).c_str());

	
	map<wstring, CAnimation2D*>& mapAnim = m_pTargetObj->Animator2D()->GetAnimMap();

	map<wstring, CAnimation2D*> ::iterator iter = mapAnim.find((wstring)str);
	if (iter == mapAnim.end())
	{
		MessageBox(_T("The current animation key does not exist."), _T("Error"));

		return;
	}
	AccTime.GetWindowTextW(strAccTime);

	float fValue = (float)_tstof(strAccTime);

	iter->second->SetDuration(fValue);



	CString CSoffsetx, CSoffsety;

	p_list = (CListBox *)GetDlgItem(IDC_LIST1);

	index = p_list->GetCurSel();

	if (index != LB_ERR)
	{
		p_list->GetText(index, str);
	}
	else
		return;




	CListBox *p_list2 = (CListBox *)GetDlgItem(IDC_LIST2);



	int vecindex = p_list2->GetCurSel();
	
	if (vecindex != LB_ERR)
	{
		p_list2->GetText(vecindex, str);
	}
	else
		return;



	OffsetX.GetWindowTextW(CSoffsetx);
	OffsetY.GetWindowTextW(CSoffsety);

	float fOffsetX = (float)_wtof(CSoffsetx);
	float fOffsetY = (float)_wtof(CSoffsety);




	KeyValue.GetWindowTextW(str);
	wstring playStrKey = str;


	iter = mapAnim.find(playStrKey);

	if (iter == mapAnim.end())
	{
		assert(NULL);
	}
	if (str!= L"" && CSoffsetx != L"" && CSoffsety!= L"")
	{
		iter->second->SetOffset(vecindex, fOffsetX, fOffsetY);
	}



	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimator2DDlg::AnimVecCursel()
{

	CString str;

	CString CSoffsetx, CSoffsety;

	CListBox *p_list = (CListBox *)GetDlgItem(IDC_LIST1);

	int index = p_list->GetCurSel();

	if (index != LB_ERR)
	{
		p_list->GetText(index, str);
	}
	else
		return;

	CString str2;

	CListBox *p_list2 = (CListBox *)GetDlgItem(IDC_LIST2);

	int vecindex = p_list2->GetCurSel();

	if (vecindex != LB_ERR)
	{
		p_list2->GetText(vecindex, str2);
	}
	else
		return;
	
	
	OffsetX.GetWindowTextW(CSoffsetx);
	OffsetY.GetWindowTextW(CSoffsety);
	
	float fOffsetX = (float)_wtof(CSoffsetx);
	float fOffsetY = (float)_wtof(CSoffsety);

	map<wstring, CAnimation2D*>& mapAnim = m_pTargetObj->Animator2D()->GetAnimMap();
	map<wstring, CAnimation2D*> ::iterator iter = mapAnim.find((wstring)str);

	/*if (iter == mapAnim.end())
	{
		return;
	}*/

	CString animString;
	animString.Format(L"%f", iter->second->GetOffset(vecindex).x);
	OffsetX.SetWindowTextW(animString);
	
	animString.Format(L"%f", iter->second->GetOffset(vecindex).y);
	OffsetY.SetWindowTextW(animString);

	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CAnimator2DDlg::ReSetClicked()
{
	Renewal();
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}
