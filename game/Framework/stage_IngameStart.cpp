#include "stdafx.h"
#include "stage_IngameStart.h"
#include <fstream>

using namespace framework;

static const UINT MAP_SIZE = 256;

cStage_IngameStart::cStage_IngameStart()
{
}
cStage_IngameStart::~cStage_IngameStart()
{
	Release();
}

void cStage_IngameStart::Release()
{
	SAFE_DELETE(m_image);
}

//void cStage_IngameStart::Init()
void cStage_IngameStart::Init(const int nId, tagIngameInfo* sIngameInfo)
{
	m_start = false;
	m_id = nId;
	SndDepot->get( "MainTema1" )->Stop();
	SndDepot->get( "MainTema2" )->Play();
	GetStageMgr()->SetSoundName( "MainTema2" );


//스카이박스 생성
	m_skybox = new graphic::cModel(7777);
	m_skybox->Create( "../media/mesh/map/skybox.dat", graphic::MODEL_TYPE::RIGID );
	Matrix44 s;
	s.SetScale( Vector3(0.9f, 1.f, 0.9f) );
	m_skybox->MultiplyTM( s );

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
/*
//스프라이트 생성
	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);
	m_readyImage = new graphic::cSprite( m_sprite, 0, "Ready" );
	m_readyImage->Create("../media/image/start01.tga");
	m_readyImage->SetPos( Vector3(384.f, 10.f, 0.f) );

	m_startImage = new graphic::cSprite( m_sprite, 0, "Start" );
	m_startImage->Create("../media/image/start02.tga");
	m_startImage->SetPos( Vector3(384.f, 10.f, 0.f) );
*/
	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);
	m_image = new graphic::cSprite( m_sprite, 0, "Start" );
	m_image->Create("../media/image/start01.tga");
	m_image->SetPos( Vector3(384.f, 10.f, 0.f) );


//바닥 생성
	m_terrain = new graphic::cTerrain();
	m_terrain->CreateTerrain(128, 128, 100.f);
	m_terrain->CreateTerrainTexture( "../media/texture/map/Grassbland01_T.tga" );
	m_terrainShader = new graphic::cShader();
//	m_terrainShader->Create( "../media/shader/hlsl_terrain_splatting.fx", "TShader" );
	m_terrainShader->Create( "../media/shader/hlsl_rigid_phong.fx", "TShader" );
	LoadMapObject( "../media/mapobject.map" );

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
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_guard_hit1.ani" );
	//	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );

	//	character1->SetAniLoop(false);
	//	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_match_start.ani" );

		character1->SetAniLoop(true);
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );

		//Matrix44 rot;
		//rot.SetRotationY( -1.f );
		Matrix44 pos;
		pos.SetTranslate( Vector3( 0, 0, -3000.f) );
		character1->MultiplyTM( pos);
	//	character1->GetCamera()->Init( character1->GetTM().GetPosition() );

	//character2
		character2->Create( "..\\media\\mesh\\valle\\valle_character1.dat" );
		character2->LoadWeapon( "..\\media\\mesh\\valle\\valle_weapon1.dat" );

	//	character2->SetAniLoop(false);
	//	character2->SetAnimation( "..\\media\\ani\\valle\\valle1_match_start.ani" );

		character2->SetAniLoop(true);
		character2->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );

		//Matrix44 rot;
		//rot.SetRotationY( 180.f );
		pos.SetTranslate( Vector3( 0, 0, 3000.f) );
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

			SndDepot->get( "Ready" )->Play();
}

//void cStage_IngameStart::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_IngameStart::Input(const float elapseTime)
{	
}

//void cStage_IngameStart::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_IngameStart::Update(const float elapseTime)
{
/*
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
				character1->Update( elapseTime, m_info1.state1, (float)m_info1.ptMouse.x, (float)m_info1.ptMouse.y );
				character1->Update( elapseTime, m_info1.state2 );
				character2->Update( elapseTime, m_info2.state1, (float)m_info2.ptMouse.x, (float)m_info2.ptMouse.y );
				character2->Update( elapseTime, m_info2.state2 );
		}
		else  //받아올 패킷이 없었다면..
		{			
		//1번, 2번 캐릭터 각각 공격모드인 경우에만 다시 업데이트하고 그 외의 상태일 경우 무시
			if( character1->GetMode() < character1->LATTACK )
			{
				character1->Update( elapseTime, m_info1.state1, (float)m_info1.ptMouse.x, (float)m_info1.ptMouse.y );
				character1->Update( elapseTime, m_info1.state2 );
			}
			if( character2->GetMode() < character2->LATTACK )
			{
				character2->Update( elapseTime, m_info2.state1, (float)m_info2.ptMouse.x, (float)m_info2.ptMouse.y );
				character2->Update( elapseTime, m_info2.state2 );
			}
		}  //if( PacketReceive(packetRecv) )
*/
//	}


//새롭게 갱신된 정보대로 적용시킴
	bool bAniState1 = character1->Move(elapseTime);
	bool bAniState2 = character2->Move(elapseTime);
/*	if( bAniState1 == false && bAniState2 == false )
	{
		character1->SetAniLoop(true);
		character1->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
		character2->SetAniLoop(true);
		character2->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
	}
*/
	if( SndDepot->get( "Ready" )->IsPlaying() == false && m_start == false )
	{
		m_start = true;
		SndDepot->get( "Start" )->Play();
		m_image->Create("../media/image/start02.tga");
	}
	else if( m_start == true && SndDepot->get( "Start" )->IsPlaying() == false )
	{
	//	character1->UpdatePosition();
	//	character2->UpdatePosition();

		tagIngameInfo sIngameInfo;
		sIngameInfo.pCharacter1 = character1;
		sIngameInfo.pCharacter2 = character2;
		sIngameInfo.pShader = m_shader;
		sIngameInfo.pTerrain = m_terrain;
		sIngameInfo.pTerrainShader = m_terrainShader;
		sIngameInfo.pSprite = m_sprite;
		sIngameInfo.pShadowTex = m_pShadowTex;
		sIngameInfo.pShadowSurf = m_pShadowSurf;
		sIngameInfo.pShadowTexZ = m_pShadowTexZ;
		sIngameInfo.pSkybox = m_skybox;

		GetStageMgr()->SetStage( GetStageMgr()->INGAME );
		GetStageMgr()->GetStage()->Init(m_id, &sIngameInfo);
	}

}

