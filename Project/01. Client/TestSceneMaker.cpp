#include "stdafx.h"
#include "TestSceneMaker.h"

#include "AnrealEngine/SceneMgr.h"

#include "AnrealEngine/PathMgr.h"
#include "AnrealEngine/Scene.h"
#include "AnrealEngine/CollisionMgr.h"

#include "AnrealEngine/ResMgr.h"
#include "AnrealEngine/Texture.h"
#include "AnrealEngine/Mesh.h"

#include "AnrealEngine/Material.h"
#include "AnrealEngine/Prefab.h"


#include "AnrealEngine/Transform.h"
#include "AnrealEngine/MeshRender.h"
#include "AnrealEngine/Camera.h"
#include "AnrealEngine/Animator2D.h"
#include "AnrealEngine/Collider.h"

#include "SaveLoadMgr.h"

void CTestSceneMaker::init()
{
	//CScene* pCurScene = CSceneMgr::GetInst()->GetCurScene();
	//pCurScene->SetName(L"Temp Scene");
	//


	//// Camera Object 만들기
	//CGameObject* pCamObject = new CGameObject;
	//pCamObject->SetName(L"MainCamera");

	//pCamObject->AddComponent(new CCamera);
	//
	//
	//Vec3 CameraVec3 = { 1920, 1080, 0.f };


	//pCamObject->Camera()->SetPerspective(false);
	//pCamObject->Camera()->CheckLayerAll();
	//pCurScene->AddLayer(L"Camera");
	//pCurScene->AddGameObject(L"Camera", pCamObject);

	////


	////
	//pCurScene->awake();
	//pCurScene->start();

	wstring ScenePath;

	ScenePath = CPathMgr::GetResPath();


//	ScenePath += L"Scene\\RUDIBRIUM.scene";
	ScenePath += L"Scene\\MapleStory_BOSS.scene";

	CSaveLoadMgr::LoadScene(ScenePath);

	return;



//	return;

//	// Layer 추가하기
//	pCurScene->AddLayer(L"Player");
//	pCurScene->AddLayer(L"Monster");
//	pCurScene->AddLayer(L"Map");
//	
//	// 충돌 Layer 지정하기
//	CCollisionMgr::GetInst()->CollisionCheck(L"Player", L"Monster");
//	
//	// 필요한 리소스 로딩하기	
//	Ptr<CTexture> pUniverseTex = CResMgr::GetInst()->Load<CTexture>(L"Universe", L"Texture\\universe.jpg");
//	Ptr<CTexture> pPlayerTex = CResMgr::GetInst()->Load<CTexture>(L"Player_00", L"Texture\\00.png");
//
//
//
//	// Player Object 만들기
//	CGameObject* pObject = new CGameObject;
//	pObject->SetName(L"Player");
//
//	pObject->AddComponent(new CMeshRender);
//	pObject->AddComponent(new CCollider);
//	pObject->AddComponent(new CAnimator2D);
//
//	// Material 값 셋팅
//	Ptr<CMaterial> pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Std2DMtrl");
//	pMtrl->SetData(SHADER_PARAM::TEX_0, pPlayerTex);
//	int i = 1;
//	pMtrl->SetData(SHADER_PARAM::INT_0, &i);
//
//	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
//	pObject->MeshRender()->SetMaterial(pMtrl);
//
//	pObject->Transform()->SetLocalScale(Vec3(200.f, 260.f, 1.f));
//	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 100.f));
//
//	// Collider 값 셋팅
//	pObject->Collider()->SetColliderType(COLLIDER_TYPE::COLLIDER_2D_BOX);
//	pObject->Collider()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
//	//pObject->Collider()->SetOffsetPos(Vec3(1.f, 1.f, 0.f));
//
//	// Animation 추가하기
//	CTexture* pTex = CResMgr::GetInst()->Load<CTexture>(L"Player", L"Texture\\Player.png");
//	//pObject->Animator2D()->AddAnimation(pTex, POINT{ 0, 256 * 6 }, 128, 12, POINT{ 128, 256 }, L"Casting");
//	pObject->Animator2D()->AddAnimation(L"Texture\\Animation2D\\Explosion\\", L"Explosion", 0.02f, true);
//	pObject->Animator2D()->PlayAnim(L"Explosion"/*, true*/);
//
//
//
//
//
//	CScript* pPlayerScript = new CPlayerScript;
//
//	pObject->AddComponent(pPlayerScript);
//
//	pCurScene->AddGameObject(L"Player", pObject);
//
//
//	/*CScript* pPla*/
//
//	// Monster Object 만들기
//	pObject = new CGameObject;
//	pObject->SetName(L"Monster");
//
//	pObject->AddComponent(new CMeshRender);
//	pObject->AddComponent(new CCollider);
//	pObject->AddComponent(new CAnimator2D);
//
//	// Material 값 셋팅
//	pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Std2DMtrl");
//
//	pMtrl->SetData(SHADER_PARAM::TEX_0, pPlayerTex);
//	i = 0;
//	pMtrl->SetData(SHADER_PARAM::INT_0, &i);
//	//수정 해보기
//
//	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
//	pObject->MeshRender()->SetMaterial(pMtrl);
//
//	pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
//	pObject->Transform()->SetLocalPos(Vec3(-500.f, 0.f, 100.f));
//
//	// Collider 값 셋팅
//	pObject->Collider()->SetColliderType(COLLIDER_TYPE::COLLIDER_2D_BOX);
//	pObject->Collider()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
//	pObject->Collider()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
//
//	pCurScene->AddGameObject(L"Monster", pObject);
///*
//
//	pObject = new CGameObject;
//	pObject->SetName(L"Map");
//
//	pObject->AddComponent(new CMeshRender);
//	pObject->AddComponent(new CCollider);
//	pObject->AddComponent(new CAnimator2D);
//
//	pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Std2DMtrl");
//
//	pMtrl->SetData(SHADER_PARAM::TEX_0, pPlayerTex);
//	i = 0;
//	pMtrl->SetData(SHADER_PARAM::INT_0, &i);
//
//	pObject->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
//	pObject->MeshRender()->SetMaterial(pMtrl);
//
//	pObject->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
//	pObject->Transform()->SetLocalPos(Vec3(0.f, 0.f, 500.f));
//
//	pCurScene->AddGameObject(L"Map", pObject);
//
//*/
//
//	// Child Object 만들기
//	CGameObject* pChild = new CGameObject;
//	pChild->SetName(L"ChildObject");
//
//	pChild->AddComponent(new CMeshRender);
//	pChild->AddComponent(new CCollider);
//	pChild->AddComponent(new CAnimator2D);
//
//	// Material 값 셋팅
//	pMtrl = CResMgr::GetInst()->Load<CMaterial>(L"Std2DMtrl");
//
//	pChild->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
//	pChild->MeshRender()->SetMaterial(pMtrl);
//
//	pChild->Transform()->SetLocalScale(Vec3(200.f, 200.f, 1.f));
//	pChild->Transform()->SetLocalPos(Vec3(-500.f, 0.f, 100.f));
//
//	// Collider 값 셋팅
//	pChild->Collider()->SetColliderType(COLLIDER_TYPE::COLLIDER_2D_BOX);
//	pChild->Collider()->SetOffsetScale(Vec3(1.f, 1.f, 1.f));
//	pChild->Collider()->SetOffsetPos(Vec3(0.f, 0.f, 0.f));
//
//	pObject->AddChild(pChild);
//
//	// Prefab 용도 Object 생성
//	CGameObject* pBulletPrefab = new CGameObject;
//
//	pBulletPrefab->AddComponent(new CBulletScript);
//
//	pBulletPrefab->Transform()->SetLocalScale(Vec3(50.f, 50.f, 1.f));
//	pBulletPrefab->AddComponent(new CMeshRender);
//	//pBulletPrefab->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
//	//pBulletPrefab->MeshRender()->SetMaterial(CResMgr::GetInst()->Load<CMaterial>(L"Std2DMtrl"));
//
//	//CResMgr::GetInst()->AddResource<CPrefab>(L"Bullet", new CPrefab(pBulletPrefab));
//





	//CGameObject* pEffectPrefab = new CGameObject;

	//pEffectPrefab->AddComponent(new CEffectScript);
	//pEffectPrefab->AddComponent(new CMeshRender);
	//pEffectPrefab->AddComponent(new CAnimator2D);
	//pEffectPrefab->MeshRender()->SetMesh(CResMgr::GetInst()->Load<CMesh>(L"RectMesh"));
	//pEffectPrefab->MeshRender()->SetMaterial(CResMgr::GetInst()->Load<CMaterial>(L"Std2DMtrl"));
	//pEffectPrefab->AddComponent(new CEffectScript);

	//pCurScene->AddGameObject(L"EFFECT_LAYER", pEffectPrefab);

	//CResMgr::GetInst()->AddResource<CPrefab>(L"EFFECT_OBJECT", new CPrefab(pEffectPrefab));






//	pCurScene->awake();
//	pCurScene->start();
}