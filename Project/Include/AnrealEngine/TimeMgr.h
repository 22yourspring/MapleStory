#pragma once
#include "global.h"

class CTimeMgr
{
	SINGLE(CTimeMgr);
private:
	LARGE_INTEGER	m_llFrequency;
	LARGE_INTEGER	m_llCurCount;
	LARGE_INTEGER	m_llOldCount;

	double			m_dt;	// 1������ �ð�
	float           m_fAccTime; // �����ð�
	UINT			m_iFPS;

public:
	float DeltaTime() { return (float)m_dt; }
	UINT FPS() { return m_iFPS; }
	bool m_fIsLoading;

	void Freezing() { m_fIsLoading = true; }

	void set_fAccTime(float _m_fAccTime) {	m_fAccTime = _m_fAccTime; }

public:
	void init();
	int update();
	void render(HDC _dc);
};

