#include "stdafx.h"
#include "stage_ingame.h"

using namespace framework;

cStage_Ingame::cStage_Ingame()
{
//	m_id = 0;
	ZeroMemory(&m_packetInfo, sizeof(m_packetInfo));
	m_access = false;
}
cStage_Ingame::~cStage_Ingame()
{
	SAFE_DELETE(character1);
	SAFE_DELETE(character2);
}

//void cStage_Ingame::Init()
void cStage_Ingame::Init(const int nId)
{
	fTick1 = 0.f;
	fTick2 = 0.f;

	character1 = new graphic::cCharacter(0);
	character2 = new graphic::cCharacter(1);
//	m_id = nId;
	m_packetInfo.nId = nId;
			
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
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back2.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front1.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front2.ani" );
				character1->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );

				//Matrix44 rot;
				//rot.SetRotationY( -1.f );
				Matrix44 pos;
				pos.SetTranslate( Vector3( 0, 0, -5000.f) );
				character1->MultiplyTM( pos);
				graphic::GetCamera()->SetPosition( character1->GetTM() );

			//character2
				character2->Create( "..\\media\\mesh\\valle\\valle_character1.dat" );
				character2->LoadWeapon( "..\\media\\mesh\\valle\\valle_weapon1.dat" );
			//test
			//	character2->SetRenderBoundingBox(true);

				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_backward.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_dash.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_LA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_LLA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_LLLA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_LRA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_LLRA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_RA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_RRA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_RRRA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_jump1.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_jump2.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_jump3.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_JLA.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back1.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back2.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front1.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front2.ani" );
				character2->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );

				//Matrix44 rot;
				//rot.SetRotationY( 180.f );
				pos.SetTranslate( Vector3( 0, 0, 5000.f) );
				character2->MultiplyTM( pos);
				graphic::GetCamera()->SetPosition( character2->GetTM() );

	
	::GetCursorPos( &m_currMouse );
	::ScreenToClient( GetStageMgr()->GetWindowHandle(), &m_currMouse );
}

//void cStage_Ingame::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ingame::Input(const float elapseTime)
{
//	if( !m_access )
//		GetStageMgr()->SetSocket();
	
	POINT ptMouse;
	ptMouse.x = 0;
	ptMouse.y = 0;
	network::PROTOCOL::TYPE nState1 = network::PROTOCOL::NONE;
	network::PROTOCOL::TYPE nState2 = network::PROTOCOL::NONE;

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
		nState1 = network::PROTOCOL::ROTATION;
	}
	
/*	//프로그램 테스트용
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
	else
	{
	//	nState2 = network::PROTOCOL::NORMAL;
		nState2 = network::PROTOCOL::NONE;
	//	pMe->Update( pMe->NORMAL );
	//	character1->Update( character1->NORMAL );
	}

/*	
	if( network::PROTOCOL::FORWARD <= nState2 && nState2 <= network::PROTOCOL::DASH ||
		nState1 == network::PROTOCOL::ROTATION )
	{
	//	if( fTick1 >= 0.1f )
	//	{
			fTick1 = 0.f;
			PacketSend(nState1, nState2, ptMouse);
	//	}
	}
*/
/*
	if( nState2 != m_packetInfo.header2.protocol || nState1 != m_packetInfo.header1.protocol )
	{
		PacketSend(nState1, nState2, ptMouse);
	}
	else if( fTick1 >= 0.1f )
	{
		
	}
*/
	fTick1 += elapseTime;
	if( nState2 != network::PROTOCOL::NONE || nState1 != network::PROTOCOL::NONE )
	{
		fTick1 = 0.f;
		PacketSend(nState1, nState2, ptMouse);
	}
	
	
	//	PacketSend();
	//	m_access = PacketSend((int)nState1, (int)nState2, ptMouse);
/*	else if( m_elapseTime >= 100.f )
	{
		m_elapseTime = 0.f;
	//	PacketSend();
		PacketSend(nState1, nState2, ptMouse);
	//	m_access = PacketSend((int)nState1, (int)nState2, ptMouse);
	}	*/
}

