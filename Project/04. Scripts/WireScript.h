#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"


class CWireScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vScale;
	Vec3		startPos;

	bool		Script_start;

	CRANE_FSM	OBJECT_State;
	CRANE_FSM	PREV_OBJECTState;

	virtual void awake();
	virtual int update();

	void SkillPos();

	void saveStartPos();

public:
	CLONE(CWireScript);

public:
	CWireScript();
	~CWireScript();
};