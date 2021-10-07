#include "Core.h"

#include "Device.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "ShaderMgr.h"
#include "SceneMgr.h"
#include "RenderMgr.h"
#include "EventMgr.h"

tGlobalValue     g_global = {};


CCore::CCore()
{
}

CCore::~CCore()
{		
}

void CCore::init(HWND _hWnd, const tResolution & _res, bool _bWindow)
{
	passhWnd = _hWnd;

	CDevice::GetInst()->init(_hWnd, _res, _bWindow);

	CPathMgr::init();
	CKeyMgr::GetInst()->init();
	CTimeMgr::GetInst()->init();
	CRenderMgr::GetInst()->init();
	CShaderMgr::GetInst()->init();
	CResMgr::GetInst()->init();				
	CSceneMgr::GetInst()->init();
	

	// 상수버퍼 만들기
	CDevice::GetInst()->CreateCBuffer(L"Transform", sizeof(tTransformMatrix), 0);
	CDevice::GetInst()->CreateCBuffer(L"ShaderParam", sizeof(tShaderParam), 1);
	CDevice::GetInst()->CreateCBuffer(L"Anim2DInfo", sizeof(tAnim2DInfo), 2);
	CDevice::GetInst()->CreateCBuffer(L"Global", sizeof(tGlobalValue), 4);
}

int CCore::progress()
{
	update();
	render();
	return 0;
}

int CCore::update()
{	
	CKeyMgr::GetInst()->update();
	CTimeMgr::GetInst()->update();
	CSceneMgr::GetInst()->update();

	CEventMgr::GetInst()->update();

	return 0;
}

void CCore::render()
{
	float arrCol[4]{0.f, 0.f, 0.f, 1.f};
	CDevice::GetInst()->ClearTarget(arrCol);
	
	CSceneMgr::GetInst()->render();
	
	// 윈도우로 출력
	CDevice::GetInst()->Present();
}