//void cStage_Ingame::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ingame::Update(const float elapseTime)
{
//	fTick2 += elapseTime;

//	if(fTick2 >= 0.2f)
//	{
//		fTick2 = 0.f;

		network::InfoProtocol packetRecv;
		ZeroMemory(&packetRecv, sizeof(packetRecv));
		m_access = PacketReceive(packetRecv);

		if( m_access == true )
		{
			graphic::cCharacter* pMe = ( packetRecv.nId == 0 ? character1 : character2 );
		
			if( packetRecv.header1.protocol == network::PROTOCOL::ROTATION )
			{		
				pMe->Update( pMe->ROTATION, (float)packetRecv.ptMouse.x, (float)packetRecv.ptMouse.y );
			}

			if( packetRecv.header2.protocol != network::PROTOCOL::NONE )
			{
				pMe->Update( packetRecv.header2.protocol );
			}
		}

//	}

/*	
	if( InputMgr->isOnceKeyDown('1') )
	{
		graphic::GetCamera()->SetHeight(-10.f);
	}
	else if( InputMgr->isOnceKeyDown('2') )
	{
		graphic::GetCamera()->SetHeight(10.f);
	}
*/
	bool bAniState = character1->Move(elapseTime);
	bool bAniState2 = character2->Move(elapseTime);
//	graphic::GetCamera()->SetPosition( character1->GetTM() );
		
		if( character1->GetCubeCheck() == true )
		{
			if( true == character2->CollisionCheck( *(character1->GetWeaponCube()), graphic::GetCamera()->GetLook() ) )
			{
				character1->SetAttackSuccess();
			}
		}
		else if( character2->GetCubeCheck() == true )
		{
			if ( true == character1->CollisionCheck( *(character2->GetWeaponCube()), graphic::GetCamera()->GetLook() ) )
			{
				character2->SetAttackSuccess();
			}
		}

		if( character1->GetMode() == character1->BEHIT )
		{
			if( true == character1->CollisionCheck( *(character2->GetCharacterCube()), graphic::GetCamera()->GetLook(), graphic::GetCamera()->GetDirection() ) )
				character1->UpdateBeHit( bAniState, graphic::GetCamera()->GetDirection(), character2->GetAniPosGap()  );
		}
		else if( character2->GetMode() == character2->BEHIT )
		{
			if( true == character2->CollisionCheck( *(character1->GetCharacterCube()), graphic::GetCamera()->GetLook(), graphic::GetCamera()->GetDirection() ) )
				character2->UpdateBeHit( bAniState2, graphic::GetCamera()->GetDirection(), character1->GetAniPosGap() );
		}
	
}

//void cStage_Ingame::Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ingame::Render(const float elapseTime)
{
		//fps 및 그리드 출력
		graphic::GetRenderer()->RenderFPS();
		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();


//		character1->Render();
//		character2->Render();

	//test
		character1->RenderShader( graphic::GetCamera()->GetShader() );
		character2->RenderShader( graphic::GetCamera()->GetShader() );				

	//test
		graphic::GetCamera()->Render( character1->GetHP(), character2->GetHP() );
}

bool cStage_Ingame::PacketSend(const network::PROTOCOL::TYPE nState1, const network::PROTOCOL::TYPE nState2, const POINT ptMouse)
//bool cStage_Ingame::PacketSend(const int nState1, const int nState2, const POINT ptMouse)
//bool cStage_Ingame::PacketSend(const network::InfoProtocol packetInfo)
{
	m_packetInfo.header1.protocol = nState1;
	m_packetInfo.header2.protocol = nState2;
//	m_packetInfo.nId = m_id;
	m_packetInfo.ptMouse = ptMouse;

	char buff[ 128];  //배열 크기는 미리 정해진 약속이어야 한다. 즉, 채우려는 데이터가 적어도 배열크기로 다 채워야한다.
	ZeroMemory(buff, sizeof(buff));
	memcpy(buff, &m_packetInfo, sizeof(m_packetInfo));  //데이터 채우기

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
			packetInfo.header1 = protocol->header1;
			packetInfo.header2 = protocol->header2;
			packetInfo.ptMouse.x = protocol->ptMouse.x;
			packetInfo.ptMouse.y = protocol->ptMouse.y;

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