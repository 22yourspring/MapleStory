#include "Script.h"

#include "Transform.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Layer.h"

#include "TimeMgr.h"
#include "EventMgr.h"


int CScript::DeathCount = 0;

CScript::CScript()
{
	SetComponentType(COMPONENT_TYPE::SCRIPT);
}

CScript::CScript(long long _PlayerHP, long long _PlayerMP, long long _Player_Value_HP, long long _Player_Value_MP)
	: Object_HP(_PlayerHP)
	, Object_MP(_PlayerMP)
	, Object_Value_HP(_Player_Value_HP)
	, Object_Value_MP(_Player_Value_MP)
{
	SetComponentType(COMPONENT_TYPE::SCRIPT);
}

CScript::CScript(long long _Object_HP, long long _Object_MP, long long _Object_Value_HP, long long _Object_Value_MP, UINT _PresentSKILL, bool _m_bAggro, bool _RagingCollision, bool _InstinctCollision, bool _ShoutCollision, bool _DeathFaultCollision, bool _AlphaCollision, bool _BetaCollision, bool _EclipseCollision, int _RagingEffectCount, float _RagingCount, int _InstinctEffectCount, float _InstinctCount, int _DeathFaultEffectCount, float _DeathFaultCount, int _ShoutEffectCount, float _ShoutCount, int _AlphaEffectCount, float _AlphaCount, int _BetaEffectCount, float _BetaCount, int _EclipseEffectCount, float _EclipseCount, float _AlphaTime, float _BetaTime, float _EclipseTime)
	: Object_HP(_Object_HP)
	, Object_MP(_Object_MP)
	, Object_Value_HP(_Object_Value_HP)
	, Object_Value_MP(_Object_Value_MP)
	, PresentSKILL(_PresentSKILL)
	, m_bAggro(false)
	, isBePushed(false)
	, RagingCollision(_RagingCollision)
	, InstinctCollision(_InstinctCollision)
	, ShoutCollision(_ShoutCollision)
	, DeathFaultCollision(_DeathFaultCollision)
	, AlphaCollision(_AlphaCollision)
	, BetaCollision(_BetaCollision)
	, EclipseCollision(_EclipseCollision)
	, RagingEffectCount(_RagingEffectCount)
	, RagingCount(_RagingCount)
	, InstinctEffectCount(_InstinctEffectCount)
	, InstinctCount(_InstinctCount)
	, DeathFaultEffectCount(_DeathFaultEffectCount)
	, DeathFaultCount(_DeathFaultCount)
	, ShoutEffectCount(_ShoutEffectCount)
	, ShoutCount(_ShoutCount)
	, AlphaEffectCount(_AlphaEffectCount)
	, AlphaCount(_AlphaCount)
	, BetaEffectCount(_BetaEffectCount)
	, BetaCount(_BetaCount)
	, EclipseEffectCount(_EclipseEffectCount)
	, EclipseCount(_EclipseCount)
	, AlphaTime(_AlphaTime)
	, BetaTime(_BetaTime)
	, EclipseTime(_EclipseTime)

{
	SetComponentType(COMPONENT_TYPE::SCRIPT);
}

CScript::~CScript()
{
}

void CScript::Instantiate(Ptr<CPrefab> _pPrefab, const wstring & _strLayerName, const Vec3 & _vPos)
{
	CGameObject* pCopy = _pPrefab->Instantiate();
	pCopy->Transform()->SetLocalPos(_vPos);	
	int iIdx = CSceneMgr::GetInst()->GetLayer(_strLayerName)->GetIdx();

	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::CREATE_OBJECT, (INT_PTR)pCopy, iIdx });
}

void CScript::DestroyObject(CGameObject * _pTargetObj)
{
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJECT, (INT_PTR)_pTargetObj, 0 });	
}

void CScript::DestroyObject()
{
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DELETE_OBJECT, (INT_PTR)Object(), 0 });
}

void CScript::DisconnectParent(CGameObject * _pTargetObj)
{
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DISCONNECT_PARENT, (INT_PTR)_pTargetObj, 0 });
}

void CScript::DisconnectParent()
{
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::DISCONNECT_PARENT, (INT_PTR)Object(), 0 });
}

void CScript::AddChild(CGameObject * _pChild)
{
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::ADD_CHILD, (INT_PTR)Object(), (INT_PTR)_pChild });
}

void CScript::AddChild(CGameObject * _pParent, CGameObject* _pChild)
{
	CEventMgr::GetInst()->AddEvent(tEvent{ EVENT_TYPE::ADD_CHILD, (INT_PTR)_pParent, (INT_PTR)_pChild});
}





void CScript::SubHP(long long _Damage)
{
	if (_Damage < 0)
	{
		_Damage *= -1; /*수정된 부분 여기*/

	}
	if (Object_HP == 0)
	{
		return;
	}

	if (Object_HP - _Damage <= 0)
	{
		Object_HP = 0;
		return;
	}
	else
	{
		Object_HP -= _Damage;
		return;
	}
}

void CScript::SubMP(long long _Damage)
{
	if (Object_MP - _Damage < 0)
	{
		return;
	}
	else
	{
		Object_MP -= _Damage;
		return;
	}
}

void CScript::SubDeathCount()
{
	if (CScript::DeathCount > 0)
	{
		CScript::DeathCount -= 1;
	}
}

void CScript::AddDeathCount()
{
	CScript::DeathCount += 1;
}

void CScript::Regeneration(CGameObject* _Object)
{
	if (CScript::DeathCount <= 0)
	{
	//	assert(NULL);
	}

	if (CScript::DeathCount != 0)
	{
		RegenarationTime += (DT / CScript::DeathCount);
	}
	else
		RegenarationTime += DT;		// 현재 죽은 마리수로 DT를 나눠야 함  // AND 리제네레이션 확인 

	if (RegenarationTime < 10.f)
	{
		return;
	}

	RegenarationTime = 0.f;

	SubDeathCount();

	_Object->Regeneration();
}

void CScript::Regeneration(CGameObject * _Object, bool _AlterEgo)
{
	if (_AlterEgo)
	{
		SubDeathCount();

		_Object->Regeneration();
	}
}

void CScript::KillObject(CGameObject * _Object)
{
	_Object->Kill();

	AddDeathCount();
}

bool CScript::Confirm_Life(CGameObject * _Object)
{
	return _Object->Confirm_Life();
}



void CScript::AddHP(long long _Healing)
{
	if (Object_HP + _Healing > Object_Value_HP)
	{
		Object_HP = Object_Value_HP;
		return;
	}
	else
	{
		Object_HP += _Healing;
		return;
	}
}

void CScript::AddMP(long long _Healing)
{
	if (Object_MP + _Healing > Object_Value_MP)
	{
		Object_MP = Object_Value_MP;
		return;
	}
	else
	{
		Object_MP += _Healing;
		return;
	}
}