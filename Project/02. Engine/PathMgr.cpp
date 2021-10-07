#include "PathMgr.h"
#include "atlstr.h"

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
//
//wstring CPathMgr::GetRelativePath(wstring& _str)	//62
//{
//	/*LPVOID lpOutBuffer;
//	wchar_t** t = (wchar_t**)_str;
//	wstring str = wstring(*t);*/
//
//	int findNumStart = _str.find(L"Texture");	//62
//	int findNumEnd = _str.length();	//62
//
//	wstring returnvalue;
//
//
//	returnvalue = _str.substr(findNumStart, findNumEnd - findNumStart)+= L"\\";
//
//
//	return returnvalue;
//}

wstring CPathMgr::GetFramePath(wstring & _str)
{
	int findNumStart = (int)_str.find(L"Texture");	//62
	int findNumEnd = (int)_str.length();	//62

	wstring returnvalue;


	returnvalue = _str.substr(findNumStart, findNumEnd - findNumStart) += L"\\";


	return returnvalue;
}

wstring CPathMgr::GetAtlasPath(wstring & _str)
{
	int findNumStart = (int)_str.find(L"Texture");	//62
	int findNumEnd = (int)_str.length();	//62

	wstring returnvalue;


	returnvalue = _str.substr(findNumStart, findNumEnd - findNumStart);


	return returnvalue;
}

wstring CPathMgr::GetAnimFrameKey(wstring & _str)
{
	wstring key = _str;

	int findNumKey = (int)_str.find_last_of(L"\\");
	int findNumEnd = (int)_str.length();

	int keySize = findNumEnd - findNumKey;

	wstring tempkey = _str.substr(0, findNumKey);

	findNumKey = (int)tempkey.find_last_of(L"\\")+1;

	return tempkey.substr(findNumKey);

	/*CString exChange = key.c_str();


	key = exChange.Right(exChange.GetLength() - exChange.ReverseFind('\\'));*/


	//wstring returnKey;

	//returnKey = _str.substr(keySize, findNumEnd - keySize - 2 );

	/*return _str.substr(keySize, findNumEnd);

	return key;*/
}

wstring CPathMgr::GetAnimAtlasKey(wstring & _str)
{

	wstring key = _str;

	int findNumKey = (int)_str.find_last_of(L"\\");
	//int findNumEnd = _str.length();


	wstring tempkey = _str.substr(0, findNumKey);

	findNumKey = (int)_str.find_last_of(L".");
	//keySize = - findNumKey;

	tempkey = _str.substr(0, findNumKey);

	findNumKey = (int)tempkey.find_last_of(L"\\");

	tempkey= tempkey.substr(findNumKey + 1);


	return tempkey;

	//wstring key = _str;

	//int findNumKey = _str.find_last_of(L"\\");
	//int findNumEnd = _str.length();

	//int keySize = findNumEnd - findNumKey - 4;
	////png 확장자 삭제 4

	//wstring tempkey = _str.substr(0, findNumKey);

	//findNumKey = tempkey.find_last_of(L"\\") + 1;

	//return tempkey.substr(findNumKey);
}
