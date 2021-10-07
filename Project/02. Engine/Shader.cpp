#include "Shader.h"

#include "PathMgr.h"
#include "Device.h"

#include "BlendState.h"

CShader::CShader()
	:m_pVSBlob(NULL)
	, m_pHSBlob(NULL)
	, m_pDSBlob(NULL)
	, m_pGSBlob(NULL)
	, m_pCSBlob(NULL)
	, m_pPSBlob(NULL)
	, m_pErrBlob(NULL)
	, m_pVS(NULL)
	, m_pHS(NULL)
	, m_pDS(NULL)
	, m_pGS(NULL)
	, m_pCS(NULL)
	, m_pPS(NULL)
	, m_pBlendState(NULL)
	, m_pDepthStencilState(NULL)
{
}

CShader::~CShader()
{
	SAFE_RELEASE(m_pVSBlob);
	SAFE_RELEASE(m_pHSBlob);
	SAFE_RELEASE(m_pDSBlob);
	SAFE_RELEASE(m_pGSBlob);
	SAFE_RELEASE(m_pCSBlob);
	SAFE_RELEASE(m_pPSBlob);
	SAFE_RELEASE(m_pErrBlob);
	SAFE_RELEASE(m_pVS);
	SAFE_RELEASE(m_pHS);
	SAFE_RELEASE(m_pDS);
	SAFE_RELEASE(m_pGS);
	SAFE_RELEASE(m_pCS);
	SAFE_RELEASE(m_pPS);
}

void CShader::UpdateData()
{
	CONTEXT->VSSetShader(m_pVS, NULL, NULL);
	CONTEXT->PSSetShader(m_pPS, NULL, NULL);

	// BlendState Setting
	if (nullptr != m_pBlendState)
		m_pBlendState->SetState();
	else
		m_pBlendState->SetDefaultState();

	// DepthStencilState Setting


}

int CShader::CreateVertexShader(const wstring & _strFileName
	, const string & _strFuncName, const string & _strVersion)
{
	wstring strFilePath = CPathMgr::GetResPath();
	strFilePath += L"Shader\\";
	strFilePath += _strFileName;
	
	string strErr;

	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strFilePath.c_str(), NULL
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), _strVersion.c_str(), iFlag, 0, &m_pVSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(NULL, strErr.c_str(), "쉐이더 생성 실패", MB_OK);
		return RET_FAILED;
	}

	DEVICE->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize()
		, NULL, &m_pVS);

	return RET_SUCCESS;
}

int CShader::CreatePixelShader(const wstring & _strFileName, const string & _strFuncName, const string & _strVersion)
{
	wstring strFilePath = CPathMgr::GetResPath();
	strFilePath += L"Shader\\";
	strFilePath += _strFileName;

	string strErr;

	int iFlag = 0;
#ifdef _DEBUG
	iFlag = D3DCOMPILE_DEBUG;
#endif

	if (FAILED(D3DCompileFromFile(strFilePath.c_str(), NULL
		, D3D_COMPILE_STANDARD_FILE_INCLUDE
		, _strFuncName.c_str(), _strVersion.c_str(), iFlag, 0, &m_pPSBlob, &m_pErrBlob)))
	{
		strErr = (char*)m_pErrBlob->GetBufferPointer();
		MessageBoxA(NULL, strErr.c_str(), "쉐이더 생성 실패", MB_OK);
		return RET_FAILED;
	}

	DEVICE->CreatePixelShader(m_pPSBlob->GetBufferPointer(), m_pPSBlob->GetBufferSize()
		, NULL, &m_pPS);

	return RET_SUCCESS;
}
