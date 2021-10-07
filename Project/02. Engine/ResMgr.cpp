#include "ResMgr.h"

#include "Ptr.h"
#include "Mesh.h"
#include "Material.h"
#include "ShaderMgr.h"


CResMgr::CResMgr()
{
}

CResMgr::~CResMgr()
{
	for (UINT i = 0; i < (UINT)RESOURCE_TYPE::END; ++i)
	{
		Safe_Delete_Map(m_mapRes[i]);
	}

	Safe_Delete_Vec(m_vecCloneRes);
}

void CResMgr::init()
{
	CreateDefaultMesh();
	CreateDefaultMaterial();
}


void CResMgr::CreateDefaultMesh()
{
	//==========
	// Rect 메쉬
	//==========
	VTX arrVTX[4] = {};
	arrVTX[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVTX[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVTX[0].vUV = Vec2(0.f, 0.f);

	arrVTX[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVTX[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVTX[1].vUV = Vec2(1.f, 0.f);

	arrVTX[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVTX[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[2].vUV = Vec2(1.f, 1.f);

	arrVTX[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVTX[3].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVTX[3].vUV = Vec2(0.f, 1.f);


	// 인덱스 버퍼 만들기
	IDX32 arrIdx[2] = {};
	arrIdx[0]._1 = 0;
	arrIdx[0]._2 = 1;
	arrIdx[0]._3 = 2;

	arrIdx[1]._1 = 0;
	arrIdx[1]._2 = 2;
	arrIdx[1]._3 = 3;

	Ptr<CMesh> pMesh = CMesh::Create(arrVTX, sizeof(VTX), 4, D3D11_USAGE_DEFAULT
		, arrIdx, sizeof(UINT), 6, D3D11_USAGE_DEFAULT);

	AddResource<CMesh>(L"RectMesh", pMesh);


	//===================
	// Collider Rect 메쉬
	//===================
	// 인덱스 버퍼 만들기
	UINT idx[5] = { 0, 1, 2, 3, 0 };

	pMesh = CMesh::Create(arrVTX, sizeof(VTX), 4, D3D11_USAGE_DEFAULT
		, idx, sizeof(UINT), 5, D3D11_USAGE_DEFAULT, D3D_PRIMITIVE_TOPOLOGY_LINESTRIP);
	AddResource<CMesh>(L"ColliderRectMesh", pMesh);

	   
	//=============
	// Circle 메쉬
	//=============
	vector<VTX> vecVTX;
	vector<UINT> vecIDX;
	
	int iSliceCount = 64;
	float fRadius = 0.5f;	
	float fThetaStep = XM_2PI / (float)iSliceCount;
	
	// 중심 정점
	VTX vtx;
	vtx.vPos = Vec3(0.f, 0.f, 0.f);
	vtx.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	vecVTX.push_back(vtx);

	// 테두리 정점
	for (int i = 0; i < iSliceCount; ++i)
	{
		vtx.vPos = Vec3(0.5f * cos(fThetaStep * i), 0.5f * sin(fThetaStep * i), 0.f);
		vtx.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		vecVTX.push_back(vtx);
	}	

	// 인덱스 버퍼 만들기
	for (int i = 0; i < iSliceCount-1; ++i)
	{
		vecIDX.push_back(0);
		vecIDX.push_back(i + 2);
		vecIDX.push_back(i + 1);
	}

	vecIDX.push_back(0);
	vecIDX.push_back(1);
	vecIDX.push_back(iSliceCount);
	
    pMesh = CMesh::Create( &vecVTX[0], sizeof(VTX), (UINT)vecVTX.size(), D3D11_USAGE_DEFAULT
		, &vecIDX[0], sizeof(UINT), (UINT)vecIDX.size(), D3D11_USAGE_DEFAULT);

	AddResource<CMesh>(L"CircleMesh", pMesh);

	//======================
	// Collider Circle 메쉬
	//======================
	vecVTX.clear();
	vecIDX.clear();	
	
	// 테두리 정점
	for (int i = 0; i < iSliceCount; ++i)
	{
		vtx.vPos = Vec3(0.5f * cos(fThetaStep * i), 0.5f * sin(fThetaStep * i), 0.f);
		vtx.vColor = Vec4(1.f, 0.f, 0.f, 1.f);
		vecVTX.push_back(vtx);
	}

	// 인덱스 버퍼 만들기
	for (int i = 0; i < iSliceCount; ++i)
	{
		vecIDX.push_back(i);		
	}
	vecIDX.push_back(0);

	pMesh = CMesh::Create(&vecVTX[0], sizeof(VTX), (UINT)vecVTX.size(), D3D11_USAGE_DEFAULT
		, &vecIDX[0], sizeof(UINT), (UINT)vecIDX.size(), D3D11_USAGE_DEFAULT, D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	AddResource<CMesh>(L"ColliderCircleMesh", pMesh);
}

void CResMgr::CreateDefaultMaterial()
{
	CMaterial* pMtrl = nullptr;

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"ColorShader"));
	AddResource<CMaterial>(L"ColorMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"ColliderShader"));
	AddResource<CMaterial>(L"ColliderMtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Std2DShader"));
	AddResource<CMaterial>(L"Std2DMtrl", pMtrl);


	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"Std_EFFECT_Shader"));
	AddResource<CMaterial>(L"Std_EFFECT_Mtrl", pMtrl);

	pMtrl = new CMaterial;
	pMtrl->SetShader(CShaderMgr::GetInst()->FindShader(L"FADE_Shader"));
	AddResource<CMaterial>(L"FADE_Mtrl", pMtrl);

}
