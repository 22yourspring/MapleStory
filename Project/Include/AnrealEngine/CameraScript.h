#pragma once
#include "Script.h"

class CCameraScript :
	public CScript
{


public:
	virtual void awake();
	virtual int update();

public:
	CLONE(CCameraScript);

public:
	CCameraScript();
	virtual ~CCameraScript();
};

