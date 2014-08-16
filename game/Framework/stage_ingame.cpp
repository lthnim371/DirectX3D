#include "stdafx.h"
#include "stage_ingame.h"
#include <fstream>

using namespace framework;

static const UINT MAP_SIZE = 256;

cStage_Ingame::cStage_Ingame()
{
	ZeroMemory(&m_infoSend, sizeof(m_infoSend));
	ZeroMemory(&m_info1, sizeof(m_info1));
	ZeroMemory(&m_info2, sizeof(m_info2));
//	m_access = false;
	fTick1 = 0.f;
	fTick2 = 0.f;
	m_font = NULL;
}
cStage_Ingame::~cStage_Ingame()
{
	SAFE_DELETE(character1);
	SAFE_DELETE(character2);
	SAFE_DELETE(m_shader);
	SAFE_DELETE(m_terrainShader);
	SAFE_DELETE(m_terrain);
	SAFE_RELEASE(m_font);
	SAFE_DELETE(m_hpImage);
	m_sprite->Release();
}

//void cStage_Ingame::Init()
void cStage_Ingame::Init(const int nId)
{
// 그림자 텍스처 생성
	if (FAILED(graphic::GetDevice()->CreateTexture(MAP_SIZE, MAP_SIZE, 1, 
		D3DUSAGE_RENDERTARGET, D3DFMT_A8R8G8B8,
		D3DPOOL_DEFAULT, &m_pShadowTex, NULL)))
		return;
	if (FAILED(m_pShadowTex->GetSurfaceLevel(0, &m_pShadowSurf)))
		return;
	if (FAILED(graphic::GetDevice()->CreateDepthStencilSurface(
		MAP_SIZE, MAP_SIZE, D3DFMT_D24S8, 
		D3DMULTISAMPLE_NONE, 0, TRUE,
		&m_pShadowTexZ, NULL)))
		return;

//폰트 생성
	HRESULT hr = D3DXCreateFontA( graphic::GetDevice(), 50, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
	DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "굴림", &m_font );

//스프라이트 생성
	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);
	m_hpImage = new graphic::cSprite( m_sprite, 0, "Hp_Back" );
	m_hpImage->Create("../media/image/HP_back.png");
	m_hpImage->SetPos( Vector3(10.f, 600.f, 0.f) );
	graphic::cSprite* pHpImage2 = new graphic::cSprite( m_sprite, 1, "Hp_Front" );
	pHpImage2->Create("../media/image/HP_front.png");
	pHpImage2->SetPos( Vector3(0.f, 0.f, 0.f) );
	m_hpImage->InsertChild( pHpImage2 );

//바닥 생성
	m_terrain = new graphic::cTerrain();
	m_terrain->CreateTerrain(128, 128, 100.f);
	m_terrain->CreateTerrainTexture( "../media/texture/map/Grassbland01_T.tga" );
	m_terrainShader = new graphic::cShader();
//	m_terrainShader->Create( "../media/shader/hlsl_terrain_splatting.fx", "TShader" );
	m_terrainShader->Create( "../media/shader/hlsl_rigid_phong.fx", "TShader" );
	LoadMapObject( "../media/mapobject.map" );

	m_infoSend.nId = nId;  //사용자 식별
//	m_info2.nId = ( nId == 0 ? 1 : 0 );

	m_shader = new graphic::cShader();
	m_shader->Create( "../media/shader/hlsl_skinning_using_texcoord.fx", "TShader" );
	
	character1 = new graphic::cCharacter(1);
	character2 = new graphic::cCharacter(2);

	//character1
		character1->Create( "..\\media\\mesh\\valle\\valle_character1.dat" );
		character1->LoadWeapon( "..\\media\\mesh\\valle\\valle_weapon1.dat" );
	//test
	//	character1->SetRenderBoundingBox(true);

		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_backward.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_dash.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LA.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LLA.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LLLA.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LRA.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LLRA.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_RA.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_RRA.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_RRRA.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_jump1.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_jump2.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_jump3.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_JLA.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back1.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front1.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_guard_loop.ani" );
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );

		//Matrix44 rot;
		//rot.SetRotationY( -1.f );
		Matrix44 pos;
		pos.SetTranslate( Vector3( 0, 0, -1500.f) );
		character1->MultiplyTM( pos);
	//	character1->GetCamera()->Init( character1->GetTM().GetPosition() );

	//character2
		character2->Create( "..\\media\\mesh\\valle\\valle_character1.dat" );
		character2->LoadWeapon( "..\\media\\mesh\\valle\\valle_weapon1.dat" );

		character2->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
	
	//debug
		character2->SetAnimation( "..\\media\\ani\\valle\\valle1_guard_loop.ani" );
		character2->SetMode( character2->GUARD );

		//Matrix44 rot;
		//rot.SetRotationY( 180.f );
		pos.SetTranslate( Vector3( 0, 0, 1500.f) );
		character2->MultiplyTM( pos);
	//	character2->GetCamera()->SetPosition( character2->GetTM() );

	//	graphic::cCharacter* pMe = ( m_infoSend.nId == 1 ? character1 : character2 );
	//사용자를 식별하여 해당캐릭터 위치로 카메라 셋팅 -> 그러나 굳이 구분할 필요가 없을듯하다
