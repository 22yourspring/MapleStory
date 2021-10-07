#include "stdafx.h"
#include "ScriptMgr.h"

#include "Scripts\ActiveExplosionScript.h"
#include "Scripts\AlterEgoScript.h"
#include "Scripts\AreaScript.h"
#include "Scripts\AttackScript.h"
#include "Scripts\BeShotEffectScript.h"
#include "Scripts\BoosterScript.h"
#include "Scripts\BulletScript.h"
#include "Scripts\CamCollScript.h"
#include "Scripts\CameraScript.h"
#include "Scripts\ColliderScript.h"
#include "Scripts\ComboDeathfaultScript.h"
#include "Scripts\ComboMainScript.h"
#include "Scripts\ComboSubAScript.h"
#include "Scripts\ComboSubBScript.h"
#include "Scripts\ComboSubCScript.h"
#include "Scripts\ComboSubDScript.h"
#include "Scripts\ComboSubEScript.h"
#include "Scripts\CraneScript.h"
#include "Scripts\EffectScript.h"
#include "Scripts\FallenScript.h"
#include "Scripts\FingerScript.h"
#include "Scripts\GatekeeperScript.h"
#include "Scripts\GiganticvikingScript.h"
#include "Scripts\GravityScript.h"
#include "Scripts\InstallationTypeScript.h"
#include "Scripts\InstantDeathAreaScript.h"
#include "Scripts\InstinctActiveScript.h"
#include "Scripts\InstinctScript.h"
#include "Scripts\ItemScript.h"
#include "Scripts\LadderScript.h"
#include "Scripts\LaserBodyScript.h"
#include "Scripts\LaserPointScript.h"
#include "Scripts\LaserScript.h"
#include "Scripts\MapleWarriorScript.h"
#include "Scripts\MistScript.h"
#include "Scripts\MobGravityScript.h"
#include "Scripts\MouseScript.h"
#include "Scripts\ObstructScript.h"
#include "Scripts\PapulatusScript.h"
#include "Scripts\ParticleScript.h"
#include "Scripts\PhantomwatchScript.h"
#include "Scripts\PlayerColScript.h"
#include "Scripts\PlayerScript.h"
#include "Scripts\QuestMgr.h"
#include "Scripts\RagingBlowScript.h"
#include "Scripts\RopeScript.h"
#include "Scripts\ScaleScript.h"
#include "Scripts\ShadowAlphaScript.h"
#include "Scripts\ShadowBetaScript.h"
#include "Scripts\ShoutScript.h"
#include "Scripts\SoulEclipseScript.h"
#include "Scripts\TestScript.h"
#include "Scripts\TimeBombScript.h"
#include "Scripts\UICamScript.h"
#include "Scripts\UIScript.h"
#include "Scripts\WireScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"ActiveExplosionScript");
	_vec.push_back(L"AlterEgoScript");
	_vec.push_back(L"AreaScript");
	_vec.push_back(L"AttackScript");
	_vec.push_back(L"BeShotEffectScript");
	_vec.push_back(L"BoosterScript");
	_vec.push_back(L"BulletScript");
	_vec.push_back(L"CamCollScript");
	_vec.push_back(L"CameraScript");
	_vec.push_back(L"ColliderScript");
	_vec.push_back(L"ComboDeathfaultScript");
	_vec.push_back(L"ComboMainScript");
	_vec.push_back(L"ComboSubAScript");
	_vec.push_back(L"ComboSubBScript");
	_vec.push_back(L"ComboSubCScript");
	_vec.push_back(L"ComboSubDScript");
	_vec.push_back(L"ComboSubEScript");
	_vec.push_back(L"CraneScript");
	_vec.push_back(L"EffectScript");
	_vec.push_back(L"FallenScript");
	_vec.push_back(L"FingerScript");
	_vec.push_back(L"GatekeeperScript");
	_vec.push_back(L"GiganticvikingScript");
	_vec.push_back(L"GravityScript");
	_vec.push_back(L"InstallationTypeScript");
	_vec.push_back(L"InstantDeathAreaScript");
	_vec.push_back(L"InstinctActiveScript");
	_vec.push_back(L"InstinctScript");
	_vec.push_back(L"ItemScript");
	_vec.push_back(L"LadderScript");
	_vec.push_back(L"LaserBodyScript");
	_vec.push_back(L"LaserPointScript");
	_vec.push_back(L"LaserScript");
	_vec.push_back(L"MapleWarriorScript");
	_vec.push_back(L"MistScript");
	_vec.push_back(L"MobGravityScript");
	_vec.push_back(L"MouseScript");
	_vec.push_back(L"ObstructScript");
	_vec.push_back(L"PapulatusScript");
	_vec.push_back(L"ParticleScript");
	_vec.push_back(L"PhantomwatchScript");
	_vec.push_back(L"PlayerColScript");
	_vec.push_back(L"PlayerScript");
	_vec.push_back(L"QuestMgr");
	_vec.push_back(L"RagingBlowScript");
	_vec.push_back(L"RopeScript");
	_vec.push_back(L"ScaleScript");
	_vec.push_back(L"ShadowAlphaScript");
	_vec.push_back(L"ShadowBetaScript");
	_vec.push_back(L"ShoutScript");
	_vec.push_back(L"SoulEclipseScript");
	_vec.push_back(L"TestScript");
	_vec.push_back(L"TimeBombScript");
	_vec.push_back(L"UICamScript");
	_vec.push_back(L"UIScript");
	_vec.push_back(L"WireScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"ActiveExplosionScript" == _strScriptName)
		return new CActiveExplosionScript;
	if (L"AlterEgoScript" == _strScriptName)
		return new CAlterEgoScript;
	if (L"AreaScript" == _strScriptName)
		return new CAreaScript;
	if (L"AttackScript" == _strScriptName)
		return new CAttackScript;
	if (L"BeShotEffectScript" == _strScriptName)
		return new CBeShotEffectScript;
	if (L"BoosterScript" == _strScriptName)
		return new CBoosterScript;
	if (L"BulletScript" == _strScriptName)
		return new CBulletScript;
	if (L"CamCollScript" == _strScriptName)
		return new CCamCollScript;
	if (L"CameraScript" == _strScriptName)
		return new CCameraScript;
	if (L"ColliderScript" == _strScriptName)
		return new CColliderScript;
	if (L"ComboDeathfaultScript" == _strScriptName)
		return new CComboDeathfaultScript;
	if (L"ComboMainScript" == _strScriptName)
		return new CComboMainScript;
	if (L"ComboSubAScript" == _strScriptName)
		return new CComboSubAScript;
	if (L"ComboSubBScript" == _strScriptName)
		return new CComboSubBScript;
	if (L"ComboSubCScript" == _strScriptName)
		return new CComboSubCScript;
	if (L"ComboSubDScript" == _strScriptName)
		return new CComboSubDScript;
	if (L"ComboSubEScript" == _strScriptName)
		return new CComboSubEScript;
	if (L"CraneScript" == _strScriptName)
		return new CCraneScript;
	if (L"EffectScript" == _strScriptName)
		return new CEffectScript;
	if (L"FallenScript" == _strScriptName)
		return new CFallenScript;
	if (L"FingerScript" == _strScriptName)
		return new CFingerScript;
	if (L"GatekeeperScript" == _strScriptName)
		return new CGatekeeperScript;
	if (L"GiganticvikingScript" == _strScriptName)
		return new CGiganticvikingScript;
	if (L"GravityScript" == _strScriptName)
		return new CGravityScript;
	if (L"InstallationTypeScript" == _strScriptName)
		return new CInstallationTypeScript;
	if (L"InstantDeathAreaScript" == _strScriptName)
		return new CInstantDeathAreaScript;
	if (L"InstinctActiveScript" == _strScriptName)
		return new CInstinctActiveScript;
	if (L"InstinctScript" == _strScriptName)
		return new CInstinctScript;
	if (L"ItemScript" == _strScriptName)
		return new CItemScript;
	if (L"LadderScript" == _strScriptName)
		return new CLadderScript;
	if (L"LaserBodyScript" == _strScriptName)
		return new CLaserBodyScript;
	if (L"LaserPointScript" == _strScriptName)
		return new CLaserPointScript;
	if (L"LaserScript" == _strScriptName)
		return new CLaserScript;
	if (L"MapleWarriorScript" == _strScriptName)
		return new CMapleWarriorScript;
	if (L"MistScript" == _strScriptName)
		return new CMistScript;
	if (L"MobGravityScript" == _strScriptName)
		return new CMobGravityScript;
	if (L"MouseScript" == _strScriptName)
		return new CMouseScript;
	if (L"ObstructScript" == _strScriptName)
		return new CObstructScript;
	if (L"PapulatusScript" == _strScriptName)
		return new CPapulatusScript;
	if (L"ParticleScript" == _strScriptName)
		return new CParticleScript;
	if (L"PhantomwatchScript" == _strScriptName)
		return new CPhantomwatchScript;
	if (L"PlayerColScript" == _strScriptName)
		return new CPlayerColScript;
	if (L"PlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"QuestMgr" == _strScriptName)
		return new CQuestMgr;
	if (L"RagingBlowScript" == _strScriptName)
		return new CRagingBlowScript;
	if (L"RopeScript" == _strScriptName)
		return new CRopeScript;
	if (L"ScaleScript" == _strScriptName)
		return new CScaleScript;
	if (L"ShadowAlphaScript" == _strScriptName)
		return new CShadowAlphaScript;
	if (L"ShadowBetaScript" == _strScriptName)
		return new CShadowBetaScript;
	if (L"ShoutScript" == _strScriptName)
		return new CShoutScript;
	if (L"SoulEclipseScript" == _strScriptName)
		return new CSoulEclipseScript;
	if (L"TestScript" == _strScriptName)
		return new CTestScript;
	if (L"TimeBombScript" == _strScriptName)
		return new CTimeBombScript;
	if (L"UICamScript" == _strScriptName)
		return new CUICamScript;
	if (L"UIScript" == _strScriptName)
		return new CUIScript;
	if (L"WireScript" == _strScriptName)
		return new CWireScript;
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ACTIVEEXPLOSIONSCRIPT:
		return L"ActiveExplosionScript";
		break;

	case SCRIPT_TYPE::ALTEREGOSCRIPT:
		return L"AlterEgoScript";
		break;

	case SCRIPT_TYPE::AREASCRIPT:
		return L"AreaScript";
		break;

	case SCRIPT_TYPE::ATTACKSCRIPT:
		return L"AttackScript";
		break;

	case SCRIPT_TYPE::BESHOTEFFECTSCRIPT:
		return L"BeShotEffectScript";
		break;

	case SCRIPT_TYPE::BOOSTERSCRIPT:
		return L"BoosterScript";
		break;

	case SCRIPT_TYPE::BULLETSCRIPT:
		return L"BulletScript";
		break;

	case SCRIPT_TYPE::CAMCOLLSCRIPT:
		return L"CamCollScript";
		break;

	case SCRIPT_TYPE::CAMERASCRIPT:
		return L"CameraScript";
		break;

	case SCRIPT_TYPE::COLLIDERSCRIPT:
		return L"ColliderScript";
		break;

	case SCRIPT_TYPE::COMBODEATHFAULTSCRIPT:
		return L"ComboDeathfaultScript";
		break;

	case SCRIPT_TYPE::COMBOMAINSCRIPT:
		return L"ComboMainScript";
		break;

	case SCRIPT_TYPE::COMBOSUBASCRIPT:
		return L"ComboSubAScript";
		break;

	case SCRIPT_TYPE::COMBOSUBBSCRIPT:
		return L"ComboSubBScript";
		break;

	case SCRIPT_TYPE::COMBOSUBCSCRIPT:
		return L"ComboSubCScript";
		break;

	case SCRIPT_TYPE::COMBOSUBDSCRIPT:
		return L"ComboSubDScript";
		break;

	case SCRIPT_TYPE::COMBOSUBESCRIPT:
		return L"ComboSubEScript";
		break;

	case SCRIPT_TYPE::CRANESCRIPT:
		return L"CraneScript";
		break;

	case SCRIPT_TYPE::EFFECTSCRIPT:
		return L"EffectScript";
		break;

	case SCRIPT_TYPE::FALLENSCRIPT:
		return L"FallenScript";
		break;

	case SCRIPT_TYPE::FINGERSCRIPT:
		return L"FingerScript";
		break;

	case SCRIPT_TYPE::GATEKEEPERSCRIPT:
		return L"GatekeeperScript";
		break;

	case SCRIPT_TYPE::GIGANTICVIKINGSCRIPT:
		return L"GiganticvikingScript";
		break;

	case SCRIPT_TYPE::GRAVITYSCRIPT:
		return L"GravityScript";
		break;

	case SCRIPT_TYPE::INSTALLATIONTYPESCRIPT:
		return L"InstallationTypeScript";
		break;

	case SCRIPT_TYPE::INSTANTDEATHAREASCRIPT:
		return L"InstantDeathAreaScript";
		break;

	case SCRIPT_TYPE::INSTINCTACTIVESCRIPT:
		return L"InstinctActiveScript";
		break;

	case SCRIPT_TYPE::INSTINCTSCRIPT:
		return L"InstinctScript";
		break;

	case SCRIPT_TYPE::ITEMSCRIPT:
		return L"ItemScript";
		break;

	case SCRIPT_TYPE::LADDERSCRIPT:
		return L"LadderScript";
		break;

	case SCRIPT_TYPE::LASERBODYSCRIPT:
		return L"LaserBodyScript";
		break;

	case SCRIPT_TYPE::LASERPOINTSCRIPT:
		return L"LaserPointScript";
		break;

	case SCRIPT_TYPE::LASERSCRIPT:
		return L"LaserScript";
		break;

	case SCRIPT_TYPE::MAPLEWARRIORSCRIPT:
		return L"MapleWarriorScript";
		break;

	case SCRIPT_TYPE::MISTSCRIPT:
		return L"MistScript";
		break;

	case SCRIPT_TYPE::MOBGRAVITYSCRIPT:
		return L"MobGravityScript";
		break;

	case SCRIPT_TYPE::MOUSESCRIPT:
		return L"MouseScript";
		break;

	case SCRIPT_TYPE::OBSTRUCTSCRIPT:
		return L"ObstructScript";
		break;

	case SCRIPT_TYPE::PAPULATUSSCRIPT:
		return L"PapulatusScript";
		break;

	case SCRIPT_TYPE::PARTICLESCRIPT:
		return L"ParticleScript";
		break;

	case SCRIPT_TYPE::PHANTOMWATCHSCRIPT:
		return L"PhantomwatchScript";
		break;

	case SCRIPT_TYPE::PLAYERCOLSCRIPT:
		return L"PlayerColScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"PlayerScript";
		break;

	case SCRIPT_TYPE::QUESTMGR:
		return L"QuestMgr";
		break;

	case SCRIPT_TYPE::RAGINGBLOWSCRIPT:
		return L"RagingBlowScript";
		break;

	case SCRIPT_TYPE::ROPESCRIPT:
		return L"RopeScript";
		break;

	case SCRIPT_TYPE::SCALESCRIPT:
		return L"ScaleScript";
		break;

	case SCRIPT_TYPE::SHADOWALPHASCRIPT:
		return L"ShadowAlphaScript";
		break;

	case SCRIPT_TYPE::SHADOWBETASCRIPT:
		return L"ShadowBetaScript";
		break;

	case SCRIPT_TYPE::SHOUTSCRIPT:
		return L"ShoutScript";
		break;

	case SCRIPT_TYPE::SOULECLIPSESCRIPT:
		return L"SoulEclipseScript";
		break;

	case SCRIPT_TYPE::TESTSCRIPT:
		return L"TestScript";
		break;

	case SCRIPT_TYPE::TIMEBOMBSCRIPT:
		return L"TimeBombScript";
		break;

	case SCRIPT_TYPE::UICAMSCRIPT:
		return L"UICamScript";
		break;

	case SCRIPT_TYPE::UISCRIPT:
		return L"UIScript";
		break;

	case SCRIPT_TYPE::WIRESCRIPT:
		return L"WireScript";
		break;

	}
	return nullptr;
}