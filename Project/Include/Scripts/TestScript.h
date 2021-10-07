#pragma once
#include "AnrealEngine\Script.h"

class CTestScript :
	public CScript
{
public:
	virtual int update() {
		return 0;
	};
	virtual int lateupdate() { return 0; }

public:
	CLONE(CTestScript);

public:
	CTestScript();
	virtual ~CTestScript();
};

