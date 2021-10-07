#pragma once
#include "Script.h"

class CBulletScript :
	public CScript
{
public:
	virtual int update();

public:
	CLONE(CBulletScript);

public:
	CBulletScript();
	virtual ~CBulletScript();
};

