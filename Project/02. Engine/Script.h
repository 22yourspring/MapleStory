#pragma once
#include "Component.h"
#include "Prefab.h"


class CScript :
	public CComponent
{	
private:
	UINT	m_iScriptType;

protected:
	void Instantiate(Ptr<CPrefab> _pPrefab, const wstring& _strLayerName, const Vec3& _vPos);
	void DestroyObject(CGameObject* _pTargetObj);
	void DisconnectParent(CGameObject* _pTargetObj);
	void DestroyObject();
	void DisconnectParent();

	void AddChild(CGameObject* _pChild);
	void AddChild(CGameObject* _pParent, CGameObject* _pChild);


//Unit
private:
	long long	Object_HP;
	long long	Object_MP;
	long long	Object_Value_HP;
	long long	Object_Value_MP;

private:
	bool isBePushed;
	

public:
	CGameObject* Script_TERRAIN;

public:
	float AlphaTime;
	float BetaTime;
	float EclipseTime;

	float RegenarationTime;

	long long GetHP() { return Object_HP; }
	long long GetMP() { return Object_MP; }

	long long GetValueHP() { return Object_Value_HP; }
	long long GetValueMP() { return Object_Value_MP; }

	void SetValueHP(long long _SetValueHP) { Object_Value_HP = _SetValueHP; }
	void SetValueMP(long long _SetValueMP) { Object_Value_MP = _SetValueMP; }

	void AddHP(long long _Healing);
	void AddMP(long long _Healing);

	void SubHP(long long _Damage);
	void SubMP(long long _Damage);

	static int DeathCount;

	void SubDeathCount();
	void AddDeathCount();

	void Regeneration(CGameObject* _Object);
	void Regeneration(CGameObject* _Object, bool _AlterEgo);

	void KillObject(CGameObject* _Object);

	bool Confirm_Life(CGameObject* _Object);

	void SetPushed(bool _Pushed) { isBePushed = _Pushed; }
	bool GetPushed() { return isBePushed; }

	UINT PresentSKILL;

	void SetSkillType(UINT _SKILL_TYPE) { PresentSKILL = _SKILL_TYPE; }
	UINT GetSkillType() { return PresentSKILL; }

	bool	m_bAggro;

	bool	RagingCollision;
	bool	InstinctCollision;
	bool	DeathFaultCollision;
	bool	ShoutCollision;
	bool	AlphaCollision;
	bool	BetaCollision;
	bool	EclipseCollision;

	int RagingEffectCount;
	float RagingCount;

	int InstinctEffectCount;
	float InstinctCount;

	int DeathFaultEffectCount;
	float DeathFaultCount;

	int ShoutEffectCount;
	float ShoutCount;

	int AlphaEffectCount;
	float AlphaCount;

	int BetaEffectCount;
	float BetaCount;

	int EclipseEffectCount;
	float EclipseCount;

//
public:
	virtual void OnCollisionEnter(CCollider* _pOther) {}
	virtual void OnCollision(CCollider* _pOther) {}
	virtual void OnCollisionExit(CCollider* _pOther) {}

public:
	UINT GetScriptType() { return m_iScriptType; }

protected:
	void SetScriptType(UINT _iType) { m_iScriptType = _iType; }

public:
	virtual void Save(FILE* _pFile) {};
	virtual void Load(FILE* _pFile) {};

public:
	CScript();
	CScript(long long _PlayerHP, long long _PlayerMP, long long _Player_Value_HP, long long _Player_Value_MP);

	CScript(long long _Object_HP, long long _Object_MP, long long _Object_Value_HP, long long _Object_Value_MP, UINT _PresentSKILL, bool _m_bAggro,
		bool _RagingCollision, bool _ShoutCollision, bool _DeathFaultCollision, bool _AlphaCollision, bool _BetaCollision, bool _EclipseCollision, bool _InstinctCollision,	
		int _RagingEffectCount, float _RagingCount, int _InstinctEffectCount, float _InstinctCount, int _DeathFaultEffectCount, float _DeathFaultCount, int _ShoutEffectCount, float _ShoutCount,
		int _AlphaEffectCount, float _AlphaCount, int _BetaEffectCount, float _BetaCount, int _EclipseEffectCount, float _EclipseCount, 	
		float _AlphaTime, float _BetaTime , float _EclipseTime);

	virtual ~CScript();
};

