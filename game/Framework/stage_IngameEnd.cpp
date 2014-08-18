#include "stdafx.h"
#include "stage_IngameEnd.h"

using namespace framework;

static const UINT MAP_SIZE = 256;

cStage_IngameEnd::cStage_IngameEnd()
{
}
cStage_IngameEnd::~cStage_IngameEnd()
{
	Release();
}

void cStage_IngameEnd::Init(const int nId, tagIngameInfo* sIngameInfo)
{
	m_user = nId;

	character1 = sIngameInfo->pCharacter1;
	character2 = sIngameInfo->pCharacter2;
	m_shader = sIngameInfo->pShader;
	m_terrain = sIngameInfo->pTerrain;
	m_terrainShader = sIngameInfo->pTerrainShader;
	m_sprite = sIngameInfo->pSprite;
	m_pShadowTex = sIngameInfo->pShadowTex;
	m_pShadowSurf = sIngameInfo->pShadowSurf;
	m_pShadowTexZ = sIngameInfo->pShadowTexZ;
	m_skybox = sIngameInfo->pSkybox;

	if( m_user == 1 )
	{
		m_camDirOriginal = character1->GetCamera()->GetDirection();
		character1->GetBoneMgr()->SetAniLoop(false);
		character1->SetAnimation( sIngameInfo->bResult == true ? 
			"..\\media\\ani\\valle\\valle1_win.ani" : "..\\media\\ani\\valle\\valle1_lose.ani" );
		character2->GetBoneMgr()->SetAniLoop(false);
		character2->SetAnimation( sIngameInfo->bResult == false ? 
			"..\\media\\ani\\valle\\valle1_win.ani" : "..\\media\\ani\\valle\\valle1_lose.ani" );
	}
	else if( m_user == 2 )
	{
		m_camDirOriginal = character2->GetCamera()->GetDirection();
		character2->GetBoneMgr()->SetAniLoop(false);
		character2->SetAnimation( sIngameInfo->bResult == true ? 
			"..\\media\\ani\\valle\\valle1_win.ani" : "..\\media\\ani\\valle\\valle1_lose.ani" );
		character1->GetBoneMgr()->SetAniLoop(false);
		character1->SetAnimation( sIngameInfo->bResult == false ? 
			"..\\media\\ani\\valle\\valle1_win.ani" : "..\\media\\ani\\valle\\valle1_lose.ani" );
	}
}

void cStage_IngameEnd::Release()
{
	SAFE_DELETE(character1);
	SAFE_DELETE(character2);
	SAFE_DELETE(m_shader);
	SAFE_DELETE(m_terrainShader);
	SAFE_DELETE(m_terrain);
	m_sprite->Release();
	m_pShadowTex->Release();
	m_pShadowSurf->Release();
	m_pShadowTexZ->Release();
	SAFE_DELETE(m_skybox);
}

void cStage_IngameEnd::Input(const float elapseTime)
{
}

void cStage_IngameEnd::Update(const float elapseTime)
{
//	graphic::cCharacter* pMe = m_user == 1 ? character1 : character2;

//	if( m_camDirOriginal.DotProduct(pMe->GetCamera()->GetDirection()) < 1 )
//		pMe->GetCamera()-

	if( character1->Move(elapseTime) == false && character2->Move(elapseTime) == false )
	{
		character1->GetBoneMgr()->SetAniLoop(true);
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
		character2->GetBoneMgr()->SetAniLoop(true);
		character2->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
	}
}