//		if( m_infoSend.nId == 1 )
//		{
			Vector3 characterPos( character1->GetTM().GetPosition() );
			character1->GetCamera()->Init( characterPos , characterPos + Vector3(0, 300.f, 300.f) );
//		}
//		else if( m_infoSend.nId == 2 )
//		{
			characterPos = Vector3( character2->GetTM().GetPosition() );
			character2->GetCamera()->Init( characterPos , characterPos + Vector3(0, 300.f, 300.f) );
//		}

//초기 마우스위치 값 보관
	::GetCursorPos( &m_currMouse );
	::ScreenToClient( GetStageMgr()->GetWindowHandle(), &m_currMouse );
}

//void cStage_Ingame::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ingame::Input(const float elapseTime)
{
//	if( !m_access )
//		GetStageMgr()->SetSocket();

//패킷 전송에 필요한 변수들 생성 및 초기화
	POINT ptMouse;
	ptMouse.x = 0;
	ptMouse.y = 0;
	network::PROTOCOL::TYPE nState1 = network::PROTOCOL::NONE;
	network::PROTOCOL::TYPE nState2 = network::PROTOCOL::NONE;

//회전 확인
	m_prevMouse = m_currMouse;
	::GetCursorPos( &m_currMouse );
	::ScreenToClient( GetStageMgr()->GetWindowHandle(), &m_currMouse );
	if( m_currMouse.x != m_prevMouse.x || m_currMouse.y != m_prevMouse.y )
	{
		//character1->Action( character1->ROTATION, m_currMouse.x );
		ptMouse.x = m_currMouse.x - m_prevMouse.x;
		ptMouse.y = m_currMouse.y - m_prevMouse.y;
	//	character1->Update( character1->ROTATION, (float)ptMouse.x, (float)ptMouse.y );
	//	pMe->Update( pMe->ROTATION, (float)ptMouse.x, (float)ptMouse.y );
	//	nState1 = network::PROTOCOL::ROTATION;
		if( ptMouse.x > 0 )
			nState1 = network::PROTOCOL::RIGHTROTATION;
		else if( ptMouse.x < 0 )
			nState1 = network::PROTOCOL::LEFTROTATION;
	}
	
/*	//카메라 높이 조절(프로그램 테스트)
//	else if( InputMgr->isOnceKeyDown('1') )
	if( InputMgr->isOnceKeyDown('1') )
	{
		graphic::GetCamera()->SetHeight(-10.f);
	}
	else if( InputMgr->isOnceKeyDown('2') )
	{
		graphic::GetCamera()->SetHeight(10.f);
	}*/

	if( InputMgr->isOnceKeyDown( VK_LBUTTON ) )
	{
		nState2 = network::PROTOCOL::LATTACK;
	//	pMe->Update( pMe->LATTACK );
	//	character1->Update( character1->LATTACK );
	}
	else if( InputMgr->isOnceKeyDown( VK_RBUTTON ) )
	{
		nState2 = network::PROTOCOL::RATTACK;
	//	pMe->Update( pMe->RATTACK );
	//	character1->Update( character1->RATTACK );
	}
	else if( InputMgr->isStayKey('W') )
	{	
		if( InputMgr->isStayKey(VK_SHIFT) )
			nState2 = network::PROTOCOL::DASH;
		//	pMe->Update( pMe->DASH );
		//	character1->Update( character1->DASH );
		else if( InputMgr->isOnceKeyDown(VK_SPACE) )
			nState2 = network::PROTOCOL::FRONTJUMP;
		//	pMe->Update( pMe->FRONTJUMP );
		//	character1->Update( character1->FRONTJUMP );
		else
			nState2 = network::PROTOCOL::FORWARD;
		//	pMe->Update( pMe->FORWARD );
		//	character1->Update( character1->FORWARD );
	}
	else if( InputMgr->isStayKey('S') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			nState2 = network::PROTOCOL::BACKJUMP;
		//	pMe->Update( pMe->BACKJUMP );
		//	character1->Update( character1->BACKJUMP );
		else
			nState2 = network::PROTOCOL::BACKWARD;
		//	pMe->Update( pMe->BACKWARD );
		//	character1->Update( character1->BACKWARD );
	}
	else if( InputMgr->isStayKey('A') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			nState2 = network::PROTOCOL::LEFTJUMP;
		//	pMe->Update( pMe->LEFTJUMP );
		//	character1->Update( character1->LEFTJUMP );
		else
			nState2 = network::PROTOCOL::LEFTWARD;
		//	pMe->Update( pMe->LEFTWARD );
		//	character1->Update( character1->LEFTWARD );
	}
	else if( InputMgr->isStayKey('D') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			nState2 = network::PROTOCOL::RIGHTJUMP;
		//	pMe->Update( pMe->RIGHTJUMP );
		//	character1->Update( character1->RIGHTJUMP );
		else
			nState2 = network::PROTOCOL::RIGHTWARD;
		//	pMe->Update( pMe->RIGHTWARD );
		//	character1->Update( character1->RIGHTWARD );
	}
	else if( InputMgr->isOnceKeyDown(VK_SPACE) )
	{	
		nState2 = network::PROTOCOL::JUMP;
	//	pMe->Update( pMe->JUMP );
	//	character1->Update( character1->JUMP );
	}
	else if( InputMgr->isStayKey('E') )
	{	
		nState2 = network::PROTOCOL::GUARD;
	//	pMe->Update( pMe->JUMP );
	//	character1->Update( character1->JUMP );
	}
	else
	{
		nState2 = network::PROTOCOL::NORMAL;
	//	nState2 = network::PROTOCOL::NONE;
	//	pMe->Update( pMe->NORMAL );
	//	character1->Update( character1->NORMAL );
	}

//이전 전송된 패킷과 다를 경우에만 전송
	if( nState2 != m_infoSend.header2.protocol || nState1 != m_infoSend.header1.protocol )
	{
		PacketSend(nState1, nState2, ptMouse);
	}
}

