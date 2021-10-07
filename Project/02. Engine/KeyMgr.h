#pragma once
#include "global.h"

struct tKey
{
	KEY_TYPE	eType;
	KEY_STATE	eState;
};

class CKeyMgr
{
	SINGLE(CKeyMgr);
private:
	vector<tKey>	m_vecKey;
	Vec2			m_vMousePos;

public:
	bool GetKeyState(KEY_TYPE _eType, KEY_STATE _eState)
	{
		if (_eState == m_vecKey[(UINT)_eType].eState)
			return true;
		else
			return false;
	};

	const Vec2& GetMousePos() { return m_vMousePos; }

public:
	void init();
	void update();
};
