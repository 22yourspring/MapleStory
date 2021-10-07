#pragma once
#include "Component.h"
#include "Ptr.h"

class CMesh;
class CMaterial;
class CCollider :
	public CComponent
{
private:
	static DWORD	g_iID;

private:
	COLLIDER_TYPE	m_eType;		// �浹ü Ÿ��	
	Vec3			m_vOffsetPos;	// ������� ��ġ
	Vec3			m_vOffsetScale;	// �浹ü ũ�� ���밪(�θ� ������Ʈ�� ������ ���� �ʴ´�.)	
	Matrix			m_matCol;
	Ptr<CMaterial>	m_pMtrl;
	Ptr<CMaterial>  m_pCloneMtrl;
	Ptr<CMesh>		m_pMesh;
	float			m_fRadius;		// ��, �� �� ������

	float			m_fWorldRadius;	// ���� ����󿡼� ǥ���Ǵ� ��, �� �� ������
	DWORD			m_dwID;

public:
	void SetOffsetPos(const Vec3& _vPos) { m_vOffsetPos = _vPos; }
	void SetOffsetScale(const Vec3& _vScale) { m_vOffsetScale = _vScale; }
	
	const Vec3& GetOffsetPos() {return m_vOffsetPos; }
	const Vec3& GetOffsetScale() {return m_vOffsetScale; }
	const Matrix& GetColMat() { return m_matCol; }
	COLLIDER_TYPE GetColliderType() { return m_eType; }
	float GetWorldRadius() { return m_fWorldRadius; }

	DWORD GetID() { return m_dwID; }

private:
	void CalColliderMat();

public:
	virtual int update();
	virtual int finalupdate();
	void render();

public:
	void OnCollisionEnter(CCollider* _pOther);
	void OnCollision(CCollider* _pOther);
	void OnCollisionExit(CCollider* _pOther);
	   
	void SetColliderType(COLLIDER_TYPE _eType = COLLIDER_TYPE::COLLIDER_2D_BOX);

public:
	CLONE(CCollider);
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);

public:
	CCollider();
	CCollider(const CCollider& _other);
	virtual ~CCollider();
};

