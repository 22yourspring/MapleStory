
// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.





#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원
#include <afxwin.h>
#include <afxwin.h>
#include <afxwin.h>
#include <afxwin.h>
#include <afxwin.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

#include "AnrealEngine/global.h"
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>
#include <afxcontrolbars.h>

#include <iostream>

#ifdef _DEBUG

#ifdef _ANREAL32
#pragma comment(lib, "AnrealEngine_Debug")
#pragma comment(lib, "Scripts_Debug")
#else
#pragma comment(lib, "AnrealEngine64_Debug")
#pragma comment(lib, "Scripts64_Debug")
#endif

#else 

#ifdef _ANREAL32
#pragma comment(lib, "AnrealEngine")
#pragma comment(lib, "Scripts")
#else
#pragma comment(lib, "AnrealEngine64")
#pragma comment(lib, "Scripts64")
#endif

#endif

#define GAMEVIEW_X 1280//1120
#define GAMEVIEW_Y 720//630

#define BTNVIEW_X GAMEVIEW_X
#define BTNVIEW_Y 40

#define DBGVIEW_X GAMEVIEW_X
#define DBGVIEW_Y 380

#define HIERACHYVIEW_X 270
#define HIERACHYVIEW_Y (GAMEVIEW_Y + BTNVIEW_Y + DBGVIEW_Y)

#define COMVIEW_X 330
#define COMVIEW_Y (GAMEVIEW_Y + BTNVIEW_Y + DBGVIEW_Y)

#define FRAME_X	 (GAMEVIEW_X + HIERACHYVIEW_X + COMVIEW_X + 2) // 2는 경계 1픽셀 2개
#define FRAME_Y	 (GAMEVIEW_Y + BTNVIEW_Y + DBGVIEW_Y + 2) 

#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif