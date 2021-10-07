#pragma once

#include "global.h"

class CShader;
class CCamera;

class CShaderMgr
{
	SINGLE(CShaderMgr);
private:
	map<wstring, CShader*>	m_mapShader;

public:
	void init();
	void AddShader(const wstring& _strKey, CShader* _pShader);
	CShader* FindShader(const wstring& _strKey);
};

