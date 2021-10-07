#include "pch.h"

#include "PathMgr.h"

vector<wstring> g_vecName;

int main()
{
	CPathMgr::init();
	wstring strProjPath = CPathMgr::GetProjectPath();
	wstring strCppPath = strProjPath + L"04. Scripts\\ScriptMgr.cpp";
	wstring strHeaderPath = strProjPath + L"04. Scripts\\ScriptMgr.h";

	// 1. 현재 존재하는 모든 스크립트를 알아내야함.
	wstring strScriptCode = strProjPath + L"04. Scripts\\";

	WIN32_FIND_DATA tData = {};
	HANDLE handle = FindFirstFile(wstring(strScriptCode + L"\\*.h").c_str(), &tData);

	if (INVALID_HANDLE_VALUE == handle)
		return 0;

	while (true)
	{
		if ( wcscmp(tData.cFileName, L"stdafx.h")
			&& wcscmp(tData.cFileName, L"ScriptMgr.h")
			&& wcscmp(tData.cFileName, L"targetver.h")
			&& wcscmp(tData.cFileName, L"STScript.h"))
		{
			g_vecName.push_back(wstring(tData.cFileName).substr(0, wcslen(tData.cFileName) - 2));
		}		

		if (!FindNextFile(handle, &tData))
			break;
	}

	FindClose(handle);

	FILE* pFile = NULL;

	//=================
	// ScriptMgr h 작성
	//=================
	_wfopen_s(&pFile, strHeaderPath.c_str(), L"w");
	fwprintf_s(pFile, L"#pragma once\n\n");
	fwprintf_s(pFile, L"#include <vector>\n");
	fwprintf_s(pFile, L"#include <string>\n\n");

	   	  

	fwprintf_s(pFile, L"enum class SCRIPT_TYPE\n{\n");
	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		wstring strScriptUpperName = L"";
		for (UINT j = 0; j < g_vecName[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecName[i][j]);
		}
		
		fwprintf_s(pFile, L"\t");
		fwprintf_s(pFile, strScriptUpperName.c_str());
		fwprintf_s(pFile, L",\n");
	}
	fwprintf_s(pFile, L"\tEND,\n");
	fwprintf_s(pFile, L"};\n\n");
	

	fwprintf_s(pFile, L"using namespace std;\n\n");
	fwprintf_s(pFile, L"class CScript;\n\n");

	fwprintf_s(pFile, L"class CScriptMgr\n{\n");
	fwprintf_s(pFile, L"public:\n\tstatic void GetScriptInfo(vector<wstring>& _vec);\n");
	fwprintf_s(pFile, L"\tstatic CScript * GetScript(const wstring& _strScriptName);\n");
	fwprintf_s(pFile, L"\tstatic const wchar_t * GetScriptName(CScript * _pScript);\n};\n");	


	fclose(pFile);

	//====================
	// ScriptMgr cpp 작성
	//====================
	_wfopen_s(&pFile, strCppPath.c_str(), L"w");

	// 헤더 입력
	fwprintf_s(pFile, L"#include \"stdafx.h\"\n");
	fwprintf_s(pFile, L"#include \"ScriptMgr.h\"\n\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"#include \"Scripts\\");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L".h\"\n");
	}

	// 첫 번째 함수 작성
	fwprintf_s(pFile, L"\nvoid CScriptMgr::GetScriptInfo(vector<wstring>& _vec)\n{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"\t_vec.push_back(L\"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\");\n");
	}
	fwprintf_s(pFile, L"}\n\n");


	// 두번째 함수 작성
	/*fwprintf_s(pFile, L"CScript * CScriptMgr::GetScript(DWORD _dwTypeId)\n{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"\tif (typeid(C");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L").hash_code() == _dwTypeId)\n");
		fwprintf_s(pFile, L"\t\treturn new C");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L";\n");
	}
	fwprintf_s(pFile, L"\treturn nullptr;\n}\n\n");*/

	// 세번째 함수 작성
	fwprintf_s(pFile, L"CScript * CScriptMgr::GetScript(const wstring& _strScriptName)\n{\n");

	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"\tif (L\"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\" == _strScriptName)\n");
		fwprintf_s(pFile, L"\t\treturn new C");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L";\n");
	}
	fwprintf_s(pFile, L"\treturn nullptr;\n}\n\n");


	// 네번째 함수
	fwprintf_s(pFile, L"const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)\n{\n");
	fwprintf_s(pFile, L"\tswitch ((SCRIPT_TYPE)_pScript->GetScriptType())\n\t{\n");
	for (UINT i = 0; i < g_vecName.size(); ++i)
	{
		fwprintf_s(pFile, L"\tcase SCRIPT_TYPE::");
		
		wstring strScriptUpperName = L"";
		for (UINT j = 0; j < g_vecName[i].size(); ++j)
		{
			strScriptUpperName += toupper(g_vecName[i][j]);
		}

		fwprintf_s(pFile, strScriptUpperName.c_str());

		fwprintf_s(pFile, L":\n\t\treturn ");
		fwprintf_s(pFile, L"L\"");
		fwprintf_s(pFile, g_vecName[i].c_str());
		fwprintf_s(pFile, L"\";\n\t\tbreak;\n\n");				
	}

	fwprintf_s(pFile, L"\t}\n\treturn nullptr;\n}");
	
	fclose(pFile);

	return 0;
}