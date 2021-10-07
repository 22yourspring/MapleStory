#include "Mesh.h"

#include "Device.h"
#include "Shader.h"

CMesh::CMesh()
	: m_pVB(NULL)
	, m_pIB(NULL)
	, m_pLayout(NULL)
	, m_iVtxSize(0)
	, m_iVtxCount(0)
	, m_iIdxSize	(0)
	, m_iIdxCount	(0)	
	, m_eTopology(D3D_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_TRIANGLELIST)
	, m_pVtxMem(nullptr)
	, m_pIdxMem(nullptr)	
{
}

CMesh::~CMesh()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pLayout);
}

CMesh * CMesh::Create(void * _pVtx, UINT _iVtxSize, UINT _iVtxCount
	, D3D11_USAGE _eVtxUsage, void * _pIdx
	, UINT _iIdxSize, UINT _iIdxCount, D3D11_USAGE _eIdxUsage, D3D11_PRIMITIVE_TOPOLOGY _eTopology)
{
	// Vertex buffer ����
	D3D11_BUFFER_DESC tVtxDesc = {};
	tVtxDesc = {};
	tVtxDesc.ByteWidth = _iVtxSize * _iVtxCount;
	tVtxDesc.Usage = _eVtxUsage;
	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == tVtxDesc.Usage)
	{
		tVtxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}	
	tVtxDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;

	D3D11_SUBRESOURCE_DATA tSubData = {};
	tSubData.pSysMem = _pVtx;

	ID3D11Buffer* pVtxBuff = NULL;
	if (FAILED(DEVICE->CreateBuffer(&tVtxDesc, &tSubData, &pVtxBuff)))
	{
		return NULL;
	}

	// Index Buffer ����	
	D3D11_BUFFER_DESC tIdxDesc = {};
	tIdxDesc.ByteWidth = _iIdxSize * _iIdxCount;
	tIdxDesc.Usage = _eIdxUsage;
	if (D3D11_USAGE::D3D11_USAGE_DYNAMIC == tIdxDesc.Usage)
	{
		tIdxDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	}
	tIdxDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;

	tSubData = {};
	tSubData.pSysMem = _pIdx;

	ID3D11Buffer* pIdxBuff = NULL;
	if (FAILED(DEVICE->CreateBuffer(&tIdxDesc, &tSubData, &pIdxBuff)))
	{
		return NULL;
	}

	CMesh* pMesh = new CMesh;
	pMesh->m_pVB = pVtxBuff;
	pMesh->m_tVtxDesc = tVtxDesc;
	pMesh->m_iVtxSize = _iVtxSize;
	pMesh->m_iVtxCount = _iVtxCount;

	pMesh->m_pIB = pIdxBuff;
	pMesh->m_iIdxSize = _iIdxSize;
	pMesh->m_iIdxCount = _iIdxCount;
	pMesh->m_tIdxDesc = tIdxDesc;
	pMesh->m_eTopology = _eTopology;
	
	// Layout ���� �����
	// Layout �����
	// �ϳ��� ���� ������ ��� �Ǿ��ִ����� �˷���
	// vertex ���̴����� ���ϴ� ������ �����ؼ� ������ �� �ֵ��� �˸�
	int iOffset = 0;

	D3D11_INPUT_ELEMENT_DESC tLayoutDesc[3] = {};

	tLayoutDesc[0].SemanticName = "POSITION"; // ���ؽ� ���̴��� ��ġ�ϴ� �ø�ƽ �̸�
	tLayoutDesc[0].SemanticIndex = 0;			// ���� �ø�ƽ�� ���� offset �ε���
	tLayoutDesc[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	tLayoutDesc[0].InputSlot = 0;
	tLayoutDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc[0].InstanceDataStepRate = 0; // �ν��Ͻ� �Ҷ� 
	tLayoutDesc[0].AlignedByteOffset = iOffset;

	iOffset += GetSizeofFormat(DXGI_FORMAT_R32G32B32_FLOAT);

	tLayoutDesc[1].SemanticName = "COLOR"; // ���ؽ� ���̴��� ��ġ�ϴ� �ø�ƽ �̸�
	tLayoutDesc[1].SemanticIndex = 0;			// ���� �ø�ƽ�� ���� offset �ε���
	tLayoutDesc[1].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	tLayoutDesc[1].InputSlot = 0;
	tLayoutDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc[1].InstanceDataStepRate = 0; // �ν��Ͻ� �Ҷ� 
	tLayoutDesc[1].AlignedByteOffset = iOffset;

	iOffset += GetSizeofFormat(DXGI_FORMAT_R32G32B32A32_FLOAT);

	tLayoutDesc[2].SemanticName = "TEXCOORD"; // ���ؽ� ���̴��� ��ġ�ϴ� �ø�ƽ �̸�
	tLayoutDesc[2].SemanticIndex = 0;			// ���� �ø�ƽ�� ���� offset �ε���
	tLayoutDesc[2].Format = DXGI_FORMAT_R32G32_FLOAT;
	tLayoutDesc[2].InputSlot = 0;
	tLayoutDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	tLayoutDesc[2].InstanceDataStepRate = 0; // �ν��Ͻ� �Ҷ� 
	tLayoutDesc[2].AlignedByteOffset = iOffset;

	iOffset += GetSizeofFormat(DXGI_FORMAT_R32G32_FLOAT);
		   
	for (UINT i = 0; i < 3; ++i)
	{
		pMesh->m_vecLayout.push_back(tLayoutDesc[i]);
	}

	return pMesh;
}

void CMesh::UpdateData()
{
}

void CMesh::SetShader(CShader * _pShader)
{	
	if (m_pVtxBlob == _pShader->GetVSBlob())
		return;

	m_pVtxBlob = _pShader->GetVSBlob();
	
	SAFE_RELEASE(m_pLayout);
	DEVICE->CreateInputLayout(&m_vecLayout[0], (UINT)m_vecLayout.size(), m_pVtxBlob->GetBufferPointer(), m_pVtxBlob->GetBufferSize(), &m_pLayout);	
}

void CMesh::render()
{
	CONTEXT->IASetInputLayout(m_pLayout); // ���� ���� �˸���
	
	UINT iStride = m_iVtxSize;	// ���� �Ѱ��� ����(���۳����� ���� �������� �̵��ϱ� ���� ����)
	UINT iOffset = 0;			// ���۳����� ���° �������� ������ ���� ����
	CONTEXT->IASetVertexBuffers(0, 1, &m_pVB, &iStride, &iOffset);
	CONTEXT->IASetPrimitiveTopology(m_eTopology); // 3D ���� �⺻ ������� ���� ���� ����
	CONTEXT->IASetIndexBuffer(m_pIB, DXGI_FORMAT_R32_UINT, 0);
	CONTEXT->DrawIndexed(m_iIdxCount, 0, 0);
}

void CMesh::FileSave(FILE * _pFile)
{	
	
}

void CMesh::FileLoad(FILE * _pFile)
{
	
}