//void cStage_Ingame::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ingame::Update(const float elapseTime)
{
//	graphic::GetRenderer()->Update( elapseTime );
//	fTick2 += elapseTime;

//	if(fTick2 >= 0.2f)
//	{
//		fTick2 = 0.f;
	//서버로부터 패킷 받아오기
		network::InfoProtocol packetRecv;
		ZeroMemory(&packetRecv, sizeof(packetRecv));
		if( PacketReceive(packetRecv) )  //패킷을 받아왔다면..
		{
//			graphic::cCharacter* pMe = ( packetRecv.nId != 1 ? character1 : character2 );
//			pMe->GetCamera()->SetCamera( packetRecv.camLook, packetRecv.camPos );

				if( packetRecv.nId == 1 )  //사용자 식별번호가 1번이라면..
				{
					m_info1 = packetRecv;  //받아온 패킷 보관해두기
					if( m_infoSend.nId != m_info1.nId )  //받아온 식별번호와 지금 실행하고 있는 사용자와 다르다면...(즉, 현재 사용자가 2번이라면..)
					{
					//1번캐릭터 위치 갱신
						character1->GetCamera()->SetCamera( m_info1.camLook, m_info1.camPos );
						character1->SetTM( m_info1.character );
					}
				}
				else if( packetRecv.nId == 2 )  //사용자 식별번호가 2번이라면..
				{
					m_info2 = packetRecv;
					if( m_infoSend.nId != m_info2.nId )
					{
						character2->GetCamera()->SetCamera( m_info2.camLook, m_info2.camPos );
						character2->SetTM( m_info2.character );
					}
				}  //if( packetRecv.nId == 1 )
		
			//받아온 패킷정보로 해당 캐릭터를 새롭게 갱신하고 다른 캐릭터는 이전 패킷정보 그대로 다시 적용
				character1->Update( m_info1.header1.protocol, (float)m_info1.ptMouse.x, (float)m_info1.ptMouse.y );
				character1->Update( m_info1.header2.protocol );
				character2->Update( m_info2.header1.protocol, (float)m_info2.ptMouse.x, (float)m_info2.ptMouse.y );
				character2->Update( m_info2.header2.protocol );
	/*
				if( packetRecv.header1.protocol == network::PROTOCOL::ROTATION )
					pMe->Update( pMe->ROTATION, (float)packetRecv.ptMouse.x, (float)packetRecv.ptMouse.y );
				if( packetRecv.header2.protocol != network::PROTOCOL::NONE )
					pMe->Update( packetRecv.header2.protocol );
*/
		}
		else  //받아올 패킷이 없었다면..
		{			
		//1번, 2번 캐릭터 각각 공격모드인 경우에만 다시 업데이트하고 그 외의 상태일 경우 무시
			if( character1->GetMode() < character1->LATTACK )
			{
				character1->Update( m_info1.header1.protocol, (float)m_info1.ptMouse.x, (float)m_info1.ptMouse.y );
				character1->Update( m_info1.header2.protocol );
			}
			if( character2->GetMode() < character2->LATTACK )
			{
				character2->Update( m_info2.header2.protocol, (float)m_info2.ptMouse.x, (float)m_info2.ptMouse.y );
				character2->Update( m_info2.header2.protocol );
			}
		}  //if( PacketReceive(packetRecv) )

//	}

	ObjectCollisionCheck();

//새롭게 갱신된 정보대로 적용시킴
	bool bAniState1 = character1->Move(elapseTime);
	bool bAniState2 = character2->Move(elapseTime);
//	graphic::GetCamera()->SetPosition( character1->GetTM() );
	
	//현재 사용자가 공격이 적중할 순간일 떄 사용자의 무기와 상대방의 몸통이 충돌하였는지 확인
		if( character1->GetCubeCheck() == true )  //공격이 적중할 순간이라면..
		{
		//상대방을 기준으로 충돌을 확인
			if( true == character2->CollisionCheck1( *(character1->GetWeaponCube()), character1->GetCamera()->GetLook() ) )
			{
				character1->SetAttackSuccess();  //상대방이 맞았다면 상태 셋팅
			}
		}
		else if( character2->GetCubeCheck() == true )
		{
			if ( true == character1->CollisionCheck1( *(character2->GetWeaponCube()), character2->GetCamera()->GetLook() ) )
			{
				character2->SetAttackSuccess();
			}
		}  //if( character1->GetCubeCheck() == true )

	//현재 사용자가 공격상태일 때 사용자의 몸통과 상대방의 몸통이 충돌하였는지 확인
		if( character1->GetMode() == character1->LATTACK ||
			character1->GetMode() == character1->RATTACK)
		{
			if( true == character2->CollisionCheck2( *(character1->GetCharacterCube()), character1->GetCamera()->GetLook(), character1->GetCamera()->GetDirection() ) )
				character2->UpdateBeHit( bAniState2, character1->GetCamera()->GetDirection(), character1->GetAniPosGap()  );
		}
		else if( character2->GetMode() == character2->LATTACK ||
			character2->GetMode() == character2->RATTACK)
		{
			if( true == character1->CollisionCheck2( *(character2->GetCharacterCube()), character2->GetCamera()->GetLook(), character2->GetCamera()->GetDirection() ) )
				character1->UpdateBeHit( bAniState1, character2->GetCamera()->GetDirection(), character2->GetAniPosGap() );
		}  //if( character1->GetMode() == character1->BEHIT )
}

