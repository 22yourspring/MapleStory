#include "pch.h"
#include "PathMgr.h"


wchar_t CPathMgr::g_szResPath[255] = L"";
wchar_t CPathMgr::g_szIncPath[255] = L"";
wchar_t CPathMgr::g_szProjPath[255] = L"";

void CPathMgr::init()
{
	// Resources 폴더까지의 경로를 구한다.
	GetCurrentDirectory(255, g_szResPath);

	// 상위폴더 경로 
	for (UINT i = (UINT)wcslen(g_szResPath) - 1; i > 0; --i)
	{
		if ('\\' == g_szResPath[i])
		{
			g_szResPath[i + 1] = 0;
			break;
		}
	}
	
	// -> bin -> Resources
	wcscat_s(g_szResPath, L"content\\");


	// Inc 폴더 경로 
	GetCurrentDirectory(255, g_szIncPath);
	
	int iCount = 0;
	for (UINT i = (UINT)wcslen(g_szIncPath) - 1; i > 0; --i)
	{
		if ('\\' == g_szIncPath[i])
		{
			g_szIncPath[i + 1] = 0;
			++iCount;
		}

		if (iCount == 2)
			break;
	}
	
	wcscat_s(g_szIncPath, L"Project\\Include\\");


	// Proj 폴더 경로 
	GetCurrentDirectory(255, g_szProjPath);

	iCount = 0;
	for (UINT i = (UINT)wcslen(g_szProjPath) - 1; i > 0; --i)
	{
		if ('\\' == g_szProjPath[i])
		{
			g_szProjPath[i + 1] = 0;
			++iCount;
		}

		if (iCount == 2)
			break;
	}

	wcscat_s(g_szProjPath, L"Project\\");
}

wchar_t * CPathMgr::GetResPath()
{	
	return g_szResPath;
}

wchar_t * CPathMgr::GetIncludePath()
{
	return g_szIncPath;
}

wchar_t * CPathMgr::GetProjectPath()
{
	return g_szProjPath;
}
