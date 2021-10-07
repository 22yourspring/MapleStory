#pragma once
#include "Script.h"

class CCamCollScript :
	public CScript
{
	CGameObject*	m_pCamTarget;

	Vec3 vPos;
	Vec3 PlayerPos;
	Vec3 vRot;
	Vec3 vFront;
	Vec3 vRight;

	Vec3 m_TerrainvPos;
	Vec3 m_TerrainvScale;

	Vec3 PrevPos;

	float fData;

public:
	virtual void awake();
	virtual int update();

	bool iscollision;
	bool isUpcollision;
	bool prevCollision;

	float PlusY;

	bool CameraOn;

	UINT EarthQuakeCamPos = 10;

	Vec3 GetCameraPos() { return vPos; }

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);


public:
	CLONE(CCamCollScript);


public:
	CCamCollScript();
	virtual ~CCamCollScript();
};

