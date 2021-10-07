#include "Camera.h"

#include "Device.h"
#include "Transform.h"

#include "SceneMgr.h"
#include "Layer.h"

#include "TimeMgr.h"

#include "ConstBuffer.h"

CCamera::CCamera()
	: m_fNear(1.f)
	, m_fFar(1000.f)
	, m_fFOV(XM_PI / 4.f)
	, m_fScale(1.f)
	, m_tResolution{}
	, m_bPerspective(true)
	, m_fSpeed(1.f)
	, m_fRatio(0.f)
	, m_bTransforming(false)
{
	SetComponentType(COMPONENT_TYPE::CAMERA);
	m_tResolution = CDevice::GetInst()->GetResolution();	
}

CCamera::~CCamera()
{
}


int CCamera::update()
{
	return 0;
}

int CCamera::finalupdate()
{
	// 카메라가 월드에서, 원점에서, z 양의축을 바라보고 있는 상황
	Vec3 vWorldPos = -Transform()->GetWorldPos();	

	// 카메라를 Z 를 바라보게 하는 만큼의 회전행렬을 구한다.
	// 월드 기준 우, 상, 전 벡터를 얻어온다.
	m_matView = Matrix::Identity;
	m_matView.Right(Transform()->GetWorldDir(DIR_TYPE::RIGHT));
	m_matView.Up(Transform()->GetWorldDir(DIR_TYPE::UP));
	m_matView.Front(Transform()->GetWorldDir(DIR_TYPE::FRONT));
	m_matView = m_matView.Transpose();

	m_matView._41 = vWorldPos.Dot(Transform()->GetWorldDir(DIR_TYPE::RIGHT));
	m_matView._42 = vWorldPos.Dot(Transform()->GetWorldDir(DIR_TYPE::UP));
	m_matView._43 = vWorldPos.Dot(Transform()->GetWorldDir(DIR_TYPE::FRONT));

	g_transform.matView = m_matView;

	// 투영행렬
	// 결과값은 투영좌표계 x
	// 투영좌표계 x output.w;
	// output.w 에는 viewspace 의 z 값		
	if (m_bPerspective)
	{
		m_matProj = XMMatrixPerspectiveFovLH(m_fFOV, m_tResolution.x / m_tResolution.y, m_fNear, m_fFar);		 
	}
	else
	{
		float fWidth = m_tResolution.x * 0.5f * m_fScale;
		float fHeight = m_tResolution.y * 0.5f * m_fScale;

		m_matProj =	XMMatrixOrthographicOffCenterLH(-fWidth, fWidth, -fHeight, fHeight
				, m_fNear, m_fFar);
	}
	
	// 카메라를 현재 Scene 에 등록
	CSceneMgr::GetInst()->RegisterCamera(this);

	// 직교 <---> 원근 서로 변화가 생겼을 경우
	if (m_bTransforming)
	{
		// m_matOldProj --> m_matProj
		if (m_bPerspective)
			m_fSpeed += (float)DT * 1.f;
		else
		{
			m_fSpeed -= (float)DT * 24.f;
			if (m_fSpeed < 0.f)
				m_fSpeed = 0.05f;
		}

		m_fRatio += (float)(DT / 2.f) * m_fSpeed;

		if (m_fRatio >= 1.f)
		{
			m_fRatio = 1.f;
			m_bTransforming = false;
		}

		for (int i = 0; i < 4; ++i)
		{
			for (int j = 0; j < 4; ++j)
			{
				// 시간에 따라 행렬 변환						
				m_matProj.m[i][j] = Lerf(m_matPrevProj.m[i][j], m_matProj.m[i][j], m_fRatio);
			}
		}
	}

	g_transform.matProj = m_matProj;

	return 0;
}

void CCamera::render(CLayer** _pLayer)
{

	static CConstBuffer* pBuff = CDevice::GetInst()->FindCBuffer(L"Global");
	pBuff->UpdateData(&g_global, sizeof(tGlobalValue));
	pBuff->UpdateRegisterAll();

	// 카메라가 찍을 Layer 선별
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL == _pLayer[i] || !(m_iLayerMask & (1 << i)))
			continue;

		// GameObject 중에서, 화면에 보이는 Object 선별
		_pLayer[i]->render();
	}
}

void CCamera::CheckLayer(UINT _iIdx)
{
	if (m_iLayerMask & (1 << _iIdx))
	{
		m_iLayerMask &= ~(1 << _iIdx);
	}
	else
	{
		m_iLayerMask |= (1 << _iIdx);
	}	
}

void CCamera::CheckLayer(const wstring & _strLayerName)
{
	UINT iIdx = CSceneMgr::GetInst()->GetLayer(_strLayerName)->GetIdx();
	m_iLayerMask |= (1 << iIdx);
}

void CCamera::Save(FILE * _pFile)
{
	CComponent::Save(_pFile);

	fwrite(&m_fNear, sizeof(float), 1, _pFile);	// 최소거리
	fwrite(&m_fFar, sizeof(float), 1, _pFile);	// 최대거리
	fwrite(&m_fFOV, sizeof(float), 1, _pFile);	// 시야각 (원근투영)
	fwrite(&m_fScale, sizeof(float), 1, _pFile);	// 투영 크기(직교투영)

	fwrite(&m_iLayerMask, sizeof(UINT), 1, _pFile);	// 투영 크기(직교투영)
	fwrite(&m_bPerspective, 1, 1, _pFile);
}

void CCamera::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);
	
	fread(&m_fNear, sizeof(float), 1, _pFile);	// 최소거리
	fread(&m_fFar, sizeof(float), 1, _pFile);	// 최대거리
	fread(&m_fFOV, sizeof(float), 1, _pFile);	// 시야각 (원근투영)
	fread(&m_fScale, sizeof(float), 1, _pFile);	// 투영 크기(직교투영)

	fread(&m_iLayerMask, sizeof(UINT), 1, _pFile);	// 투영 크기(직교투영)
	fread(&m_bPerspective, 1, 1, _pFile);
}