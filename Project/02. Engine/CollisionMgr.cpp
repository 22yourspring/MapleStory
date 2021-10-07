#include "CollisionMgr.h"
#include "Collider.h"

#include "SceneMgr.h"
#include "Layer.h"

CCollisionMgr::CCollisionMgr()
	: m_iColliderCheck{}
{
}

CCollisionMgr::~CCollisionMgr()
{
}

int CCollisionMgr::update()
{	
	// �浹 ����
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (int j = i ; j < MAX_LAYER; ++j)
		{
			if (m_iColliderCheck[i] & (1 << j))
			{
				Collision(i, j);
			}
		}
	}
	
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{		
		m_arrCollider[i].clear();
	}

	return 0;
}

void CCollisionMgr::CollisionCheck(UINT _iLayerIdx1, UINT _iLayerIdx2)
{
	assert(CSceneMgr::GetInst()->GetLayer(_iLayerIdx1));
	assert(CSceneMgr::GetInst()->GetLayer(_iLayerIdx2));

	// �� ���� �� ���� ���� �� �ε�����...
	UINT iRow = _iLayerIdx1;
	UINT iCol = _iLayerIdx2;

	if (iRow > _iLayerIdx2)
	{
		iRow = _iLayerIdx2;
		iCol = _iLayerIdx1;
	}
	
	UINT iCheck = 1 << iCol;
	m_iColliderCheck[iRow] |= iCheck;
}

void CCollisionMgr::CollisionCheck(const wstring & _strLayerName1, const wstring & _strLayerName2)
{
	UINT iLayerIdx1 = CSceneMgr::GetInst()->GetLayer(_strLayerName1)->GetIdx();
	UINT iLayerIdx2 = CSceneMgr::GetInst()->GetLayer(_strLayerName2)->GetIdx();
	
	CollisionCheck(iLayerIdx1, iLayerIdx2);
}

void CCollisionMgr::Collision(UINT _iFirst, UINT _iSecond)
{
	for (UINT i = 0; i < m_arrCollider[_iFirst].size(); ++i)
	{		
		for (UINT j = 0; j < m_arrCollider[_iSecond].size(); ++j)
		{
			CID id;
			id._id_1 = m_arrCollider[_iFirst][i]->GetID();
			id._id_2 = m_arrCollider[_iSecond][j]->GetID();			
			map<ULONGLONG, bool>::iterator iter = m_mapID.find(id.id);

			// �浹 �ߴ�.
			if (IsCollision(m_arrCollider[_iFirst][i], m_arrCollider[_iSecond][j]))
			{				
				if (iter == m_mapID.end())
				{
					// ������ ��� x -> ������ �浹���� ����. -> ���� �浹
					m_mapID.insert(make_pair(id.id, true));
					m_arrCollider[_iFirst][i]->OnCollisionEnter(m_arrCollider[_iSecond][j]);
					m_arrCollider[_iSecond][j]->OnCollisionEnter(m_arrCollider[_iFirst][i]);
				}
				else if(false == iter->second)
				{
					// ������ �浹���� ���� -> ���� �浹
					iter->second = true;
					m_arrCollider[_iFirst][i]->OnCollisionEnter(m_arrCollider[_iSecond][j]);
					m_arrCollider[_iSecond][j]->OnCollisionEnter(m_arrCollider[_iFirst][i]);
				}
				else
				{
					// �浹 ��
					m_arrCollider[_iFirst][i]->OnCollision(m_arrCollider[_iSecond][j]);
					m_arrCollider[_iSecond][j]->OnCollision(m_arrCollider[_iFirst][i]);
				}
			}
			// �浹 ����
			else
			{
				if (iter != m_mapID.end() && true == iter->second)
				{
					// �浹 �� ��� ����
					iter->second = false;
					m_arrCollider[_iFirst][i]->OnCollisionExit(m_arrCollider[_iSecond][j]);
					m_arrCollider[_iSecond][j]->OnCollisionExit(m_arrCollider[_iFirst][i]);
				}
			}
		}
	}	
}

bool CCollisionMgr::IsCollision(CCollider * _pFirst, CCollider * _pSecond)
{	
	Vec3 vCenterFirst = XMVector3TransformCoord(Vec3::Zero, _pFirst->GetColMat());
	Vec3 vCenterSecond = XMVector3TransformCoord(Vec3::Zero, _pSecond->GetColMat());

	if (COLLIDER_TYPE::COLLIDER_2D_BOX == _pFirst->GetColliderType()
		&& COLLIDER_TYPE::COLLIDER_2D_BOX == _pSecond->GetColliderType())
	{
		// �� �浹ü ���� �浹 ���� Ȯ���ϱ�
		// 0 ---- 1
		// |      |
		// |      |
		// 3 ---- 2
		// �浹ü ��ġ�� ���� ���� ���ý����̽� ���� 4���� ����
		static Vec3 arrLocal[4] = { Vec3(-0.5f, 0.5f, 0.f)
			, Vec3(0.5f, 0.5f, 0.f)
			, Vec3(0.5f, -0.5f, 0.f)
			, Vec3(-0.5f, -0.5f, 0.f) };

		Vec3 col1[3] = {};
		// �ĺ�
		Vec3 vSource[4] = {};
		Vec3 vProj[4] = {};
		Vec3 vCenter = vCenterFirst - vCenterSecond;

		// ù��° �浹ü�� ���� ������� ������ų ���� ���ϱ�
		col1[0] = XMVector3TransformCoord(arrLocal[3], _pFirst->GetColMat());
		col1[1] = XMVector3TransformCoord(arrLocal[0], _pFirst->GetColMat());
		col1[2] = XMVector3TransformCoord(arrLocal[2], _pFirst->GetColMat());

		vSource[0] = col1[2] - col1[0];
		vSource[1] = col1[1] - col1[0];

		vProj[0] = vSource[0]; vProj[0].Normalize();
		vProj[1] = vSource[1]; vProj[1].Normalize();

		// �ι�° �浹ü�� ���� ���� �ĺ���, ���� �� ����
		col1[0] = XMVector3TransformCoord(arrLocal[3], _pSecond->GetColMat());
		col1[1] = XMVector3TransformCoord(arrLocal[0], _pSecond->GetColMat());
		col1[2] = XMVector3TransformCoord(arrLocal[2], _pSecond->GetColMat());

		vSource[2] = col1[2] - col1[0];
		vSource[3] = col1[1] - col1[0];

		vProj[2] = vSource[2]; vProj[2].Normalize();
		vProj[3] = vSource[3]; vProj[3].Normalize();

		for (int i = 0; i < 4; ++i)
		{
			float fDist = 0.f;
			float fCenter = 0.f;
			for (int j = 0; j < 4; ++j)
			{
				fDist += abs(vSource[j].Dot(vProj[i]));
			}
			fDist *= 0.5f;
			fCenter = abs(vCenter.Dot(vProj[i]));

			if (fCenter > fDist)
			{
				return false;
			}
		}
		return true;
	}	
	else if (COLLIDER_TYPE::COLLIDER_2D_CIRCLE == _pFirst->GetColliderType() &&
		COLLIDER_TYPE::COLLIDER_2D_CIRCLE == _pSecond->GetColliderType())
	{
		// �� vs ��
		float fAddRadius = _pFirst->GetWorldRadius() + _pSecond->GetWorldRadius();
		float fDist = Vec3::Distance(vCenterFirst, vCenterSecond);

		if (fDist < fAddRadius)
		{
			return true;
		}
		return false;
	}
	else 
	{
		// �� vs �簢��

	}
	return true;
}