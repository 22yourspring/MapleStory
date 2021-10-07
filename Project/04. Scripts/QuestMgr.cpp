#include "stdafx.h"
#include "QuestMgr.h"
#include "ScriptMgr.h"

CQuestMgr::CQuestMgr()
	: a(10)
{
	SetScriptType((UINT)SCRIPT_TYPE::QUESTMGR);
}

CQuestMgr::~CQuestMgr()
{
}