//void cStage_Ingame::Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ingame::Render(const float elapseTime)
{
		graphic::cCharacter* pMe = ( m_infoSend.nId == 1 ? character1 : character2 );
		pMe->GetCamera()->SetView();

	//fps 및 그리드 출력
//		graphic::GetRenderer()->RenderGrid();
/*
		m_terrainShader->SetMatrix( "mVP", pMe->GetCamera()->GetView() * pMe->GetCamera()->GetProjection() );
		m_terrainShader->SetVector( "vLightDir", Vector3(0,-1,0) );
		m_terrainShader->SetVector( "vEyePos", pMe->GetCamera()->GetPosition() );
		m_terrainShader->SetRenderPass(1);
		m_terrain->RenderShader( *m_terrainShader, pMe->GetCamera() );
*/
		graphic::GetRenderer()->RenderAxis();
		graphic::GetRenderer()->RenderFPS();

//		character1->Render();
//		character2->Render();

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

		Vector3 pos = pMe->GetCamera()->GetLook();
		Vector3 light = Vector3(500,1000,0);

		Matrix44 matView;// 뷰 행렬
		matView.SetView2( light, pos, Vector3(0,1,0));

		Matrix44 matProj;// 투영 행렬
		matProj.SetProjection( D3DX_PI/2.5f, 1, 0.1f, 5000);

		m_shader->SetMatrix( "mVP", matView * matProj);
		m_shader->SetVector( "vLightDir", Vector3(0,-1,0) );
		m_shader->SetVector( "vEyePos", pMe->GetCamera()->GetPosition());

		m_shader->SetRenderPass(1);
		character1->RenderShader( *m_shader );
		character2->RenderShader( *m_shader );

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

#if 1 // 디버그용 텍스처 출력
		{
			graphic::GetDevice()->SetTextureStageState(0,D3DTSS_COLOROP,	D3DTOP_SELECTARG1);
			graphic::GetDevice()->SetTextureStageState(0,D3DTSS_COLORARG1,	D3DTA_TEXTURE);
			graphic::GetDevice()->SetTextureStageState(1,D3DTSS_COLOROP,    D3DTOP_DISABLE);
			float scale = 128.0f;
			typedef struct {FLOAT p[4]; FLOAT tu, tv;} TVERTEX;

			TVERTEX Vertex[4] = {
				// x  y  z rhw tu tv
				{0, scale, 0, 1, 0, 0,},
				{scale, scale,0, 1, 1, 0,},
				{scale, scale+scale,0, 1, 1, 1,},
				{0, scale+scale,0, 1, 0, 1,},
			};
			graphic::GetDevice()->SetTexture( 0, m_pShadowTex );
			graphic::GetDevice()->SetVertexShader(NULL);
			graphic::GetDevice()->SetFVF( D3DFVF_XYZRHW | D3DFVF_TEX1 );
			graphic::GetDevice()->SetPixelShader(0);
			graphic::GetDevice()->DrawPrimitiveUP( D3DPT_TRIANGLEFAN, 2, Vertex, sizeof( TVERTEX ) );
		}
#endif

	//test
/*		Matrix44 VP;
		VP = pMe->GetCamera()->GetView() * pMe->GetCamera()->GetProjection();
		m_shader->SetMatrix( "mVP", VP );
		m_shader->SetVector( "vLightDir", Vector3(0,-1,0) );  //hlsl에 디폴트 값으로 되어있음
		m_shader->SetVector( "vEyePos", pMe->GetCamera()->GetPosition() );
*/
//		character1->RenderShader( *m_shader );
//		character2->RenderShader( *m_shader );		
		
		graphic::cSprite* pImg = dynamic_cast<graphic::cSprite*>(m_hpImage->GetChildren()[0]);
		pImg->SetRect( sRect( 0, 0, (int)( (pMe->GetHP() * 0.01f) * 256 ), 64 ) );
		m_hpImage->Render( Matrix44() );

		if( m_font )
		{
			char buff[16];
			::_itoa_s( pMe->GetHP(), buff, sizeof(buff), 10 );
			string str("HP : ");
			str.append( buff );
			sRect rect(10,660,110,840);
			m_font->DrawTextA( NULL, str.c_str(), -1, (RECT*)&rect,
				DT_NOCLIP, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );

			::_itoa_s( pMe->GetSP(), buff, sizeof(buff), 10 );
			str.assign("SP : ");
			str.append( buff );
			rect.SetX(1050);
			rect.SetY(660);
			m_font->DrawTextA( NULL, str.c_str(), -1, (RECT*)&rect,
				DT_NOCLIP, D3DXCOLOR( 1.0f, 0.2f, 0.0f, 1.0f ) );


			ID3DXFont* font = NULL;
			HRESULT hr = D3DXCreateFontA( graphic::GetDevice(), 20, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
			DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "굴림", &font );
			::_itoa_s( (int)pMe->GetCamera()->GetPosition().y, buff, sizeof(buff), 10 );
			str.assign("camHeight : ");
			str.append( format(buff) );
			rect.SetX(10);
			rect.SetY(30);
			font->DrawTextA( NULL, str.c_str(), -1, &rect,
				DT_NOCLIP, D3DXCOLOR( 0.0f, 0.0f, 1.0f, 1.0f ) );
			SAFE_RELEASE(font);
		}
}

