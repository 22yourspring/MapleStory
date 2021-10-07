#include "Transform.h"

#include "Device.h"
#include "ConstBuffer.h"
#include "Animation2D.h"
#include "Animator2D.h"

#include "SceneMgr.h"
#include "Layer.h"

tTransformMatrix g_transform;

const Vec3 g_vDir[(UINT)DIR_TYPE::END] = { Vec3(1.f, 0.f, 0.f), Vec3(0.f, 1.f, 0.f), Vec3(0.f, 0.f, 1.f) };


CTransform::CTransform()
	: m_vLocalDir{Vec3::Right, Vec3::Up, Vec3::Front}
	, m_vWorldDir{ Vec3::Right, Vec3::Up, Vec3::Front }
{
	SetComponentType(COMPONENT_TYPE::TRANSFORM);
}

CTransform::~CTransform()
{
}

int CTransform::update()
{
	return 0;
}

int CTransform::finalupdate()
{
	// 월드 행렬 계산하기
	// 크기 행렬
	Matrix matScale = XMMatrixScaling(m_vLocalScale.x, m_vLocalScale.y, m_vLocalScale.z);

	// 회전(자전) 행렬
	Matrix matRotate = XMMatrixRotationX(m_vLocalRot.x);
	matRotate *= XMMatrixRotationY(m_vLocalRot.y);
	matRotate *= XMMatrixRotationZ(m_vLocalRot.z);

	// 방향벡터(우, 상, 전) 갱신하기
	for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
	{
		m_vLocalDir[i] = XMVector3TransformNormal(g_vDir[i], matRotate);
		m_vLocalDir[i].Normalize();
		m_vWorldDir[i] = m_vLocalDir[i];
	}

	Matrix matTrans;

	if (nullptr != Animator2D() && nullptr != Animator2D()->GetCurAnim())
	{
		float OFFSETX = Animator2D()->GetCurAnim()->GetFrmVec()[Animator2D()->GetCurAnim()->GetCurFrm()].vOffset.x;
		float OFFSETY = Animator2D()->GetCurAnim()->GetFrmVec()[Animator2D()->GetCurAnim()->GetCurFrm()].vOffset.y;

		matTrans = XMMatrixTranslation(m_vLocalPos.x + OFFSETX, m_vLocalPos.y + OFFSETY, m_vLocalPos.z);
	}
	else
	{	// 이동 행렬
		matTrans = XMMatrixTranslation(m_vLocalPos.x, m_vLocalPos.y, m_vLocalPos.z);
	}
	// 월드행렬
	m_matWorld = matScale * matRotate * matTrans;

	if (nullptr != ParentObject())
	{
		/*m_matWorld *= ParentObject()->Transform()->GetWorldMat();
		
		for (UINT i = 0; i < (UINT)DIR_TYPE::END; ++i)
		{
			m_vWorldDir[i] = XMVector3TransformNormal(g_vDir[i], m_matWorld);
			m_vWorldDir[i].Normalize();
		}*/

		//메이플 전용 부모오브젝트로부터 영향 받지 않게 막기
	}

	return 0;
}

void CTransform::UpdateData()
{
	g_transform.matWorld = m_matWorld;

	if (CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"UI") != nullptr)
	{
		if (GetGameObject()->GetLayerIdx() == CSceneMgr::GetInst()->GetCurScene()->GetLayer(L"UI")->GetIdx())
		{
			g_transform.matWV = g_transform.matWorld;
		}
		else
		{
			g_transform.matWV = g_transform.matWorld * g_transform.matView;
		}
	}
	else
	{
		g_transform.matWV = g_transform.matWorld * g_transform.matView;
	}
	
	g_transform.matWVP = g_transform.matWV * g_transform.matProj;

	static CConstBuffer* pBuffer = CDevice::GetInst()->FindCBuffer(L"Transform");
	
	pBuffer->UpdateData(&g_transform, sizeof(tTransformMatrix));
	pBuffer->UpdateRegister(SHADER_TYPE::ST_VERTEX);
}

Vec3 CTransform::GetWorldPos()
{
	return XMVector3TransformCoord(Vec3::Zero, m_matWorld);
}

void CTransform::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_vLocalPos, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vLocalScale, sizeof(Vec3), 1, _pFile);
	fwrite(&m_vLocalRot, sizeof(Vec3), 1, _pFile);
}

void CTransform::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);

	fread(&m_vLocalPos, sizeof(Vec3), 1, _pFile);
	fread(&m_vLocalScale, sizeof(Vec3), 1, _pFile);
	fread(&m_vLocalRot, sizeof(Vec3), 1, _pFile);

}