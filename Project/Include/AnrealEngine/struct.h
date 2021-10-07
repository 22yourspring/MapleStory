#pragma once

#include "define.h"

struct tResolution
{
	float x, y;
};

// 정점 구조체
struct VTX
{
	Vec3	vPos;
	Vec4	vColor;	
	Vec2    vUV;
};

// 인덱스 구조체
struct IDX32
{
	UINT _1, _2, _3;

	IDX32()
		: _1(0)
		, _2(0)
		, _3(0)
	{}

	IDX32(UINT __1, UINT __2, UINT __3)
		: _1(__1)
		, _2(__2)
		, _3(__3)
	{}
};

struct tEvent
{
	EVENT_TYPE eType;
	INT_PTR	   wParam;
	INT_PTR	   lParam;
};

// 상수버퍼 구조체
struct tTransformMatrix
{
	Matrix	matWorld;
	Matrix	matView;
	Matrix	matProj;
	Matrix	matWV;
	Matrix	matWVP;
};

struct tShaderParam
{
	int		arrInt[4];
	float	arrFloat[4];
	Vec2	arrVec2[4];
	Vec4	arrVec4[4];
};

struct tAnim2DInfo
{
	Vec2 vLT;
	Vec2 vSize;
	int g_bAnim2dUse;
	int g_iPadding[3];
};

struct tGlobalValue
{
	float fDT;
	float fAccTime;
	float fWidth;
	float fHeight;
	int   iLightCount;
	int   iPadding[3];
};