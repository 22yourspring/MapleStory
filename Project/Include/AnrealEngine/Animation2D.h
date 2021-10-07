#pragma once
#include "Entity.h"
#include "Ptr.h"

class CAnimator2D;
class CTexture;
struct tAnimFrm
{
	Ptr<CTexture>   pTex;		// 참조 이미지
	Vec2			vLT;		// 참조하는 이미지 내에서, 사용할 부위의 좌 상단 위치(UV 좌표 기준)
	Vec2			vOffset;	// ptLT 좌표에 더해질 상대위치					  (UV 좌표 기준)
	Vec2			vSize;		// 사용할 부위의 크기								  (UV 좌표 기준)
	float			fDuration;	// 지속 시간

public:
	tAnimFrm();
	~tAnimFrm();
};

class CAnimation2D :
	public CEntity
{
private:
	CAnimator2D *		m_pOwner;
	vector<tAnimFrm>	m_vecFrm;

	int					m_iCurFrm;
	float				m_fAccTime;
	bool				m_bFinish;

	POINT				m_Matrix;
	POINT				m_AnimLocVal;
	bool				m_bFullAts;
	bool				m_bAnimRepeat;
	bool				m_bSynthesisAtlas;


public:
	virtual int update();

public:
	virtual void UpdateData();

public:
	int GetCurFrm() { return m_iCurFrm; }

	vector<tAnimFrm>&	GetFrmVec() { return m_vecFrm; }

	void SetOffset(int _idx, float x, float y) 
	{
		m_vecFrm[_idx].vOffset.x = x; 
		m_vecFrm[_idx].vOffset.y = y; 
	}

	Vec2 GetOffset(int _idx)
	{
		return m_vecFrm[_idx].vOffset;
	}

	void SetOwner(CAnimator2D* _pAnimator) { m_pOwner = _pAnimator; }
	CAnimator2D*	GetOwner() { return m_pOwner; }
	
	Ptr<CTexture> GetpTex(int  = 0);
	
	void SetFrmIdx(int _iIndex);

	void Create(CTexture * _pTex, const POINT & _ptLT
		, UINT _iWGap, UINT iFrmCount, const POINT & _ptCropSize, const wstring& _strKey, float _Acctime, POINT _matrix, POINT _AnimLocVal, bool _FullAts, bool _Repeat);
	void Create(const wstring& _strFullPath, const wstring& _strKey, float _Acctime, bool _Repeat);

	void Delete(const wstring& _strKey);

	void SetMatrix(POINT _matrix) { m_Matrix = _matrix; }
	POINT	GetMatrix() { return m_Matrix; }

	bool IsFinish() { return m_bFinish; }

	bool GetFullAts() { return m_bFullAts; }
	void SetFullAts(bool _bFullAts) { m_bFullAts = _bFullAts; }


	bool GetAnimRepeat() { return m_bAnimRepeat; }
	void SetAnimRepeat(bool _Repeat) { m_bAnimRepeat = _Repeat; }

	bool GetSynthesisAtlas() { return m_bSynthesisAtlas; }
	void SetSynthesisAtlas(bool _Repeat) { m_bSynthesisAtlas = _Repeat; }

	float GetDuration(int _idx = 0);
	void SetDuration(float _AccTime);


	POINT GetAnimLocVal() { return m_AnimLocVal; }

public:
	CLONE(CAnimation2D);
	void Save(FILE* _pFile);
	void Load(FILE* _pFile);

public:
	CAnimation2D();
	virtual ~CAnimation2D();
};