void cStage_IngameEnd::Render(const float elapseTime)
{
	m_skybox->Render();

	graphic::cCharacter* pMe = m_user == 1 ? character1 : character2;
	pMe->GetCamera()->SetView();

	graphic::GetRenderer()->RenderFPS();

	//---------------------------------------------------------------
		// 모델 출력 + 그림자.
		LPDIRECT3DSURFACE9 pOldBackBuffer, pOldZBuffer;
		D3DVIEWPORT9 oldViewport;

		graphic::GetDevice()->GetRenderTarget(0, &pOldBackBuffer);
		graphic::GetDevice()->GetDepthStencilSurface(&pOldZBuffer);
		graphic::GetDevice()->GetViewport(&oldViewport);

		graphic::GetDevice()->SetRenderTarget(0, m_pShadowSurf);
		graphic::GetDevice()->SetDepthStencilSurface(m_pShadowTexZ);
		// 뷰포트변경  x y  width    height   minz maxz
		D3DVIEWPORT9 viewport = {0,0, MAP_SIZE,MAP_SIZE,0.0f,1.0f};
		graphic::GetDevice()->SetViewport(&viewport);

		// 그림자맵 클리어
		graphic::GetDevice()->Clear(0L, NULL
			, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER
			, 0x00000000, 1.0f, 0L);

//		Vector3 pos = pMe->GetCamera()->GetLook();
		Vector3 light = Vector3(1111,1111,0);

		Matrix44 matView;// 뷰 행렬
//		matView.SetView2( light, pos, Vector3(0,1,0));
	//test
		matView.SetView2( light, pMe->GetCamera()->GetLook(), Vector3(0,1,0));

		Matrix44 matProj;// 투영 행렬
		matProj.SetProjection( D3DX_PI * 0.37f, 1, 0.1f, 7777);

		m_shader->SetMatrix( "mVP", matView * matProj);
		m_shader->SetVector( "vLightDir", Vector3(0,-1,0) );
		m_shader->SetVector( "vEyePos", pMe->GetCamera()->GetPosition());

		m_shader->SetRenderPass(1);
		character1->RenderShadow( *m_shader );
		character2->RenderShadow( *m_shader );

		//-----------------------------------------------------
		// 렌더링타겟 복구
		//-----------------------------------------------------
		graphic::GetDevice()->SetRenderTarget(0, pOldBackBuffer);
		graphic::GetDevice()->SetDepthStencilSurface(pOldZBuffer);
		graphic::GetDevice()->SetViewport(&oldViewport);
		pOldBackBuffer->Release();
		pOldZBuffer->Release();
		graphic::GetDevice()->SetTransform( D3DTS_VIEW, (D3DXMATRIX*)&( pMe->GetCamera()->GetView() ) );
		graphic::GetDevice()->SetTransform( D3DTS_PROJECTION, (D3DXMATRIX*)&( pMe->GetCamera()->GetProjection() ) );

		m_shader->SetMatrix( "mVP", pMe->GetCamera()->GetView() * pMe->GetCamera()->GetProjection() );
		m_shader->SetRenderPass(0);
		character1->RenderShader( *m_shader );
		character2->RenderShader( *m_shader );

		//------------------------------------------------------------------------
		// 지형 출력.
		//------------------------------------------------------------------------
		D3DXMATRIX mTT;
		mTT = D3DXMATRIX(0.5f, 0.0f, 0.0f, 0.0f
			, 0.0f,-0.5f, 0.0f, 0.0f
			, 0.0f, 0.0f, 1.0f, 0.0f
			, 0.5f, 0.5f, 0.0f, 1.0f);
		Matrix44 mT = *(Matrix44*)&mTT;

		m_terrainShader->SetMatrix( "mVP", pMe->GetCamera()->GetView() * pMe->GetCamera()->GetProjection());
		m_terrainShader->SetVector( "vLightDir", Vector3(0,-1,0) );
		m_terrainShader->SetVector( "vEyePos", pMe->GetCamera()->GetPosition());
		m_terrainShader->SetTexture("ShadowMap", m_pShadowTex);
		
		Matrix44 m = matView * matProj * mT;
		m_terrainShader->SetMatrix( "mWVPT", m );

		m_terrainShader->SetRenderPass(2);
		m_terrain->RenderShader( *m_terrainShader , pMe->GetCamera() );


}
		