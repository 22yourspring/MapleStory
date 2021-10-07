#include "TimeMgr.h"
#include "Core.h"
#include "SceneMgr.h"

CTimeMgr::CTimeMgr()
	: m_llFrequency{}
	, m_llCurCount{}
	, m_llOldCount{}
	, m_dt(0.)
{
}

CTimeMgr::~CTimeMgr()
{
}


void CTimeMgr::init()
{
	QueryPerformanceFrequency(&m_llFrequency); // 1초에 타이머가 셀수있는 진동수	
	QueryPerformanceCounter(&m_llOldCount);	
}

int CTimeMgr::update()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_dt = (double)(m_llCurCount.QuadPart - m_llOldCount.QuadPart) / (double)m_llFrequency.QuadPart;
	
	if (m_fIsLoading)
	{
		m_fIsLoading = false;
	
		m_dt = 0.f;
	}

	m_llOldCount = m_llCurCount;


	if (CSceneMgr::m_bFadeIn == FADE::FADEIN)
	{
		m_fAccTime += (float)(m_dt * 4.f);

		if (m_fAccTime >= 1.f)
		{
			CSceneMgr::m_bFadeIn = FADE::NOTHING;
			m_fAccTime = 1.f;
		}

		g_global.fDT = (float)m_dt;
		g_global.fAccTime = m_fAccTime;
	}
	else if (CSceneMgr::m_bFadeIn == FADE::FADEOUT)
	{
		m_fAccTime -= (float)(m_dt * 4.f);

		if (m_fAccTime <= -1.f)
		{
			CSceneMgr::m_bFadeIn = FADE::NOTHING;
			m_fAccTime = -1.f;
		}

		g_global.fDT = (float)m_dt;
		g_global.fAccTime = m_fAccTime;
	}
	else
	{
		m_fAccTime = -1.f;
		g_global.fDT = (float)m_dt;
		g_global.fAccTime = m_fAccTime;
	}




	// 1초에 1번 계산
	static double accTime = 0.f;
	accTime += m_dt;

	if (accTime > 1.f)
	{
		accTime = 0.;
		m_iFPS = (UINT)(1. / m_dt);
	}


	return 0;
}

void CTimeMgr::render(HDC _dc)
{	
	
}
