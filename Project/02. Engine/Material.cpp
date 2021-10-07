#include "Material.h"
#include "Device.h"
#include "ConstBuffer.h"
#include "Texture.h"

#include "ResMgr.h"
#include "ShaderMgr.h"

CMaterial::CMaterial()
	: m_pShader(nullptr)
	, m_tParam{}
	, m_arrTex{}
{
}

CMaterial::~CMaterial()
{
}

void CMaterial::UpdateData()
{
	m_pShader->UpdateData();

	// 상수 데이터 전달
	static CConstBuffer* pBuffer = CDevice::GetInst()->FindCBuffer(L"ShaderParam");

	pBuffer->UpdateData(&m_tParam, sizeof(tShaderParam));
	pBuffer->UpdateRegisterAll();

	// 텍스쳐 데이터 전달
	UINT iTexCount = (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0;
	for (UINT i = 0; i < iTexCount; ++i)
	{
		if (nullptr != m_arrTex[i])
			m_arrTex[i]->SetRegisterAll(i);
	}
}

void CMaterial::SetData(SHADER_PARAM _eType, void * _pData)
{
	switch (_eType)
	{
	case SHADER_PARAM::INT_0:
	case SHADER_PARAM::INT_1:
	case SHADER_PARAM::INT_2:
	case SHADER_PARAM::INT_3:
		m_tParam.arrInt[(UINT)_eType - (UINT)SHADER_PARAM::INT_0] = *((int*)_pData);
		break;
	case SHADER_PARAM::FLOAT_0:
	case SHADER_PARAM::FLOAT_1:
	case SHADER_PARAM::FLOAT_2:
	case SHADER_PARAM::FLOAT_3:
		m_tParam.arrFloat[(UINT)_eType - (UINT)SHADER_PARAM::FLOAT_0] = *((float*)_pData);
		break;
	case SHADER_PARAM::VEC2_0:
	case SHADER_PARAM::VEC2_1:
	case SHADER_PARAM::VEC2_2:
	case SHADER_PARAM::VEC2_3:
		m_tParam.arrVec2[(UINT)_eType - (UINT)SHADER_PARAM::VEC2_0] = *((Vec2*)_pData);
		break;
	case SHADER_PARAM::VEC4_0:
	case SHADER_PARAM::VEC4_1:
	case SHADER_PARAM::VEC4_2:
	case SHADER_PARAM::VEC4_3:
		m_tParam.arrVec4[(UINT)_eType - (UINT)SHADER_PARAM::VEC4_0] = *((Vec4*)_pData);
		break;
	case SHADER_PARAM::TEX_0:
	case SHADER_PARAM::TEX_1:
	case SHADER_PARAM::TEX_2:
	case SHADER_PARAM::TEX_3:
	case SHADER_PARAM::TEX_4:
	case SHADER_PARAM::TEX_5:
	case SHADER_PARAM::TEX_6:
	case SHADER_PARAM::TEX_7:
		m_arrTex[(UINT)_eType - (UINT)SHADER_PARAM::TEX_0] = (CTexture*)_pData;
		break;
	default:
		assert(nullptr);
		break;
	}
}

void CMaterial::Load(const wstring & _strFullPath, const wstring & _strName)
{
}

CMaterial * CMaterial::Clone()
{
	CMaterial* pCloneMtrl = new CMaterial(*this);
	CResMgr::GetInst()->AddClone(pCloneMtrl);
	return pCloneMtrl;
}

void CMaterial::FileSave(FILE * _pFile)
{
	// Material Key
	SaveWString(GetName(), _pFile);

	// 참조하는 쉐이더
	wstring strShaderName;
	if (nullptr != m_pShader)
		strShaderName = m_pShader->GetName();	
	
	SaveWString(strShaderName, _pFile);

	// 쉐이더에 전달할 값
	fwrite(&m_tParam, sizeof(tShaderParam), 1, _pFile);

	// Material 이 쉐이더로 전달하기위해서 참조하던 텍스쳐들
	for (int i = 0; i < (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0; ++i)
	{
		wstring strTexName;
		if (nullptr != m_arrTex[i])
			strTexName = m_arrTex[i]->GetName();

		SaveWString(strTexName, _pFile);
	}	

	int iCount = (int)m_vecParam.size();
	fwrite(&iCount, sizeof(int), 1, _pFile);
	for (int i = 0; i < iCount; ++i)
	{
		fwrite(&m_vecParam[i].eType, sizeof(int), 1, _pFile);		
		SaveWString(m_vecParam[i].strParamName, _pFile);
	}	
}

void CMaterial::FileLoad(FILE * _pFile)
{
	// Material Key
	wstring strMtrlName = LoadWString(_pFile);

	// 참조하는 쉐이더
	wstring strShaderName = LoadWString(_pFile);

	// 쉐이더에 전달할 값
	tShaderParam tParam = {};
	fread(&tParam, sizeof(tShaderParam), 1, _pFile);

	// Material 이 쉐이더로 전달하기위해서 참조하던 텍스쳐들
	CTexture* arrTex[(UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0] = {};

	for (int i = 0; i < (UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0; ++i)
	{
		wstring strTexName = LoadWString(_pFile);
		arrTex[i] = CResMgr::GetInst()->Load<CTexture>(strTexName);
	}
	
	int iCount = 0;
	fread(&iCount, sizeof(int), 1, _pFile);
	vector<tParamInfo> vecParam;
	
	for (int i = 0; i < iCount; ++i)
	{
		tParamInfo info = {};
		
		fread(&info.eType, sizeof(int), 1, _pFile);
		info.strParamName = LoadWString(_pFile);

		vecParam.push_back(info);
	}
	
	CMaterial* pMtrl = CResMgr::GetInst()->Load<CMaterial>(strMtrlName);
	if (nullptr == pMtrl)
	{
		pMtrl = new CMaterial;
		// ResMgr 에 등록
		CResMgr::GetInst()->AddResource<CMaterial>(strMtrlName, pMtrl);
	}

	// 읽은 값으로 Material 셋팅
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(strShaderName));
	pMtrl->m_tParam = tParam;
	pMtrl->m_vecParam = vecParam;
	memcpy(pMtrl->m_arrTex, arrTex, sizeof(void*) * ((UINT)SHADER_PARAM::TEX_END - (UINT)SHADER_PARAM::TEX_0));		
}

