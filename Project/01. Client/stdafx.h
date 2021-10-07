// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"






#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

// C RunTime Header Files
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>

#include <crtdbg.h>
#include "AnrealEngine/global.h"

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


#define GAMEVIEW_X 1920//1120
#define GAMEVIEW_Y 1080//630


//FPS CONSOLE
#ifdef UNICODE
#pragma comment(linker, "/entry:wWinMainCRTStartup /subsystem:console") 
#else
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console") 
#endif