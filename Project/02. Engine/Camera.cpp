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
	// ī�޶� ���忡��, ��������, z �������� �ٶ󺸰� �ִ� ��Ȳ
	Vec3 vWorldPos = -Transform()->GetWorldPos();	

	// ī�޶� Z �� �ٶ󺸰� �ϴ� ��ŭ�� ȸ������� ���Ѵ�.
	// ���� ���� ��, ��, �� ���͸� ���´�.
	m_matView = Matrix::Identity;
	m_matView.Right(Transform()->GetWorldDir(DIR_TYPE::RIGHT));
	m_matView.Up(Transform()->GetWorldDir(DIR_TYPE::UP));
	m_matView.Front(Transform()->GetWorldDir(DIR_TYPE::FRONT));
	m_matView = m_matView.Transpose();

	m_matView._41 = vWorldPos.Dot(Transform()->GetWorldDir(DIR_TYPE::RIGHT));
	m_matView._42 = vWorldPos.Dot(Transform()->GetWorldDir(DIR_TYPE::UP));
	m_matView._43 = vWorldPos.Dot(Transform()->GetWorldDir(DIR_TYPE::FRONT));

	g_transform.matView = m_matView;

	// �������
	// ������� ������ǥ�� x
	// ������ǥ�� x output.w;
	// output.w ���� viewspace �� z ��		
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
	
	// ī�޶� ���� Scene �� ���
	CSceneMgr::GetInst()->RegisterCamera(this);

	// ���� <---> ���� ���� ��ȭ�� ������ ���
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
				// �ð��� ���� ��� ��ȯ						
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

	// ī�޶� ���� Layer ����
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (NULL == _pLayer[i] || !(m_iLayerMask & (1 << i)))
			continue;

		// GameObject �߿���, ȭ�鿡 ���̴� Object ����
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

	fwrite(&m_fNear, sizeof(float), 1, _pFile);	// �ּҰŸ�
	fwrite(&m_fFar, sizeof(float), 1, _pFile);	// �ִ�Ÿ�
	fwrite(&m_fFOV, sizeof(float), 1, _pFile);	// �þ߰� (��������)
	fwrite(&m_fScale, sizeof(float), 1, _pFile);	// ���� ũ��(��������)

	fwrite(&m_iLayerMask, sizeof(UINT), 1, _pFile);	// ���� ũ��(��������)
	fwrite(&m_bPerspective, 1, 1, _pFile);
}

void CCamera::Load(FILE * _pFile)
{
	CComponent::Load(_pFile);
	
	fread(&m_fNear, sizeof(float), 1, _pFile);	// �ּҰŸ�
	fread(&m_fFar, sizeof(float), 1, _pFile);	// �ִ�Ÿ�
	fread(&m_fFOV, sizeof(float), 1, _pFile);	// �þ߰� (��������)
	fread(&m_fScale, sizeof(float), 1, _pFile);	// ���� ũ��(��������)

	fread(&m_iLayerMask, sizeof(UINT), 1, _pFile);	// ���� ũ��(��������)
	fread(&m_bPerspective, 1, 1, _pFile);
}