#pragma once

class CPathMgr
{
private:
	static wchar_t g_szResPath[255];
	static wchar_t g_szIncPath[255];
	static wchar_t g_szProjPath[255];

public:
	static void init();
	static wchar_t* GetResPath();
	static wchar_t* GetIncludePath();
	static wchar_t* GetProjectPath();


private:
	CPathMgr() {};
	~CPathMgr() {};
};


