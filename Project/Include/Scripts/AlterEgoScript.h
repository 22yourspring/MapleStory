#pragma once
#include "Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

//#include "AnrealEngine/Well512Random.h"

class CPlayerScript;
class CAlterEgoScript :
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
	PAPULATUS_FSM		m_MonsterState;
	PAPULATUS_FSM		Prev_MonsterState;

	static Vec3	vParentPos;

	static int AlterEgoActiveCount;

	static const long long ALTEREGO_MAX_HP;
	static const long long ALTEREGO_MAX_MP;

	float	m_fSleepAcc;
	float	IdleAcc;

	float	RandomTime;
	float	Rand_SleepTime;
	float	PushedAcc;

	bool	GetMovingSpeedCheck;
	float	MovingSpeed;

	void	ReturnAlter();

	float	DistanceValue;
	bool	DistanceCheck;

	bool	ReturnCheck;

	DIR		m_MonsterDir;

	bool	m_bTracing;
	bool	LeftFromPlayer();
	bool	UpFromPlayer();
	bool	Patrol_On;

	bool	m_bPatrolRadder;
	bool	m_bPatrolLeft;
	bool	m_bPatrolRight;

	bool	YFixing;

	void state();
	void ChangeState();
	void CurDir();

	void SetDir();
	void SectionMove();
	void PlayerTracing();

	void PatrolLeft();
	void PatrolRight();

	void YFixingFunc();

	virtual void awake();
	virtual int update();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);




public:
	CLONE(CAlterEgoScript);

public:
	CAlterEgoScript();
	virtual ~CAlterEgoScript();
};

