#pragma once
#include "Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

//#include "AnrealEngine/Well512Random.h"

class CPlayerScript;
class CMobGravityScript;
class CPapulatusScript :
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
	PAPULATUS_FSM		Papulatus_State;
	PAPULATUS_FSM		Prev_PapulatusState;

	PAPULATUS_PHASE1_PATTERN		SleepState;

	static PAPULATUS_PHASE1_PATTERN PatternDesign1;
	static PAPULATUS_PHASE2_PATTERN PatternDesign2;

	long long PapualtusHP;
	long long PrevPapualtusHP;
	static Vec3 TerrainPosCalling;
	static Vec3 TerrainScaleCalling;

	static bool Phase1Start;

	static float SleepHealCount;
	bool DoSleepHeal;

	void DoCount();

	static bool TimeBombsOn;

	void TimeBombsCount();

	static bool PapulatusFallen;

	static int RandomSkill;
	static bool CurseCheck;
	static bool AlramingCheck;
	static bool TorrentCheck;

	static float AccTorrent;
	static float AccAlterEgo;
	static bool AlterEgoReady;
	static bool AlterEgoActive;
	static bool Prev_AlterActive;

	static bool HealingActive;

	static bool CrackOfCheck;

	static bool LaserOffAction;

	static int AlterEgo_SurviveCount;

	static bool AlterEgo_Distinguish;

	static void AlterEgoSetCount() { AlterEgo_SurviveCount += 1; }

	int TempSkill;

	float m_fSleepAcc;
	float IdleAcc;

	float RandomTime;
	float Rand_SleepTime;
	float PushedAcc;

	static float CurAcctime;

	DIR		m_MonsterDir;

	bool	m_bTracing;
	bool	LeftFromPlayer();
	bool	UpFromPlayer();
	bool	YSort;

	static CPapulatusScript* PapulatusScriptPtr;

	static bool		Phase2ReverseGravity;

	static DIR PapulatusDir();

	bool	Patrol_On;

	bool	m_bPatrolRadder;
	bool	m_bPatrolLeft;
	bool	m_bPatrolRight;

	bool	DeadLockProcess_CrackLaser;

	bool	AlramDeadLockProcessor;
	bool	SleepDeadLockProcessor;

	static bool	Phase0Check;
	static bool	Phase1Check;
	static bool	Phase2Check;

	static bool	Phase0Pass;
	static bool	Phase1Pass;
	static bool	Phase2Pass;

	static bool	TimeBombCheck;

	static const long long Phase0HP;
	static const long long Phase1HP;
	static const long long Phase2HP;

	static const long long PAPULATUS_MAX_MP;

	void state();
	void ChangeState();
	void CurDir();

	void SetDir();
	void SectionMove();
	void PlayerTracing();

	void TorrentTimeCheck();
	void AlterEgoCheck();
	void SetMove();

	static void BlinkPlayer();

	void PapulatusFSM();
	
	static CGameObject* PapulatusPtr;

	void PatrolLeft();
	void PatrolRight();

	virtual void awake();
	virtual int update();

	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);

	CMobGravityScript* GravityPtr;


public:
	CLONE(CPapulatusScript);

public:
	CPapulatusScript();
	virtual ~CPapulatusScript();
};

