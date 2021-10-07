#pragma once

#include "global.h"

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

//	static wstring GetRelativePath(wstring& _str);

	static wstring GetFramePath(wstring& _str);
	static wstring GetAtlasPath(wstring& _str);


	static wstring GetAnimFrameKey(wstring& _str);
	static wstring GetAnimAtlasKey(wstring& _str);

private:
	CPathMgr() {};
	~CPathMgr() {};
};


