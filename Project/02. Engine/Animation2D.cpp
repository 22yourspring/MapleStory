#include "Animation2D.h"

#include "ResMgr.h"
#include "Texture.h"
#include "TimeMgr.h"

#include "Device.h"
#include "ConstBuffer.h"

tAnimFrm::tAnimFrm() {

}
tAnimFrm::~tAnimFrm() {

}

CAnimation2D::CAnimation2D()
	: m_pOwner(nullptr)
	, m_iCurFrm(0)
	, m_fAccTime(0.f)
	, m_bFinish(false)
	, m_Matrix{ 1, 1 }
{
}

CAnimation2D::~CAnimation2D()
{
}

int CAnimation2D::update()
{
	m_fAccTime += DT;

	if (m_vecFrm[m_iCurFrm].fDuration <= m_fAccTime)
	{
		while (true)
		{
			m_fAccTime -= m_vecFrm[m_iCurFrm].fDuration;

			++m_iCurFrm;

			if ((int)m_vecFrm.size() <= m_iCurFrm)
			{
				m_iCurFrm = (int)(m_vecFrm.size() - 1);
				m_bFinish = true;
				break;
			}
			else if (m_vecFrm[m_iCurFrm].fDuration > m_fAccTime)
				break;
		}
	}

	return 0;
}

void CAnimation2D::UpdateData()
{
	if (m_iCurFrm == 1)
		int i = 0;
	static CConstBuffer* pBuffer = CDevice::GetInst()->FindCBuffer(L"Anim2DInfo");

	g_anim2d.g_bAnim2dUse = true;
	g_anim2d.vLT = m_vecFrm[m_iCurFrm].vLT/* + m_vecFrm[m_iCurFrm].vOffset*/;
	g_anim2d.vSize = m_vecFrm[m_iCurFrm].vSize /*+ m_vecFrm[m_iCurFrm].vOffset*/;

	pBuffer->UpdateData(&g_anim2d, sizeof(tAnim2DInfo));
	pBuffer->UpdateRegisterAll();

	m_vecFrm[m_iCurFrm].pTex->SetRegisterAll(16);
}

Ptr<CTexture> CAnimation2D::GetpTex(int _idx)
{
	if (m_vecFrm.size() == 0)
	{
		return 0;
	}

	if ((UINT)_idx > m_vecFrm.size())
	{
		MessageBox(NULL, L"Currently overhangs of vector", L"Error Page.", MB_OK);
		_idx = 0;
	}

	return m_vecFrm[_idx].pTex;
}

void CAnimation2D::SetFrmIdx(int _iIndex)
{
	if ((int)m_vecFrm.size() <= _iIndex)
		assert(NULL);

	m_bFinish = false;
	m_iCurFrm = _iIndex;
	m_fAccTime = 0.f;
}

void CAnimation2D::Create(CTexture * _pTex, const POINT & _ptLT, UINT _iWGap, UINT iFrmCount, const POINT & _ptCropSize, const wstring & _strKey, float _Acctime, POINT _Matrix, POINT _AnimLocVale, bool _FullAts, bool _Repeat)
{
	tAnimFrm frm = {};
	m_Matrix = _Matrix;
	m_bFullAts = _FullAts;
	m_AnimLocVal = _AnimLocVale;
	m_bAnimRepeat = _Repeat;

	m_bSynthesisAtlas = true;

	if (_FullAts == false)
	{
		for (UINT i = 0; i < iFrmCount; ++i)
		{
			frm.pTex = _pTex;

			frm.vLT = Vec2(((float)_ptLT.x) / (_pTex->GetWidth()) + (i * (float)_iWGap / _pTex->GetWidth()), ((float)_ptLT.y) / _pTex->GetHeight());
			frm.vSize = Vec2(((float)_ptCropSize.x) / (_pTex->GetWidth()), ((float)_ptCropSize.y) / _pTex->GetHeight());
			frm.fDuration = _Acctime;
			m_vecFrm.push_back(frm);
		}

		SetName(_strKey);
	}

	else
	{
		for (size_t j = 0; j < 2; ++j)
		{
			for (UINT i = 0; i < iFrmCount; ++i)
			{
				frm.pTex = _pTex;

				frm.vLT = Vec2(((float)_ptLT.x) / (_pTex->GetWidth()) + (i * (float)_iWGap / _pTex->GetWidth()), ((float)_ptLT.y) / _pTex->GetHeight() +  (j * (float)(_pTex->GetHeight() / 2.0f) / _pTex->GetHeight()));
				frm.vSize = Vec2(((float)_ptCropSize.x) / (_pTex->GetWidth()), ((float)_ptCropSize.y) / _pTex->GetHeight());
				frm.fDuration = _Acctime;
				m_vecFrm.push_back(frm);
			}

			SetName(_strKey);
		}
		
	}
	
}

