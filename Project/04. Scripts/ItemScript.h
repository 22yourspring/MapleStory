#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

enum class ITEM_TYPE
{
	ELIXIR,
	VIKING,
	KEEPER,
	G_WATCH,
	CRACK,
	END,
};

enum class ITEM_STATE
{
	DROP,
	INVENTORY,
	MONSTER,
	END
};

class CItemScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vRot;

	ITEM_STATE	OBJECT_State;
	ITEM_STATE	PREV_OBJECTState;
	ITEM_TYPE	ItemType;

	bool	m_bSelect;

	virtual void awake();
	virtual int update();

	/*virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);*/

	void Skillstate();
	void SkillPos();
	void SkillChangeState();



public:
	CLONE(CItemScript);

public:
	CItemScript();
	~CItemScript();
};