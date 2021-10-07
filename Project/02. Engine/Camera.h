#pragma once
#include "Component.h"

class CLayer;
class CCamera :
	public CComponent
{
private:
	Matrix			m_matView;
	Matrix			m_matProj;
	Matrix			m_matPrevProj; // ��ȭ�� ���۵� �� ��İ�

	float			m_fNear;	// �ּҰŸ�
	float			m_fFar;		// �ִ�Ÿ�
	float			m_fFOV;		// �þ߰� (��������)
	float			m_fScale;	// ���� ũ��(��������)

	tResolution		m_tResolution;
	UINT			m_iLayerMask;

	bool			m_bPerspective;
	bool			m_bTransforming; // ��������� ���� �� ���
	float			m_fSpeed;        // ��ȭ �ӵ�
	float			m_fRatio;		 // ��ȭ ��ġ
	
public:
	float GetNear() { return m_fNear; }
	void SetNear(float _fNear) { m_fNear = _fNear; }
	
	float GetFar() { return m_fFar; }
	void SetFar(float _fFar) { m_fFar = _fFar; }

	float GetFOV() { return m_fFOV; }
	void SetFOV(float _fFOV) { m_fFOV = _fFOV; }

	float GetScale() { return m_fScale; }
	void SetScale(float _fScale) { m_fScale = _fScale; }

	bool IsPerspective() { return m_bPerspective; }
	void SetPerspective(bool _bTrue) 
	{ 
		m_bPerspective = _bTrue;

		if (XMMatrixIsIdentity(m_matProj))
			return;		

		if (m_bPerspective)
			m_fSpeed = 0.1f;
		else
			m_fSpeed = 10.f;
				
		m_matPrevProj = m_matProj;
		m_bTransforming = true;
		m_fRatio = 0.f;
	}

	void CheckLayer(UINT _iIdx);
	void CheckLayer(const wstring& _strLayerName);
	void CheckLayerAll() { m_iLayerMask = 0xffffffff; }
	void CheckLayerClear() { m_iLayerMask = 0; }
	UINT GetLayerMask() { return m_iLayerMask; }

public:
	virtual int update();
	virtual int finalupdate();
	void render(CLayer** _pLayer);

public:
	CLONE(CCamera);
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CCamera();
	virtual ~CCamera();
};