void CAnimation2D::Create(const wstring & _strFolderPath, const wstring & _strKey, float _Acctime, bool _Repeat)
{	
	m_Matrix = { 1,1 };
	m_AnimLocVal = { 1,1 };
	m_bAnimRepeat = _Repeat;
	wstring strFilePath = CPathMgr::GetResPath() + _strFolderPath + L"\\*.png";
	wstring strRelativePath;


	m_bSynthesisAtlas = false;


	WIN32_FIND_DATA data = {};
	HANDLE hFind = FindFirstFile(strFilePath.c_str(), &data);

	

	tAnimFrm frm = {};
	while (true)
	{
		frm.fDuration = _Acctime;
		frm.vLT = Vec2(0.f, 0.f);
		frm.vSize = Vec2(1, 1);


		strRelativePath = wstring(_strFolderPath + data.cFileName);

		
		//frm.pTex = CResMgr::GetInst()->Load<CTexture>(strRelativePath, strRelativePath);
		frm.pTex = CResMgr::GetInst()->Load<CTexture>(strRelativePath);

		if (frm.pTex == NULL)
		{
			frm.pTex = CResMgr::GetInst()->Load<CTexture>(strRelativePath, strRelativePath);

			
		
			

			m_vecFrm.push_back(frm);

			if (!FindNextFile(hFind, &data))
				break;
		}

		else
		{
			m_vecFrm.push_back(frm);
			if (!FindNextFile(hFind, &data))
				break;
		}
	}

	FindClose(hFind);
}

void CAnimation2D::Delete(const wstring & _strKey)
{
	CResMgr::GetInst()->SubResource<CTexture>(_strKey);

	m_vecFrm.clear();
}

float CAnimation2D::GetDuration(int _idx)
{

	if (m_vecFrm.size() == 0)
	{
		return 0;
	}

	if ((UINT)_idx > m_vecFrm.size())
	{
		MessageBox(NULL, L"Currently overhangs of vector", L"Error Page.", MB_OK);
		_idx = 0;
	}

	return (float)m_vecFrm[_idx].fDuration;
}

void CAnimation2D::SetDuration(float _AccTime)
{
	UINT vecSize = (UINT)m_vecFrm.size();

	for (size_t i = 0; i < vecSize; ++i)
	{
		m_vecFrm[i].fDuration = _AccTime;
	}
}


void CAnimation2D::Save(FILE * _pFile)
{	
	SaveWString(GetName(), _pFile);	

	UINT iFrmCount = (UINT)m_vecFrm.size();
	fwrite(&iFrmCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iFrmCount; ++i)
	{
		fwrite(&m_vecFrm[i].fDuration, sizeof(float), 1, _pFile);
		fwrite(&m_vecFrm[i].vLT, sizeof(Vec2), 1, _pFile);
		fwrite(&m_vecFrm[i].vOffset, sizeof(Vec2), 1, _pFile);
		fwrite(&m_vecFrm[i].vSize, sizeof(Vec2), 1, _pFile);

		SaveWString(m_vecFrm[i].pTex->GetName(), _pFile);
	}	

	
	fwrite(&m_Matrix, sizeof(POINT), 1, _pFile);
	fwrite(&m_AnimLocVal, sizeof(POINT), 1, _pFile);
	fwrite(&m_bFullAts, sizeof(bool), 1, _pFile);
	fwrite(&m_bAnimRepeat, sizeof(bool), 1, _pFile);
	fwrite(&m_bSynthesisAtlas, sizeof(bool), 1, _pFile);

}

void CAnimation2D::Load(FILE * _pFile)
{
	SetName(LoadWString(_pFile));

	UINT iFrmCount = (UINT)m_vecFrm.size();
	fread(&iFrmCount, sizeof(UINT), 1, _pFile);

	tAnimFrm tFrm = {};

	for (UINT i = 0; i < iFrmCount; ++i)
	{
		fread(&tFrm.fDuration, sizeof(float), 1, _pFile);
		fread(&tFrm.vLT, sizeof(Vec2), 1, _pFile);
		fread(&tFrm.vOffset, sizeof(Vec2), 1, _pFile);
		fread(&tFrm.vSize, sizeof(Vec2), 1, _pFile);

		wstring strKey = LoadWString(_pFile);
		tFrm.pTex = CResMgr::GetInst()->Load<CTexture>(strKey);

		m_vecFrm.push_back(tFrm);
	}


	fread(&m_Matrix, sizeof(POINT), 1, _pFile);
	fread(&m_AnimLocVal, sizeof(POINT), 1, _pFile);
	fread(&m_bFullAts, sizeof(bool), 1, _pFile);
	fread(&m_bAnimRepeat, sizeof(bool), 1, _pFile);
	fread(&m_bSynthesisAtlas, sizeof(bool), 1, _pFile);
}