bool cStage_Ingame::PacketSend(const network::PROTOCOL::TYPE nState1, const network::PROTOCOL::TYPE nState2, const POINT ptMouse)
//bool cStage_Ingame::PacketSend(const int nState1, const int nState2, const POINT ptMouse)
//bool cStage_Ingame::PacketSend(const network::InfoProtocol packetInfo)
{
	graphic::cCharacter* pMe = ( m_infoSend.nId == 1 ? character1 : character2 );

	m_infoSend.header1.protocol = nState1;
	m_infoSend.header2.protocol = nState2;
//	m_infoSend.nId = m_id;
	m_infoSend.ptMouse = ptMouse;
	m_infoSend.camLook = pMe->GetCamera()->GetLook();
	m_infoSend.camPos = pMe->GetCamera()->GetPosition();
	m_infoSend.character = pMe->GetTM();

	char buff[ 128];  //배열 크기는 미리 정해진 약속이어야 한다. 즉, 채우려는 데이터가 적어도 배열크기로 다 채워야한다.
	ZeroMemory(buff, sizeof(buff));
	memcpy(buff, &m_infoSend, sizeof(m_infoSend));  //데이터 채우기

	const int result = send( GetStageMgr()->GetSocket(), buff, sizeof(buff), 0 );
	if (result == INVALID_SOCKET)
	{	
		//if( false == GetStageMgr()->SetSocket() )
			return false;		
	}

	return true;
}

