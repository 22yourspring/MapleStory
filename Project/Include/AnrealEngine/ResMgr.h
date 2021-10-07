#pragma once
#include "global.h"

#include "Mesh.h"
#include "Texture.h"
#include "Material.h"
#include "Sound.h"
#include "Prefab.h"

#include "PathMgr.h"

class CResMgr
{
	SINGLE(CResMgr);
private:
	map<wstring, CRes*> m_mapRes[(UINT)RESOURCE_TYPE::END];
	vector<CRes*>		m_vecCloneRes;
	
public:
	void init();

private:
	void CreateDefaultMesh();
	void CreateDefaultMaterial();

public:
	const map<wstring, CRes*>& GetResourceMap(RESOURCE_TYPE _eType) { return m_mapRes[(UINT)_eType]; }

public:
	template<typename T>
	void AddResource(const wstring& _strKey, CRes* _pRes);

	template<typename T>
	void SubResource(const wstring& _strKey);




	void AddClone(CRes* _pCloneRes) {m_vecCloneRes.push_back(_pCloneRes);}

	template<typename T>
	T* Load(const wstring& _strKey, const wstring& _strPath = L"");
};



template<typename T>
inline void CResMgr::AddResource(const wstring & _strKey, CRes * _pRes)
{
	static const type_info& info = typeid(T);
	static const type_info& mesh = typeid(CMesh);
	static const type_info& texture = typeid(CTexture);
	static const type_info& material = typeid(CMaterial);
	static const type_info& sound = typeid(CSound);
	static const type_info& prefab = typeid(CPrefab);

	RESOURCE_TYPE eType = RESOURCE_TYPE::END;
	
	if (&info == &mesh)
	{
		eType = RESOURCE_TYPE::MESH;		
	}
	else if (&info == &texture)
	{
		eType = RESOURCE_TYPE::TEXTURE;
	}
	else if (&info == &material)
	{
		eType = RESOURCE_TYPE::MATERIAL;
	}
	else if (&info == &sound)
	{
		eType = RESOURCE_TYPE::SOUND;
	}
	else if (&info == &prefab)
	{
		eType = RESOURCE_TYPE::PREFAB;
	}

	if (m_mapRes[(UINT)eType].end() != m_mapRes[(UINT)eType].find(_strKey))
	{
		//	assert(NULL);

		m_mapRes[(UINT)eType].erase(_strKey);

		//	assert(NULL);
	}

	_pRes->SetName(_strKey);
	m_mapRes[(UINT)eType].insert(make_pair(_strKey, _pRes));
}


template<typename T>
inline void CResMgr::SubResource(const wstring & _strKey)
{
	static const type_info& info = typeid(T);
	static const type_info& mesh = typeid(CMesh);
	static const type_info& texture = typeid(CTexture);
	static const type_info& material = typeid(CMaterial);
	static const type_info& sound = typeid(CSound);
	static const type_info& prefab = typeid(CPrefab);

	RESOURCE_TYPE eType = RESOURCE_TYPE::END;

	if (&info == &mesh)
	{
		eType = RESOURCE_TYPE::MESH;
	}
	else if (&info == &texture)
	{
		eType = RESOURCE_TYPE::TEXTURE;
	}
	else if (&info == &material)
	{
		eType = RESOURCE_TYPE::MATERIAL;
	}
	else if (&info == &sound)
	{
		eType = RESOURCE_TYPE::SOUND;
	}
	else if (&info == &prefab)
	{
		eType = RESOURCE_TYPE::PREFAB;
	}

	if (m_mapRes[(UINT)eType].end() != m_mapRes[(UINT)eType].find(_strKey))
	{
		m_mapRes[(UINT)eType].erase(_strKey);
	}

	return;
}




template<typename T>
inline T * CResMgr::Load(const wstring & _strKey, const wstring & _strPath)
{
	static const type_info& info = typeid(T);
	static const type_info& mesh = typeid(CMesh);
	static const type_info& texture = typeid(CTexture);
	static const type_info& material = typeid(CMaterial);
	static const type_info& sound = typeid(CSound);
	static const type_info& prefab = typeid(CPrefab);
	
	RESOURCE_TYPE eType = RESOURCE_TYPE::END;

	if (&info == &mesh)
	{
		eType = RESOURCE_TYPE::MESH;
	}
	else if (&info == &texture)
	{
		eType = RESOURCE_TYPE::TEXTURE;
	}
	else if (&info == &material)
	{
		eType = RESOURCE_TYPE::MATERIAL;
	}
	else if (&info == &sound)
	{
		eType = RESOURCE_TYPE::SOUND;
	}
	else if (&info == &prefab)
	{
		eType = RESOURCE_TYPE::PREFAB;
	}

	map<wstring, CRes*>::iterator iter = m_mapRes[(UINT)eType].find(_strKey);
		
	if (!_strPath.empty())
	{
		// 파일 경로가 지정된 경우
		// 읽어들인다.
		if (iter != m_mapRes[(UINT)eType].end())
		{
			/*T* pRes = new T;
			wstring strFullPath = CPathMgr::GetResPath() + _strPath;
			pRes->Load(strFullPath, _strKey);

			return pRes;*/

			return 0;

			// 이미 등록시킬 키가 사용된 경우

		//	MessageBox(NULL, L"Currently the same key value exists in Resource Map.", L"Error Page.", MB_OK);

		//	return 0;
			/*assert(NULL);*/
		}

		// 경로로 부터 로딩 진행
		T* pRes = new T;
		wstring strFullPath = CPathMgr::GetResPath() + _strPath;
		pRes->Load(strFullPath, _strKey);
		pRes->SetName(_strKey);
		pRes->SetPath(_strPath);
		m_mapRes[(UINT)eType].insert(make_pair(_strKey, pRes));
		return pRes;
	}
	else if (iter == m_mapRes[(UINT)eType].end()) 
	{
		// 리소스 매니저에게, 이미 로딩된 리소스를 요청했는데 없는경우
		return nullptr;
	}
	
	return (T*)iter->second;
}
