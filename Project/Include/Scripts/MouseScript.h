#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"


class CMouseScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vScale;

	static bool		CurUIMoving;

	static Vec3		CurMoving;
	static Vec3		PrevMousePos;

	CGameObject*	SelectItem;
	CGameObject*	NextSelect;
	CGameObject*	PrevSocket;

	bool	MouseUp;
	bool	UISelect;

	bool	ItemSelect;

	virtual void awake();
	virtual int update();

	void ProcessPos();

	static 	CURSOR	Finger;
	static  CURSOR	PREV_Figner;


	void CursorChange();
	void UIMoving();
	void UIrelease();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

public:
	CLONE(CMouseScript);

public:
	CMouseScript();
	~CMouseScript();
};