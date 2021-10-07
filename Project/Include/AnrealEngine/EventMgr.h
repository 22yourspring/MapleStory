#pragma once

#include "global.h"

class CGameObject;
class CEventMgr
{
private:
	vector<tEvent>		 m_vecEvent;
	vector<tEvent>		 m_vecEventCopy;
	vector<CGameObject*> m_vecDelObj;


public:
	int update();

public:
	void AddEvent(const tEvent& _event){m_vecEvent.push_back(_event);}
	vector<tEvent>& GetEventList() { return m_vecEventCopy; }

private:
	void Execute(const tEvent& _event);

	SINGLE(CEventMgr);
};

