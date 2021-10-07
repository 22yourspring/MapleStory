#pragma once
#include "Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

//#include "AnrealEngine/Well512Random.h"

class CPlayerScript;
class CGiganticvikingScript :
	public CScript
{
private:
	Ptr<CPrefab>	m_pBullet;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

	Vec3	vPos;
	Vec3	vRot;

	CGameObject*	MonsterTerrain;

	bool	m_Left;

	Vec3 TerrainPos;
	Vec3 TerrainScale;


public:
	MONSTER_FSM		m_MonsterState;
	MONSTER_FSM		Prev_MonsterState;

	float	DistanceValue;
	bool	DistanceCheck;

	float m_fSleepAcc;
	float IdleAcc;

	float RandomTime;
	float Rand_SleepTime;
	float PushedAcc;

	static const long long GIGANTICVIKING_MAX_HP;
	static const long long GIGANTICVIKING_MAX_MP;


	DIR		m_MonsterDir;

	bool	m_bTracing;
	bool	LeftFromPlayer();
	bool	Patrol_On;

	bool	m_bPatrolRadder;
	bool	m_bPatrolLeft;
	bool	m_bPatrolRight;

	void state();
	void ChangeState();
	void CurDir();

	void SetDir();
	void SectionMove();
	void PlayerTracing();

	void PatrolLeft();
	void PatrolRight();

	virtual void awake();
	virtual int update();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);




public:
	CLONE(CGiganticvikingScript);

public:
	CGiganticvikingScript();
	virtual ~CGiganticvikingScript();
};

