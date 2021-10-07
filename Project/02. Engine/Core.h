#pragma once

#include "global.h"

class CCore
{
	SINGLE(CCore);

public:
	void init(HWND _hWnd, const tResolution& _res, bool _bWindow = true);
	int progress();

	HWND GetWndFocus() { return  passhWnd; }

private:

	HWND passhWnd;

	int update();
	void render();
};

