#include "stdafx.h"
#include "stage_ingame.h"

using namespace framework;

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
	SAFE_RELEASE(m_font);
}

//void cStage_Ingame::Init()
void cStage_Ingame::Init(const int nId)
{
//font 생성
	HRESULT hr = D3DXCreateFontA( graphic::GetDevice(), 50, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
	DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "굴림", &m_font );

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
		graphic::GetRenderer()->RenderFPS();
		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();

//		character1->Render();
//		character2->Render();

	//test
		Matrix44 VP;
		VP = pMe->GetCamera()->GetView() * pMe->GetCamera()->GetProjection();
		m_shader->SetMatrix( "mVP", VP );
		m_shader->SetVector( "vLightDir", Vector3(0,-1,0) );  //hlsl에 디폴트 값으로 되어있음
		m_shader->SetVector( "vEyePos", pMe->GetCamera()->GetPosition() );
		character1->RenderShader( *m_shader );
		character2->RenderShader( *m_shader );
		
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