#include "stdafx.h"
#include "SaveLoadMgr.h"

#include "shlwapi.h"
#pragma comment(lib,"shlwapi")

#include "AnrealEngine/SceneMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/Layer.h"
#include "AnrealEngine/GameObject.h"
#include "AnrealEngine/Component.h"
#include "AnrealEngine/func.h"
#include "AnrealEngine/Script.h"

#include "AnrealEngine/TimeMgr.h"


#include "AnrealEngine/ResMgr.h"
#include "AnrealEngine/CollisionMgr.h"
#include "HierachyView.h"
#include "ComponentView.h"

#include "Scripts/ScriptMgr.h"
#include "AnrealEngine/ResMgr.h"

#include "MainFrm.h"


void CSaveLoadMgr::SaveScene(const wstring & _strScenePath)
{
	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strScenePath.c_str(), L"wb");
	assert(pFile);
	
	// Resource ����( Prefab ����)
	SaveResource(pFile);	

	CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	assert(pCurScene);

	wstring Scenename = PathFindFileNameW(_strScenePath.c_str());

	SaveWString(Scenename, pFile);

	// Scene ������ ����
	UINT iLayerCount = pCurScene->GetLayerCount();
	fwrite(&iLayerCount, sizeof(UINT), 1, pFile);
	for (UINT i = 0; i < iLayerCount; ++i)
	{
		CLayer* pLayer = pCurScene->GetLayer(i);
		SaveLayer(pLayer, pFile);
	}
	fclose(pFile);
}

void CSaveLoadMgr::SaveLayer(CLayer * _pLayer, FILE* _pFile)
{
	// Layer Name
	SaveWString(_pLayer->GetName(), _pFile);
	
	// GameObject
	vector<CGameObject*>& vecParentObj = _pLayer->GetObjectVec();
	UINT iObjCount = (UINT)vecParentObj.size();

	fwrite(&iObjCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iObjCount; ++i)
	{
		SaveGameObject(vecParentObj[i], _pFile);		
	}
}

void CSaveLoadMgr::SaveGameObject(CGameObject * _pObj, FILE * _pFile)
{
	// GameObject �� ������Ʈ ���� ����
	_pObj->Save(_pFile);

	// GameObject �� Script ���� ����
	vector<CScript*>& vecScript = _pObj->GetScript();
	UINT iScriptCount = (UINT)vecScript.size();
	fwrite(&iScriptCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < vecScript.size(); ++i)
	{
		// ��ũ��Ʈ �̸� ����
		SaveWString(CScriptMgr::GetScriptName(vecScript[i]), _pFile);

		// ��ũ��Ʈ ������ ����
		vecScript[i]->::CComponent::Save(_pFile);
		vecScript[i]->Save(_pFile);
	}
	
	// Child Object ����
	vector<CGameObject*>& vecChild = _pObj->GetChild();
	
	UINT iChildCount = (UINT)vecChild.size();
	fwrite(&iChildCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iChildCount; ++i)
	{
		SaveGameObject(vecChild[i], _pFile);
	}
}

void CSaveLoadMgr::LoadScene(const wstring & _strScenePath)
{
	CTimeMgr::GetInst()->Freezing();
	
	CSceneMgr::m_bFadeIn = FADE::FADEOUT;
	CTimeMgr::GetInst()->set_fAccTime(1.f);

	FILE* pFile = nullptr;
	_wfopen_s(&pFile, _strScenePath.c_str(), L"rb");
	assert(pFile);

	// ���ҽ� �ε�
	LoadResource(pFile);
	
	CScene* pNewScene = new CScene(0);	

	wstring Scenename = LoadWString(pFile);

	pNewScene->SetSceneName(Scenename);

	// Scene ������ �ε�	
	UINT iLayerCount = 0;
	fread(&iLayerCount, sizeof(UINT), 1, pFile);

	for (UINT i = 0; i < iLayerCount; ++i)
	{
		CLayer* pLayer = new CLayer;
		pNewScene->AddLayer(pLayer);
		LoadLayer(pLayer, pFile);	
	}

	fclose(pFile);


	CSceneMgr::GetInst()->ChangeScene(pNewScene);
	
	((CHierachyView*)((CMainFrame*)AfxGetMainWnd())->GetHierachyView())->Reset();
	((CComponentView*)((CMainFrame*)AfxGetMainWnd())->GetComponentView())->Lastinit();
}


