#pragma once
#include "Entity.h"
#include "Ptr.h"

class CAnimator2D;
class CTexture;
struct tAnimFrm
{
	Ptr<CTexture>   pTex;		// ���� �̹���
	Vec2			vLT;		// �����ϴ� �̹��� ������, ����� ������ �� ��� ��ġ(UV ��ǥ ����)
	Vec2			vOffset;	// ptLT ��ǥ�� ������ �����ġ					  (UV ��ǥ ����)
	Vec2			vSize;		// ����� ������ ũ��								  (UV ��ǥ ����)
	float			fDuration;	// ���� �ð�

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