bool cStage_Ingame::PacketReceive(OUT network::InfoProtocol& packetInfo)
{
//	const timeval t = {0, 10}; // 10 millisecond
	const timeval t = {0, 1};  //select 함수가 얼마나 대기해서 패킷을 기다릴지를 나타내는 변수입니다.
	fd_set readSockets;
	FD_ZERO(&readSockets);
	FD_SET(GetStageMgr()->GetSocket(), &readSockets);

	const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t );  //서버로부터 패킷이 왔는지 판단 (= peekmessage와 비슷)
	if (ret != 0 && ret != SOCKET_ERROR)
	{
		char buff[ 128];
		const int result = recv( readSockets.fd_array[ 0], buff, sizeof(buff), 0);  //서버로부터 정보를 받는다. 만약 오는 정보가 없으면 계속 기다린다 //readSockets(소켓)가 있으면 어떤 서버 혹은 클라이언트와 연결되었는지 알 수 있다.
		if (result == SOCKET_ERROR || result == 0) // 받은 패킷사이즈가 0이면 서버와 접속이 끊겼다는 의미다.
		{
			GetStageMgr()->Release();
			//if( false == GetStageMgr()->SetSocket() )
				return false;
		}
		else
		{
		//	ParsePacket(buff);  //패킷이 왔다면 호출

			const network::InfoProtocol* protocol = (network::InfoProtocol*)buff;
			packetInfo.nId = protocol->nId;
			packetInfo.header1 = protocol->header1;
			packetInfo.header2 = protocol->header2;
			packetInfo.ptMouse.x = protocol->ptMouse.x;
			packetInfo.ptMouse.y = protocol->ptMouse.y;
			packetInfo.camLook = protocol->camLook;
			packetInfo.camPos = protocol->camPos;
			packetInfo.character = protocol->character;

			return true;
		}
	}

	return false;
}

void cStage_Ingame::LoadMapObject(const string& fileName)
{
	using namespace std;

	ifstream fs;
	fs.open(fileName, ios_base::binary);

	int objectCount = 0;
	fs.read((char*)&objectCount, sizeof(int));

	vector<graphic::cModel*>& rTerrainObj = m_terrain->GetRigidModels();

	for(int i=0; i<objectCount; ++i)
	{
		string fileName;
		Matrix44 tm;
		int nSize = 0;
		fs.read((char*)&nSize, sizeof(int));
		char* cFileName = new char[nSize + 1];
	//	fs.read((char*)&fileName, nSize);
		fs.read(cFileName, nSize + 1);
		fs.read((char*)&tm, sizeof(Matrix44));
		
		fileName.assign( cFileName );
		fileName.erase(0, 3);
		delete[] cFileName;

		graphic::cModel* pNewObj = new graphic::cModel( i );
		pNewObj->Create( fileName, graphic::MODEL_TYPE::RIGID );
		pNewObj->SetTM( tm );
		pNewObj->CreateCube();

//		m_terrain->AddRigidModel( *pNewObj );
//		delete pNewObj;

		rTerrainObj.push_back( pNewObj );
	}
}

