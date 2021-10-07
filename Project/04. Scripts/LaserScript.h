#pragma once
#include "AnrealEngine/Script.h"
#include "AnrealEngine/Prefab.h"
#include "AnrealEngine/Material.h"

class CLaserScript :
	public CScript
{
public:
	Ptr<CPrefab>	m_pEffectPrefab;
	Ptr<CMaterial>  m_pSharedMtrl;
	Ptr<CMaterial>	m_pCloneMtrl;

public:
	Vec3			vPos;
	Vec3			vRot;

	static Vec3     vRedPos;
	static Vec3		vBluePos;

	static float	RedTheta; // 시작 각도
	static float	BlueTheta;
	
	bool		isInit;

	float	AddTheta; // 변하는 양
	float	AddRedX;
	float	AddRedY;
	float	AddBlueX;
	float	AddBlueY;

	bool	RedSlow;
	bool	FindStart;


	static float RedAssist;
	static float BlueAssist;

	static float	RedAcc;
	static float	BlueAcc;

	static bool RedCollReady;
	static bool BlueCollReady;
	static bool RedCutting;
	static bool BlueCutting;

	bool	Fir_FirLaserSet;
	static bool	Fir_FirLaser;
	bool	LaserSetting;

	bool	FirstAccess; // 최초 1번 실행
	static bool	LaserAccess; // Laser 실행 하는가 

	static float AddTime;
	static void SetAddTime(float _SetFloat) { AddTime = _SetFloat; };

	static int LaserPattern;
	static int PrevLaserPat;
	static int LaserType;
	void SetLaserPattern();

	static bool CollisionReady;

	static float LasingTime;

	LASER_FSM	LASER_STATE;
	LASER_FSM	PREV_LASER_STATE;

	bool	LaserIdentifierChecker;
	LASER_TYPE	Laser_Identifier;

	void LaserAcc();
	static bool Accoff;

	virtual void awake();
	virtual int update();

	void OffLaser();

	void ThetaUpdate();

	void LaserCheck();

	void LaserRot();

	void Skillstate();
	void SkillChangeState();

	void AccCheck();
	void LaserCollision();

public:
	CLONE(CLaserScript);

public:
	CLaserScript();
	~CLaserScript();
};