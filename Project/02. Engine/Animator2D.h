#pragma once
#include "Component.h"


class CAnimation2D;
class CTexture;
class CAnimator2D :
	public CComponent
{
private:
	map<wstring, CAnimation2D*>	m_mapAnim;
	CAnimation2D *				m_pCurAnim;
	//bool						m_bRepeat;
	
public:
	virtual int update();

public:
	virtual void UpdateData();

	//bool GetRepeat() { return m_bRepeat; }
	//void SetRepeat(bool _bRepeat) { m_bRepeat = _bRepeat; }

	

public:
	CAnimation2D* GetCurAnim() { return m_pCurAnim; }
	
	void PlayAnim(const wstring& _strKey/*, bool _bRepeat*/);
	void AddAnimation(CTexture* _pTexture, const POINT& _ptLT, UINT _iWGap, UINT iFrmCount,
		const POINT& _ptCropSize, const wstring & _strKey, float _Acctime, POINT _matrix, POINT _AnimLocVale, bool _FullAts, bool _Repeat);

	// 낱장으로 로딩하는 경우, 폴더경로 지정
	void AddAnimation(const wstring& _strFolderPath, const wstring& _strKey, float _Acctime, bool _Repeat);

	void DelAnimation(const wstring& _strKey, const wstring & MAPKEY, int _DeleteMaxValue, int _CurValue);

	POINT FindPOINT(const wstring& _strKey);

	
	map<wstring, CAnimation2D*>& GetAnimMap() { return m_mapAnim; }
	static void ClearData();

public:
	CLONE(CAnimator2D);
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CAnimator2D();
	CAnimator2D(const CAnimator2D& _other);
	virtual ~CAnimator2D();
};

