#include "Animator2D.h"
#include "Animation2D.h"

#include "PathMgr.h"

#include "Device.h"
#include "ConstBuffer.h"

#include "Transform.h"
#include "Texture.h"

tAnim2DInfo g_anim2d = {};

CAnimator2D::CAnimator2D()
	: m_pCurAnim(nullptr)
{
	SetComponentType(COMPONENT_TYPE::ANIMATOR2D);
}

CAnimator2D::CAnimator2D(const CAnimator2D & _other)
	: m_pCurAnim(nullptr)
{
	SetComponentType(COMPONENT_TYPE::ANIMATOR2D);

	map<wstring, CAnimation2D*>::const_iterator iter = _other.m_mapAnim.begin();

	for (; iter != _other.m_mapAnim.end(); ++iter)
	{
		CAnimation2D* pCloneAnim = iter->second->Clone();
		m_mapAnim.insert(make_pair(iter->first, pCloneAnim));
		pCloneAnim->SetOwner(this);
	}

	if (nullptr != _other.m_pCurAnim)
	{
		PlayAnim(_other.m_pCurAnim->GetName());
	}
}

CAnimator2D::~CAnimator2D()
{
	Safe_Delete_Map(m_mapAnim);
}


int CAnimator2D::update()
{	
	if (nullptr != m_pCurAnim)
	{
		m_pCurAnim->update();


		if (m_pCurAnim->IsFinish() && m_pCurAnim->GetAnimRepeat())
			m_pCurAnim->SetFrmIdx(0);
	}

	return 0;
}

void CAnimator2D::UpdateData()
{
	if (nullptr == m_pCurAnim)
	{

		static CConstBuffer* pBuffer = CDevice::GetInst()->FindCBuffer(L"Anim2DInfo");
		g_anim2d.g_bAnim2dUse = false;
	
		pBuffer->UpdateData(&g_anim2d, sizeof(tAnim2DInfo));
		pBuffer->UpdateRegisterAll();

		return;
	}

	m_pCurAnim->UpdateData();
}

void CAnimator2D::PlayAnim(const wstring & _strKey)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);
	if (iter == m_mapAnim.end())
	{
		MessageBox(NULL, L"Animation does not exist.", L"Error Page.", MB_OK);
		return;
	}
	m_pCurAnim = iter->second;
	

	if (!(iter->second->GetSynthesisAtlas()))
	{
		if (GetGameObject())
		{
			if (iter != m_mapAnim.end())
			{
				if (!(iter->second->GetFullAts()))
				{
					Vec3 m_applyFC;
					m_applyFC.x = (iter->second->GetpTex(iter->second->GetCurFrm()))->GetWidth();
					m_applyFC.y = (iter->second->GetpTex(iter->second->GetCurFrm()))->GetHeight();
					m_applyFC.z = 0.f;

					GetGameObject()->Transform()->SetLocalScale(m_applyFC);
				}

			}

			if (GetGameObject()->GetName() != L"Player")
			{
				if (m_pCurAnim->GetCurFrm() != 0)
			//	if (m_pCurAnim->IsFinish())
				{
					m_pCurAnim->SetFrmIdx(0);
					return;
				}
			}
		}
	}



	
}

void CAnimator2D::AddAnimation(CTexture* _pTexture, const POINT & _ptLT, UINT _iWGap, UINT iFrmCount, const POINT & _ptCropSize, const wstring & _strKey, float _Acctime, POINT _matrix, POINT _AnimLocVale, bool _FullAts, bool _Repeat)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter != m_mapAnim.end())
	{
		MessageBox(NULL, L"The key you want to register already exists.", L"Error Page.", MB_OK);
		return;
	}
	//m_bRepeat = _Repeat;
	CAnimation2D* pNewAnim = new CAnimation2D;
	pNewAnim->Create(_pTexture, _ptLT, _iWGap, iFrmCount, _ptCropSize, _strKey, _Acctime, _matrix, _AnimLocVale, _FullAts, _Repeat);

	

	pNewAnim->SetOwner(this);
	m_mapAnim.insert(make_pair(_strKey, pNewAnim));	
}

void CAnimator2D::AddAnimation(const wstring & _strFolderPath, const wstring & _strKey, float _Acctime, bool _Repeat)
{	

	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter != m_mapAnim.end())
	{
		MessageBox(NULL, L"The key you want to register already exists.", L"Error Page.", MB_OK);
		return;
	}


	CAnimation2D* pNewAnim = new CAnimation2D;
	pNewAnim->Create(_strFolderPath, _strKey, _Acctime, _Repeat);
	pNewAnim->SetName(_strKey);
	

	pNewAnim->SetOwner(this);

	if (pNewAnim->GetpTex() == NULL)
	{
		return;
	}

	m_mapAnim.insert(make_pair(_strKey, pNewAnim));
}

void CAnimator2D::DelAnimation(const wstring & _strKey, const wstring & MAPKEY, int _DeleteMaxValue, int _CurValue)
{
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter != m_mapAnim.end())
	{
		if (iter->second == m_pCurAnim)
		{
			MessageBox(NULL, L"Animation currently running.", L"Error Page.", MB_OK);
			return;
		}


		if (iter == m_mapAnim.end())
		{
			MessageBox(NULL, L"Key not found to be deleted.", L"Error Page.", MB_OK);
			return;
		}

		else if (_strKey == iter->first)
		{
			iter->second->Delete(MAPKEY);

			if (_DeleteMaxValue == _CurValue + 1)
			{
				CAnimation2D* pTemp = iter->second;
				m_mapAnim.erase(iter);
				delete pTemp;
			}
		}
	}
}

POINT CAnimator2D::FindPOINT(const wstring & _strKey)
{
	// TODO: 여기에 반환 구문을 삽입합니다.

	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.find(_strKey);

	if (iter == m_mapAnim.end())
	{
		MessageBox(NULL, L"It is not a registered key.", L"Error Page.", MB_OK);
		return POINT{0,0};
	}

	else  if(_strKey == iter->first)
	{
		return iter->second->GetMatrix();
	}

	else
	{
		return POINT{ 0,0 };
	}

}


void CAnimator2D::ClearData()
{
	static CConstBuffer* pBuffer = CDevice::GetInst()->FindCBuffer(L"Anim2DInfo");
	pBuffer->ClearData();
}

void CAnimator2D::Save(FILE * _pFile)
{
	UINT iAnimCount = (UINT)m_mapAnim.size();
	fwrite(&iAnimCount, sizeof(UINT), 1, _pFile);
	
	map<wstring, CAnimation2D*>::iterator iter = m_mapAnim.begin();
	for (; iter != m_mapAnim.end(); ++iter)
	{

		iter->second->Save(_pFile);
	}

	bool bCurAnim = false;
	if (nullptr != m_pCurAnim)
	{
		bCurAnim = true;
	}
	fwrite(&bCurAnim, 1, 1, _pFile);

	if (bCurAnim)
	{
		SaveWString(m_pCurAnim->GetName(), _pFile);
	}
}

void CAnimator2D::Load(FILE * _pFile)
{
	UINT iAnimCount = 0;
	fread(&iAnimCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iAnimCount; ++i)
	{
		CAnimation2D* pAnim = new CAnimation2D();
		pAnim->Load(_pFile);
		
		pAnim->SetOwner(this);

		m_mapAnim.insert(make_pair(pAnim->GetName(), pAnim));
	}

	bool bCurAnim = false;
	fread(&bCurAnim, 1, 1, _pFile);

	if (bCurAnim)
	{
		wchar_t* pstrCurAnimName = LoadWString(_pFile);		
		PlayAnim(pstrCurAnimName);
	}
}