void cStage_Ingame::ObjectCollisionCheck()
{
	vector<graphic::cModel*>& rTerrainObj = m_terrain->GetRigidModels();  //맵오브젝트 목록 가져오기
	for(auto it = rTerrainObj.begin(); it != rTerrainObj.end(); ++it)  //전부 확인
	{			
		Vector3 objHalfDis = (*it)->GetCube().GetMax();  //해당 오브젝트의 절반 크기 가져오기(길이or넓이)

		if( objHalfDis.y <= 15.f )  //만약 충돌확인이 필요하지 않은 오브젝트라면 패스
			continue;
	//확인하는데 필요한 변수들 생성 및 초기화
		Vector3 objPos = (*it)->GetTM().GetPosition();
		Vector3 up(0.f, 1.f, 0.f);
		Vector3 cam1_newLook = character1->GetCamera()->GetLook();
		Vector3 cam2_newLook = character2->GetCamera()->GetLook();
		Vector3 distance1 = objPos - cam1_newLook;
		Vector3 distance2 = objPos - cam2_newLook;
		distance1.y = 0.f;
		distance2.y = 0.f;
		objHalfDis.y = 0.f;
				
		Vector3 cam1_newPos( character1->GetCamera()->GetPosition() );
		cam1_newPos.y = 100.f;
		cam1_newLook.y = cam1_newPos.y;
		Vector3 cam1_newDir = cam1_newLook - cam1_newPos;
		cam1_newDir.Normalize();
		Vector3 cam1_newRight( up.CrossProduct(cam1_newDir) );

		Vector3 cam2_newPos( character2->GetCamera()->GetPosition() );
		cam2_newPos.y = 100.f;
		cam2_newLook.y = cam2_newPos.y;
		Vector3 cam2_newDir = cam2_newLook - cam2_newPos;
		cam2_newDir.Normalize();
		Vector3 cam2_newRight( up.CrossProduct(cam2_newDir) );

		float fT = 0.f;

		if( distance1.Length() <= objHalfDis.Length() )  //1번 캐릭터와 오브젝트와의 거리 확인
		{			
			switch( character1->GetMode() )
			{
			case character1->FORWARD:
				cam1_newLook += Vector3( cam1_newDir.x, 0.f, cam1_newDir.z ) * -5.f;  //Update에서 이미 이동하였기때문에 다시 이전값으로 돌려서 검사해야 함
				if( (*it)->Pick( cam1_newLook, cam1_newDir, &fT ) )  //해당 오브젝트에 현재 캐릭터 위치를 이용하여 픽킹레이 확인
				{
				//	if( 0 <= fT && fT <= 10.f) {}
					if( fT < 0.f )  //오브젝트 안에서 빠져나오지 못하는 문제를 방지
						break;
					//충돌 확인이 되었으면 캐릭터 위치를 되돌리기
						Matrix44 matT;
						matT.SetTranslate( Vector3( cam1_newDir.x, 0.f, cam1_newDir.z ) * -5.f );  //카메라가 바라보는 방향으로
						character1->MultiplyTM( matT );  //현재 위치에 더해주기
						character1->GetCamera()->SetPosition( character1->GetTM() );  //카메라 위치도 갱신
								
				}
				break;

			case character1->BACKWARD:
				cam1_newLook += Vector3( cam1_newDir.x, 0.f, cam1_newDir.z ) * 5.f;
				if( (*it)->Pick( cam1_newLook, -cam1_newDir, &fT ) )
				{
					if( fT < 0.f )
						break;
					
						Matrix44 matT;
						matT.SetTranslate( Vector3( cam1_newDir.x, 0.f, cam1_newDir.z ) * 5.f );  //카메라가 바라보는 방향으로
						character1->MultiplyTM( matT );  //현재 위치에 더해주기
						character1->GetCamera()->SetPosition( character1->GetTM() );  //카메라 위치도 갱신
				
				}
				break;

			case character1->LEFTWARD:
				cam1_newLook += Vector3( cam1_newRight.x, 0.f, cam1_newRight.z ) * 5.f;
				if( (*it)->Pick( cam1_newLook, -cam1_newRight, &fT ) )
				{
					if( fT < 0.f )
						break;
				
						Matrix44 matT;
						matT.SetTranslate( Vector3( cam1_newRight.x, 0.f, cam1_newRight.z ) * 5.f );  //카메라가 바라보는 방향으로
						character1->MultiplyTM( matT );  //현재 위치에 더해주기
						character1->GetCamera()->SetPosition( character1->GetTM() );  //카메라 위치도 갱신
				
				}
				break;

			case character1->RIGHTWARD:
				cam1_newLook += Vector3( cam1_newRight.x, 0.f, cam1_newRight.z ) * -5.f;
				if( (*it)->Pick( cam1_newLook, cam1_newRight, &fT ) )
				{
					if( fT < 0.f )
						break;
					
						Matrix44 matT;
						matT.SetTranslate( Vector3( cam1_newRight.x, 0.f, cam1_newRight.z ) * -5.f );  //카메라가 바라보는 방향으로
						character1->MultiplyTM( matT );  //현재 위치에 더해주기
						character1->GetCamera()->SetPosition( character1->GetTM() );  //카메라 위치도 갱신
				
				}
				break;
			
			case character1->LATTACK:
			case character1->RATTACK:
				if( (*it)->Pick( cam1_newLook, cam1_newDir, &fT ) )
				{
				//	if( character1->GetBoneMgr()->GetRoot()->GetMoveControl() == false )
						character1->MoveControl( true );  //적용하고 있는 애니의 이동값이 적용되는 것을 막는다.
				}
				break;

			case character1->NORMAL:
			case character1->ROTATION:
			case character1->GUARD:
			case character1->GUARD_BE_HIT:
			case character1->BEHIT:
				if( (*it)->Pick( cam2_newLook, cam2_newDir, &fT ) )  //상대방한테 맞고 있을 경우 적의 위치와 방향으로 확인하여
				{
					if( fT < 37.f )  //현재 사용자 캐릭터가 오브젝트에 닿아 있는 공간만큼의 (예상)일정 수치 내에 상대방이 있을 경우
						character2->MoveControl( true );  //상대방 캐릭터의 애니 이동값 적용을 막는다. -> 그러면 사용자 캐릭터도 오브젝트에 닿기만 한 상태가 된다.
				}
				break;
			}  //switch( character1->GetMode() )
		}  //if( distance1.Length() <= objHalfDis.Length() )
		else if( distance2.Length() <= objHalfDis.Length() )  //2번 캐릭터도 위와 동일하게 한다.
		{			
			switch( character2->GetMode() )
			{
			case character2->FORWARD:
				cam2_newLook += Vector3( cam2_newDir.x, 0.f, cam2_newDir.z ) * -5.f;
				if( (*it)->Pick( cam2_newLook, cam2_newDir, &fT ) )
				{
				//	if( 0 <= fT && fT <= 20.f) {}
					if( fT < 0.f )
						break;
					
						Matrix44 matT;
						matT.SetTranslate( Vector3( cam2_newDir.x, 0.f, cam2_newDir.z ) * -5.f );  //카메라가 바라보는 방향으로
						character2->MultiplyTM( matT );  //현재 위치에 더해주기
						character2->GetCamera()->SetPosition( character2->GetTM() );  //카메라 위치도 갱신
								
				}
				break;

			case character2->BACKWARD:
				cam2_newLook += Vector3( cam2_newDir.x, 0.f, cam2_newDir.z ) * 5.f;
				if( (*it)->Pick( cam2_newLook, -cam2_newDir, &fT ) )
				{
					if( fT < 0.f )
						break;
					
						Matrix44 matT;
						matT.SetTranslate( Vector3( cam2_newDir.x, 0.f, cam2_newDir.z ) * 5.f );  //카메라가 바라보는 방향으로
						character2->MultiplyTM( matT );  //현재 위치에 더해주기
						character2->GetCamera()->SetPosition( character2->GetTM() );  //카메라 위치도 갱신
				
				}
				break;

			case character2->LEFTWARD:
				cam2_newLook += Vector3( cam2_newRight.x, 0.f, cam2_newRight.z ) * 5.f;
				if( (*it)->Pick( cam2_newLook, -cam2_newRight, &fT ) )
				{
					if( fT < 0.f )
						break;
				
						Matrix44 matT;
						matT.SetTranslate( Vector3( cam2_newRight.x, 0.f, cam2_newRight.z ) * 5.f );  //카메라가 바라보는 방향으로
						character2->MultiplyTM( matT );  //현재 위치에 더해주기
						character2->GetCamera()->SetPosition( character2->GetTM() );  //카메라 위치도 갱신
				
				}
				break;

			case character2->RIGHTWARD:
				cam2_newLook += Vector3( cam2_newRight.x, 0.f, cam2_newRight.z ) * -5.f;
				if( (*it)->Pick( cam2_newLook, cam2_newRight, &fT ) )
				{
					if( fT < 0.f )
						break;
					
						Matrix44 matT;
						matT.SetTranslate( Vector3( cam2_newRight.x, 0.f, cam2_newRight.z ) * -5.f );  //카메라가 바라보는 방향으로
						character2->MultiplyTM( matT );  //현재 위치에 더해주기
						character2->GetCamera()->SetPosition( character2->GetTM() );  //카메라 위치도 갱신
				
				}
				break;
			
			case character2->LATTACK:
			case character2->RATTACK:
				if( (*it)->Pick( cam2_newLook, cam2_newDir, &fT ) )
				{
				//	if( character2->GetBoneMgr()->GetRoot()->GetMoveControl() == false )
						character2->MoveControl( true );
				}
				break;

			case character2->NORMAL:
			case character2->ROTATION:
			case character2->GUARD:
			case character2->GUARD_BE_HIT:
			case character2->BEHIT:
				if( (*it)->Pick( cam1_newLook, cam1_newDir, &fT ) )
				{
					if( fT < 37.f )
						character1->MoveControl( true );
				}
				break;
			}  //switch( character2->GetMode() )
		}  //if( distance2.Length() <= objHalfDis.Length() )
	}
}