//void cStage_IngameStart::Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_IngameStart::Render(const float elapseTime)
{
	m_skybox->Render();

		graphic::cCharacter* pMe = ( m_id == 1 ? character1 : character2 );
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
//		graphic::GetRenderer()->RenderAxis();
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

//		Vector3 pos = pMe->GetCamera()->GetLook();
		Vector3 light = Vector3(0,1000,0);

		Matrix44 matView;// 뷰 행렬
//		matView.SetView2( light, pos, Vector3(0,1,0));
	//test
		matView.SetView2( light, pMe->GetCamera()->GetLook(), Vector3(0,1,0));

		Matrix44 matProj;// 투영 행렬
	//	matProj.SetProjection( D3DX_PI/2.5f, 1, 0.1f, 10000);
		matProj.SetProjection( D3DX_PI * 0.3f, 1, 0.1f, 7000);

		m_shader->SetMatrix( "mVP", matView * matProj);
		m_shader->SetVector( "vLightDir", Vector3(0,-1,0) );
		m_shader->SetVector( "vEyePos", pMe->GetCamera()->GetPosition());

		m_shader->SetRenderPass(1);
		character1->RenderShadow( *m_shader );
		character2->RenderShadow( *m_shader );

/*
		m_terrainShader->SetMatrix( "mVP", matView * matProj);
		m_terrainShader->SetVector( "vLightDir", Vector3(0,-1,0) );
		m_terrainShader->SetVector( "vEyePos", pMe->GetCamera()->GetPosition());
		m_terrainShader->SetRenderPass(3);
		m_terrain->RenderShadowRigidModels( *m_terrainShader, light, matProj );
*/

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

/*
		m_terrainShader->SetMatrix( "mVP", pMe->GetCamera()->GetView() * pMe->GetCamera()->GetProjection() );
		m_terrainShader->SetRenderPass(0);
		m_terrain->RenderShaderRigidModels( *m_terrainShader );
*/

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

		m_image->Render( Matrix44() );
	
		/*
		graphic::cSprite* pImg = dynamic_cast<graphic::cSprite*>(m_hpImage->GetChildren()[0]);
		pImg->SetRect( sRect( 0, 0, (int)( (pMe->GetHP() * 0.01f) * 256 ), 64 ) );
		m_hpImage->Render( Matrix44() );

		pImg = dynamic_cast<graphic::cSprite*>(m_spImage->GetChildren()[0]);
		pImg->SetRect( sRect( 0, 0, (int)( (pMe->GetSP() * 0.01f) * 256 ), 64 ) );
		m_spImage->Render( Matrix44() );
		*/
}
/*
bool cStage_IngameStart::PacketSend(IN network::InfoProtocol& packetInfo)
//bool cStage_IngameStart::PacketSend(const int nState1, const int nState2, const POINT ptMouse)
//bool cStage_IngameStart::PacketSend(const network::InfoProtocol packetInfo)
{
	graphic::cCharacter* pMe = ( m_infoSend.nId == 1 ? character1 : character2 );

	m_infoSend.state1 = packetInfo.state1;
	m_infoSend.state2 = packetInfo.state2;
//	m_infoSend.nId = m_id;
	m_infoSend.ptMouse = packetInfo.ptMouse;
	m_infoSend.camLook = pMe->GetCamera()->GetLook();
	m_infoSend.camPos = pMe->GetCamera()->GetPosition();
	m_infoSend.character = pMe->GetTM();
	m_infoSend.bResult = packetInfo.bResult;
	m_infoSend.sWinner = packetInfo.sWinner;

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

bool cStage_IngameStart::PacketReceive(OUT network::InfoProtocol& packetInfo)
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
			packetInfo.state1 = protocol->state1;
			packetInfo.state2 = protocol->state2;
			packetInfo.ptMouse.x = protocol->ptMouse.x;
			packetInfo.ptMouse.y = protocol->ptMouse.y;
			packetInfo.camLook = protocol->camLook;
			packetInfo.camPos = protocol->camPos;
			packetInfo.character = protocol->character;
			packetInfo.bResult = protocol->bResult;
			packetInfo.sWinner = protocol->sWinner;

			return true;
		}
	}

	return false;
}
*/

void cStage_IngameStart::LoadMapObject(const string& fileName)
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

		if( fileName == "../media/mesh/map/BaseBroken_SM.dat" )
			pNewObj->CreateCube(7.f);
		else
			pNewObj->CreateCube();

//		m_terrain->AddRigidModel( *pNewObj );
//		delete pNewObj;

		rTerrainObj.push_back( pNewObj );
	}
}