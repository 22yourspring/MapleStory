#pragma once

#include "global.h"

int GetSizeofFormat(DXGI_FORMAT _eFormat);

template<typename T>
void Safe_Delete_list(list<T> _list)
{
	typename list<T>::iterator iter = _list.begin();
	for (; iter != _list.end(); ++iter)
	{
		if (*iter != NULL)
			delete *iter;
	}
}

template<typename T>
void Safe_Delete_Vec(vector<T> _vec)
{
	for (UINT i = 0; i < _vec.size(); ++i)
	{
		if (NULL != _vec[i])
		{		
			delete _vec[i];
		}
	}
}

template<typename T1, typename T2>
void Safe_Delete_Map(map<T1, T2> _map)
{
	typename map<T1, T2>::iterator iter = _map.begin();
	for (; iter != _map.end(); ++iter)
	{
		if (NULL != iter->second)
			delete iter->second;
	}
}

template <typename T, int iSize>
void Safe_Delete_Array(T* (&_pArr)[iSize])
{
	for (int i = 0; i < iSize; ++i)
	{
		SAFE_DELETE(_pArr[i]);
	}
}

template <typename T, int iSize>
void Safe_Release_Array(T* (&_pArr)[iSize])
{
	for (int i = 0; i < iSize; ++i)
	{
		SAFE_RELEASE(_pArr[i]);
	}
}

void		SaveWString(const wstring& _str, FILE* _pFile);
wchar_t*	LoadWString(FILE* _pFile);


float Lerf(float _fStart, float _fDest, float _fRatio);

float GetRadian(float _fDegree);
float GetDegree(float _fRadian);