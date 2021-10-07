#pragma once

#define _XM_NO_INTRINSICS_

#include <d3d11.h>
#include <d3dcompiler.h>

#pragma comment(lib, "d3d11")
#pragma comment(lib, "d3dcompiler")

#include <list>
#include <vector>
#include <map>

using std::list;
using std::vector;
using std::map;

#include <array>
#include "SimpleMath.h"

#include <string>
#include <typeinfo>

using namespace std;
using namespace DirectX;

#include "define.h"
#include "struct.h"
#include "func.h"




extern tTransformMatrix	g_transform;
extern tAnim2DInfo		g_anim2d;
extern tGlobalValue     g_global;