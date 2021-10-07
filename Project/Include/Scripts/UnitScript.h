#pragma once
#include "AnrealEngine/Script.h"

class CUnitScript :
	public CScript
{
private:
	int	Object_HP;
	int	Object_MP;
	int	Object_Value_HP;
	int	Object_Value_MP;

private:
	bool isBePushed;

public:
	int GetHP() { return Object_HP; }
	int GetMP() { return Object_MP; }

	int GetValueHP() { return Object_Value_HP; }
	int GetValueMP() { return Object_Value_MP; }

	void AddHP(int _Healing);
	void AddMP(int _Healing);
	void SubHP(int _Damage);
	void SubMP(int _Damage);

	static int DeathCount;

	void SubDeathCount();
	void AddDeathCount();

	void Regeneration(CGameObject* _Object);
	void KillObject(CGameObject* _Object);

	bool Confirm_Life(CGameObject* _Object);

	void SetPushed(bool _Pushed) { isBePushed = _Pushed; }
	bool GetPushed() { return isBePushed; }

	UINT PresentSKILL;

	void SetSkillType(UINT _SKILL_TYPE) { PresentSKILL = _SKILL_TYPE; }
	UINT GetSkillType() { return PresentSKILL; }

	virtual int update() { return 0; }
	virtual int lateupdate() { return 0; }


public:
	CLONE(CUnitScript);

public:
	CUnitScript();
	CUnitScript(UINT _Object_HP, UINT _Object_MP, UINT _Object_Value_HP, UINT _Object_Value_MP);
	~CUnitScript();
};

