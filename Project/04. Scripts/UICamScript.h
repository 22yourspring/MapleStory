#pragma once
#include "Script.h"

class CUICamScript :
	public CScript
{
	CGameObject*	m_pCamTarget;

	Vec3 vPos;
	Vec3 vRot;
	Vec3 vFront;
	Vec3 vRight;

	Vec3 PrevPos;

	float fData;

public:
	virtual void awake();
	virtual int update();

	Vec3 GetCameraPos() { return vPos; }
	
	void holdPosition();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);


public:
	CLONE(CUICamScript);


public:
	CUICamScript();
	virtual ~CUICamScript();
};

