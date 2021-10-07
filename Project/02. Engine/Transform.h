#pragma once
#include "Component.h"
class CTransform :
	public CComponent
{
private:
	Vec3		m_vLocalPos;
	Vec3		m_vLocalScale;
	Vec3		m_vLocalRot;
	Vec3		m_vLocalDir[(UINT)DIR_TYPE::END];
	Vec3		m_vWorldDir[(UINT)DIR_TYPE::END];

	Matrix		m_matWorld;

public:
	void SetLocalScale(const Vec3& _vScale) { m_vLocalScale = _vScale; }
	void SetLocalPos(const Vec3 _vPos) { m_vLocalPos = _vPos; }
	void SetLocalRot(const Vec3 _vRot) { m_vLocalRot = _vRot; }

	const Vec3& GetLocalPos() { return m_vLocalPos; }
	Vec3 GetWorldPos();	   	 
	const Vec3& GetLocalScale() { return m_vLocalScale; }
	const Vec3& GetLocalRot() { return m_vLocalRot; }
	const Vec3& GetLocalDir(DIR_TYPE _eType) { return m_vLocalDir[(UINT)_eType]; }
	const Vec3& GetWorldDir(DIR_TYPE _eType) { return m_vWorldDir[(UINT)_eType]; }
	const Matrix& GetWorldMat() { return m_matWorld; }



public:
	virtual int update();
	virtual int finalupdate();

	void UpdateData();

public:
	CLONE(CTransform);
	virtual void Save(FILE* _pFile);
	virtual void Load(FILE* _pFile);


public:
	CTransform();
	virtual ~CTransform();
};

