#pragma once

#include "global.h"

class CCollider;

union CID
{
	struct
	{
		DWORD _id_1;
		DWORD _id_2;
	};
	ULONGLONG id;
};

class CCollisionMgr
{
	SINGLE(CCollisionMgr);

private:
	vector<CCollider*>		m_arrCollider[MAX_LAYER];

	UINT					m_iColliderCheck[MAX_LAYER];
	map<ULONGLONG, bool>	m_mapID;

public:
	int update();	

public:
	void AddCollider(UINT _iLayerIdx, CCollider* _pCollider)
	{
		m_arrCollider[_iLayerIdx].push_back(_pCollider);
	}

	void CollisionCheck(UINT _iLayerIdx1, UINT _iLayerIdx2);
	void CollisionCheck(const wstring& _strLayerName1, const wstring& _strLayerName2);

private:
	void Collision(UINT _iFirst, UINT _iSecond);
	bool IsCollision(CCollider* _pFirst, CCollider* _pSecond);
};

