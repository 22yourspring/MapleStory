#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

enum CRANE_DIR
{
	IDLE,
	DOWN,
	UP,
	HOLD,
	END,
};

class CCraneScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			startPos;
	DIR				SKILL_DIR;

	CRANE_FSM	OBJECT_State;
	CRANE_FSM	PREV_OBJECTState;

	CRANE_DIR	CurDIR;
	CRANE_DIR	PrevDir;

	virtual void awake();
	virtual int update();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	void Skillstate();
	void SkillPos();
	void SkillChangeState();

	bool Phase1Start;
	bool Script_start;
	void saveStartPos();

	float AccessTime;
	void F_Time(); //최초 Crane 내리기 판정

	bool FirstTime;

	float m_fIdle_Up;
	void IdleUpAcc();

	float m_fIdle_Down;
	void IdleDownAcc();

	void BindingPlayer();

	static bool CraneAction;

public:
	CLONE(CCraneScript);

public:
	CCraneScript();
	~CCraneScript();
};