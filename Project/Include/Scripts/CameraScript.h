#pragma once
#include "Script.h"

class CShoutScript;
class CCameraScript :
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
	CShoutScript* SHOUT;

	bool CameraOn;
	float GetCamScale();

	static float CamScale;

	Vec3 GetCameraPos() { return vPos; }

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);


public:
	CLONE(CCameraScript);
		

public:
	CCameraScript();
	virtual ~CCameraScript();
};

