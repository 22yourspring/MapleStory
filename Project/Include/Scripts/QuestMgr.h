#pragma once

#include "STScript.h"

class CQuestMgr :
	public CSTScript<CQuestMgr>
{
private:
	int a;

public:
	virtual int update()
	{
		return 0;
	}

	CLONE(CQuestMgr);


public:
	CQuestMgr();
	virtual ~CQuestMgr();
};

