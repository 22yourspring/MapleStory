#pragma once
#include "Res.h"
class CSound :
	public CRes
{
public:	
//	void Load(const wstring& _strFullPath, const wstring& _strName);
	
public:
	CLONE_DISABLE(CSound);

public:
	CSound();
	virtual ~CSound();
};

