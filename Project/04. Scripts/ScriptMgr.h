#pragma once

#include <vector>
#include <string>

enum class SCRIPT_TYPE
{
	ACTIVEEXPLOSIONSCRIPT,
	ALTEREGOSCRIPT,
	AREASCRIPT,
	ATTACKSCRIPT,
	BESHOTEFFECTSCRIPT,
	BOOSTERSCRIPT,
	BULLETSCRIPT,
	CAMCOLLSCRIPT,
	CAMERASCRIPT,
	COLLIDERSCRIPT,
	COMBODEATHFAULTSCRIPT,
	COMBOMAINSCRIPT,
	COMBOSUBASCRIPT,
	COMBOSUBBSCRIPT,
	COMBOSUBCSCRIPT,
	COMBOSUBDSCRIPT,
	COMBOSUBESCRIPT,
	CRANESCRIPT,
	EFFECTSCRIPT,
	FALLENSCRIPT,
	FINGERSCRIPT,
	GATEKEEPERSCRIPT,
	GIGANTICVIKINGSCRIPT,
	GRAVITYSCRIPT,
	INSTALLATIONTYPESCRIPT,
	INSTANTDEATHAREASCRIPT,
	INSTINCTACTIVESCRIPT,
	INSTINCTSCRIPT,
	ITEMSCRIPT,
	LADDERSCRIPT,
	LASERBODYSCRIPT,
	LASERPOINTSCRIPT,
	LASERSCRIPT,
	MAPLEWARRIORSCRIPT,
	MISTSCRIPT,
	MOBGRAVITYSCRIPT,
	MOUSESCRIPT,
	OBSTRUCTSCRIPT,
	PAPULATUSSCRIPT,
	PARTICLESCRIPT,
	PHANTOMWATCHSCRIPT,
	PLAYERCOLSCRIPT,
	PLAYERSCRIPT,
	QUESTMGR,
	RAGINGBLOWSCRIPT,
	ROPESCRIPT,
	SCALESCRIPT,
	SHADOWALPHASCRIPT,
	SHADOWBETASCRIPT,
	SHOUTSCRIPT,
	SOULECLIPSESCRIPT,
	TESTSCRIPT,
	TIMEBOMBSCRIPT,
	UICAMSCRIPT,
	UISCRIPT,
	WIRESCRIPT,
	END,
};

using namespace std;

class CScript;

class CScriptMgr
{
public:
	static void GetScriptInfo(vector<wstring>& _vec);
	static CScript * GetScript(const wstring& _strScriptName);
	static const wchar_t * GetScriptName(CScript * _pScript);
};