#pragma once
#include "Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

#include "AnrealEngine/Well512Random.h"

class CInstinctActiveScript;
class CShadowAlphaScript;
class CShadowBetaScript;
class CBoosterScript;
class CPlayerScript :
	public CScript
{
private:
	Ptr<CPrefab>	m_pBullet;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

	Vec3 vPos;
	Vec3 vRot;
	Vec3 CurPos;

	Vec3 PrevPos;

public:
	virtual void awake();
	virtual int update();

	static Vec3 EffectPlayerPos;
	static float EffectYvalue;
	static Vec3 PlayerPos;
	static Vec3 PlayerSkillPos;

	static CGameObject* PlayerObject;
	static CScript* PlayerToScript;

	void	state();
	void	ChangeState();
	void	CurDir();

	static CGameObject*	PlayerTerrain;

	static float	m_fSpeed;
	float	m_fAccSpeed;
	static float	m_fJumpForce;
	static float	FLASH_JUMP_F;

	static bool	HangUp;
	static bool	m_IsJump;
	static bool	TerrainIn;
	static bool	m_isHangOn;

	static bool m_bAttackON;

	static bool	PushLeft;
	static bool	PushRight;

	static bool	leapTerrain;

	static bool PushedLeft;
	static bool Invincibility;
	static bool	isCanControl;

	static float InvincibilityAcc;
	static float PushedAcc;

	static UINT	CollisionCount;
	
	static float PushForce;

	bool PushBackTrue;

	void SetInvincibility(bool _Invincibility) { Invincibility = _Invincibility; }
	bool GetInvincibility() { return Invincibility; }

	static int	MIN_ATT;
	static int	MAX_ATT;

	static int	CRITICAL;
	static int  MAPLESOLDIER;

	Vec3	GetPrevPos() { return PrevPos; }
	void	SetPrevPos(Vec3 _CurPos) { PrevPos = vPos; }

	void	SetLadderGravity(bool _GravitySet);
	void	SetLadderJumpForce(float _JumpForce);
	void	SetGravityInit();

	bool	FirstTimePush;

	void	PlayerBe_LEFT_Pushed();
	void	PlayerBe_RIGHT_Pushed();

	static FSM		m_PlayerState;
	static FSM		Prev_PlayerState;
	static DIR		m_PlayerDir;

	static DIR		GetPlayerDir() { return m_PlayerDir; }
	static FSM		GetPlayerState() { return m_PlayerState; }
	static Vec3		GetPlayerPos() { return PlayerPos; }

	static void		SetPlayerPos(Vec3 _SetPos);
	
	static void		SetPlayerState(FSM _PlayerState) { m_PlayerState = _PlayerState; }

	void PushReset();

	static float Idle;

	CShadowAlphaScript* m_pAlphaScript;
	CShadowBetaScript* m_pBetaScript;

	UINT	m_iSwingCount;

	bool	m_bIsInstall;
	
	bool	m_bLeftAirWork;
	bool	m_bRightAirWork;

	bool	CurChange;

	static bool	DownJump;

	bool	LeftJump;
	bool	RightJump;
	bool	NormalJump;

	static UINT		Player_COMBO_COUNT;
	static UINT		COMBO_MAXCOUNT;
	static bool		COMBO_ON;

	static const long long PLAYER_MAX_HP;

	bool	Enrage;

	void ComboAttackSet();

	static UINT		GetComboCount() { return Player_COMBO_COUNT; }
	static void		SetComboCount(UINT _SetComboCount) { Player_COMBO_COUNT = _SetComboCount; }

	void Jump();
	void Attack();
	void FlashAttack();


	virtual void OnCollisionEnter(CCollider * _pOther);
	virtual void OnCollision(CCollider * _pOther);
	virtual void OnCollisionExit(CCollider * _pOther);


public:
	CLONE(CPlayerScript);

public:
	CPlayerScript();
	virtual ~CPlayerScript();
};