void CSaveLoadMgr::LoadLayer(CLayer* _pLayer, FILE * _pFile)
{	
	wstring strLayerName = LoadWString(_pFile);
	_pLayer->SetName(strLayerName);
	
	UINT iObjCount = 0;
	fread(&iObjCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iObjCount; ++i)
	{		
		CGameObject* pNewObj = LoadGameObject(_pFile);
		_pLayer->AddGameObject(pNewObj);
	}	
}

CGameObject * CSaveLoadMgr::LoadGameObject(FILE * _pFile)
{
	CGameObject* pObj = new CGameObject;

	// ������Ʈ �ε�
	pObj->Load(_pFile);
		
	// GameObject �� Script ���� �ε�	
	UINT iScriptCount = 0;
	fread(&iScriptCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iScriptCount; ++i)
	{
		// ��ũ��Ʈ �̸� �ε�
		wstring strScriptName = LoadWString(_pFile);
		CScript* pScript = CScriptMgr::GetScript(strScriptName);

		// ��ũ��Ʈ�� ������ ������ �ٽ� ����
		pScript->::CComponent::Load(_pFile);
		pScript->Load(_pFile);
		
		pObj->AddComponent(pScript);		
	}
	
	// ChildObject �ε�
	// Child 
	UINT iChildCount = 0;
	fread(&iChildCount, sizeof(UINT), 1, _pFile);

	for (UINT i = 0; i < iChildCount; ++i)
	{		 
		CGameObject* pChildObj = LoadGameObject(_pFile);		
		pObj->AddChild_Load(pChildObj);
	}

	return pObj;
}

void CSaveLoadMgr::SaveResource(FILE * _pFile)
{
	for (UINT i = 0; i < (UINT)RESOURCE_TYPE::END; ++i)
	{
		const map<wstring, CRes*>& mapRes = CResMgr::GetInst()->GetResourceMap((RESOURCE_TYPE)i);

		// Resource Ÿ��
		fwrite(&i, sizeof(UINT), 1, _pFile);

		// Resource ����
		UINT iCount = (UINT)mapRes.size();
		fwrite(&iCount, sizeof(UINT), 1, _pFile);

		if ((UINT)RESOURCE_TYPE::PREFAB == i)
		{
			SavePrefab(mapRes, _pFile);
		}
		else
		{
			for (auto& pair : mapRes)
			{
				pair.second->FileSave(_pFile);
			}
		}		
	}	

	// Resource Ÿ��
	UINT i = (UINT)RESOURCE_TYPE::END;
	fwrite(&i, sizeof(UINT), 1, _pFile);
}

void CSaveLoadMgr::LoadResource(FILE * _pFile)
{	
	while (true)
	{
		RESOURCE_TYPE eResType = RESOURCE_TYPE::END;
		fread(&eResType, sizeof(UINT), 1, _pFile);

		if (eResType == RESOURCE_TYPE::END)
			break;

		int iCount = 0;
		fread(&iCount, sizeof(int), 1, _pFile);

		switch (eResType)
		{
		case RESOURCE_TYPE::MESH:

			break;
		case RESOURCE_TYPE::TEXTURE:
			for (int i = 0; i < iCount; ++i)
			{
				CTexture::FileLoad(_pFile);
			}
			break;
		case RESOURCE_TYPE::MATERIAL:
			for (int i = 0; i < iCount; ++i)
			{
				CMaterial::FileLoad(_pFile);
			}
			break;
		case RESOURCE_TYPE::SOUND:
			for (int i = 0; i < iCount; ++i)
			{
				//CSound::FileLoad(_pFile);
			}
			break;
		case RESOURCE_TYPE::PREFAB:
			LoadPrefab(iCount, _pFile);
			break;		
		default:
			break;
		}
	}	
}

void CSaveLoadMgr::SavePrefab(const map<wstring, CRes*>& _mapPrefab, FILE * _pFile)
{
	for (auto& pair : _mapPrefab)
	{
		SaveWString(pair.second->GetName(), _pFile);		
		SaveGameObject( ((CPrefab*)pair.second)->GetOriginObj(), _pFile);
	}
}

void CSaveLoadMgr::LoadPrefab(int _iCount, FILE * _pFile)
{
	for (int i = 0; i < _iCount; ++i)
	{
		wstring strKey = LoadWString(_pFile);
		CGameObject* pObj = LoadGameObject(_pFile);

		CPrefab* pPrefab = CResMgr::GetInst()->Load<CPrefab>(strKey);
		if (nullptr == pPrefab)
		{
			CResMgr::GetInst()->AddResource<CPrefab>(strKey, new CPrefab(pObj));
		}
		else
		{
			SAFE_DELETE(pObj);
		}
	}
}
