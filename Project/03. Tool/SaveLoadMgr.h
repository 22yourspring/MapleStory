#pragma once

class CLayer;
class CGameObject;
class CRes;

class CSaveLoadMgr
{
public:
	static void SaveScene(const wstring& _strScenePath);	   	 
	static void LoadScene(const wstring& _strScenePath);	

private:
	static void SaveLayer(CLayer* _pLayer, FILE* _pFile);
	static void SaveGameObject(CGameObject* _pObj, FILE* _pFile);

	static void LoadLayer(CLayer* _pLayer, FILE* _pFile);
	static CGameObject* LoadGameObject(FILE* _pFile);

	static void SaveResource(FILE* _pFile);
	static void LoadResource(FILE* _pFile);

	static void SavePrefab(const map<wstring, CRes*>& _mapPrefab, FILE* _pFile);
	static void LoadPrefab(int _iCount, FILE* _pFile);

public:
	CSaveLoadMgr() {};
	~CSaveLoadMgr() {};
};

