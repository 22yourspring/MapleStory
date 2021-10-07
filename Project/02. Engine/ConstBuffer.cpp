#include "ConstBuffer.h"

#include "Device.h"

CConstBuffer::CConstBuffer()
	: m_pCB(NULL)
	, m_iMaxSize(0)
	, m_iRegister(0)
{
}

CConstBuffer::~CConstBuffer()
{
	SAFE_RELEASE(m_pCB);
}

CConstBuffer * CConstBuffer::Create(const wstring & _strName, UINT _iSize, UINT _iRegister)
{
	D3D11_BUFFER_DESC tBufferDesc = D3D11_BUFFER_DESC{};
	tBufferDesc.ByteWidth = _iSize;
	tBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	tBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	tBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	ID3D11Buffer* pCB = NULL;
	if (FAILED(DEVICE->CreateBuffer(&tBufferDesc, NULL, &pCB)))
	{
		assert(NULL);
	}

	CConstBuffer* pBuffer = new CConstBuffer;
	pBuffer->m_iMaxSize = _iSize;
	pBuffer->m_pCB = pCB;
	pBuffer->m_iRegister = _iRegister;
	pBuffer->SetName(_strName);


	return pBuffer;
}

void CConstBuffer::UpdateData(void * _pSrc, UINT _iSize)
{
	// 상수 버퍼의 행렬 정보를 변경
	D3D11_MAPPED_SUBRESOURCE tSub = {};
	CONTEXT->Map(m_pCB, 0, D3D11_MAP_WRITE_DISCARD, 0, &tSub);
	memcpy(tSub.pData, _pSrc, _iSize);
	CONTEXT->Unmap(m_pCB, 0);
}

void CConstBuffer::UpdateRegister(UINT _iShaderType)
{
	// 상수버퍼를 레지스터에 전달
	if(_iShaderType & SHADER_TYPE::ST_VERTEX)
		CONTEXT->VSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	if (_iShaderType & SHADER_TYPE::ST_HULL)
		CONTEXT->HSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	if (_iShaderType & SHADER_TYPE::ST_DOMAIN)
		CONTEXT->DSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	if (_iShaderType & SHADER_TYPE::ST_GEOMETRY)
		CONTEXT->GSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	if (_iShaderType & SHADER_TYPE::ST_COMPUTE)
		CONTEXT->CSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	if (_iShaderType & SHADER_TYPE::ST_PIXEL)
		CONTEXT->PSSetConstantBuffers(m_iRegister, 1, &m_pCB);
}

void CConstBuffer::UpdateRegisterAll()
{
	CONTEXT->VSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->HSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->DSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->GSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->CSSetConstantBuffers(m_iRegister, 1, &m_pCB);
	CONTEXT->PSSetConstantBuffers(m_iRegister, 1, &m_pCB);
}

void CConstBuffer::ClearData()
{
	ID3D11Buffer* pBuffer = nullptr;
	CONTEXT->VSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->HSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->DSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->GSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->CSSetConstantBuffers(m_iRegister, 1, &pBuffer);
	CONTEXT->PSSetConstantBuffers(m_iRegister, 1, &pBuffer);
}