/*  Input Backup
	if( InputMgr->isOnceKeyDown( VK_LBUTTON ) )
	{
		pMe->Update( pMe->LATTACK );
		//	character1->Update( character1->LATTACK );
	}
	else if( InputMgr->isOnceKeyDown( VK_RBUTTON ) )
	{
		pMe->Update( pMe->RATTACK );
		//	character1->Update( character1->RATTACK );
	}
	else if( InputMgr->isStayKey('W') )
	{	
		if( InputMgr->isStayKey(VK_SHIFT) )
			pMe->Update( pMe->DASH );
		//	character1->Update( character1->DASH );
		else if( InputMgr->isOnceKeyDown(VK_SPACE) )
			pMe->Update( pMe->FRONTJUMP );
		//	character1->Update( character1->FRONTJUMP );
		else
			pMe->Update( pMe->FORWARD );
		//	character1->Update( character1->FORWARD );
	}
	else if( InputMgr->isStayKey('S') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			pMe->Update( pMe->BACKJUMP );
		//	character1->Update( character1->BACKJUMP );
		else
			pMe->Update( pMe->BACKWARD );
		//	character1->Update( character1->BACKWARD );
	}
	else if( InputMgr->isStayKey('A') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			pMe->Update( pMe->LEFTJUMP );
		//	character1->Update( character1->LEFTJUMP );
		else
			pMe->Update( pMe->LEFTWARD );
		//	character1->Update( character1->LEFTWARD );
	}
	else if( InputMgr->isStayKey('D') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			pMe->Update( pMe->RIGHTJUMP );
		//	character1->Update( character1->RIGHTJUMP );
		else
			pMe->Update( pMe->RIGHTWARD );
		//	character1->Update( character1->RIGHTWARD );
	}
	else if( InputMgr->isOnceKeyDown(VK_SPACE) )
	{	
		pMe->Update( pMe->JUMP );
		//	character1->Update( character1->JUMP );
	}
	else
	{
		pMe->Update( pMe->NORMAL );
		//	character1->Update( character1->NORMAL );
	}
*/