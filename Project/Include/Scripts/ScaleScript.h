#pragma once
#include "Script.h"

class CScaleScript :
	public CScript
{
public:
	virtual int update();

public:
	CLONE(CScaleScript);

public:
	CScaleScript();
	virtual ~CScaleScript();
};

