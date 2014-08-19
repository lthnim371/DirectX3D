#include "stdafx.h"
#include "stage_NetworkLoading.h"

using namespace framework;

cStage_NetworkLoading::cStage_NetworkLoading()
{
	fTick1 = 0.f;
	fTick2 = 0.f;
//	usCount = 7;
	m_restart = false;
}
cStage_NetworkLoading::~cStage_NetworkLoading()
{
	Release();
}

//void cStage_Select::Init()
void cStage_NetworkLoading::Init(const int nId, tagIngameInfo* sIngameInfo)
{
//debug
//	GetStageMgr()->SetStage( GetStageMgr()->INGAMESTART );
//	GetStageMgr()->GetStage()->Init(nId);
//
	ZeroMemory(&m_acceseInfo, sizeof(m_acceseInfo));
	m_acceseInfo.nId = nId;
	m_acceseInfo.bAccess = true;
	m_access = false;

/*
	if( PacketReceive(100000) )
	{
		PacketSend();
		GetStageMgr()->Release();
		GetStageMgr()->SetStage( GetStageMgr()->INGAME );
		GetStageMgr()->GetStage()->Init(m_acceseInfo.nId);
	}
*/
	if( m_restart )
	{
		m_button1->Create( (m_acceseInfo.nId == 1 ? "../media/image/Connecting_2.tga" : "../media/image/Connecting_2_Gray.tga") );
		m_button2->Create( (m_acceseInfo.nId == 2 ? "../media/image/Connecting_2.tga" : "../media/image/Connecting_2_Gray.tga") );
		return;
	}
	m_restart = true;

	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);

	m_scene = new cTestScene(m_sprite, 0, "MainScene");
	m_scene->Create("../media/image/select_screen_valle.png");
	m_scene->SetPos( Vector3(0.f, 0.f, 0.f) );
	
/*
	cButton* pBtn1 = new cButton(m_sprite, 1, "Button4");
	pBtn1->Create( (m_acceseInfo.nId == 1 ? "../media/image/Connecting_2.tga" : "../media/image/Connecting_2_Gray.tga") );
	pBtn1->SetPos( Vector3(150.f, 300.f, 0.f ) );
	m_scene->InsertChild( pBtn1 );

	cButton* pBtn2 = new cButton(m_sprite, 2, "Button5");
	pBtn2->Create( (m_acceseInfo.nId == 2 ? "../media/image/Connecting_2.tga" : "../media/image/Connecting_2_Gray.tga") );
	pBtn2->SetPos( Vector3(300.f, 300.f, 0.f ) );
	m_scene->InsertChild( pBtn2 );
*/
	m_button1 = new cButton(m_sprite, 1, "Button4");
	m_button1->Create( (m_acceseInfo.nId == 1 ? "../media/image/Connecting_2.tga" : "../media/image/Connecting_2_Gray.tga") );
	m_button1->SetPos( Vector3(150.f, 300.f, 0.f ) );
	m_scene->InsertChild( m_button1 );

	m_button2 = new cButton(m_sprite, 2, "Button5");
	m_button2->Create( (m_acceseInfo.nId == 2 ? "../media/image/Connecting_2.tga" : "../media/image/Connecting_2_Gray.tga") );
	m_button2->SetPos( Vector3(300.f, 300.f, 0.f ) );
	m_scene->InsertChild( m_button2 );
}

void cStage_NetworkLoading::Release()
{
	SAFE_DELETE(m_scene);
	m_sprite->Release();
}

//void cStage_Select::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_NetworkLoading::Input(const float elapseTime)
{
}

//void cStage_Select::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_NetworkLoading::Update(const float elapseTime)
{
	fTick1 += elapseTime;
	if( fTick1 >= 0.05f )
	{
		fTick1 = 0.f;
		PacketSend();
	}

	network::AccessProtocol packetInfo;
	if( PacketReceive(packetInfo, 10) )
	{
	//	PacketSend();

		if( !m_access )
		{
			if( m_acceseInfo.nId == 1 && packetInfo.nId == 2 )
			{
				m_access = packetInfo.bAccess;
				cButton* pBtn = dynamic_cast<cButton*>(m_scene->GetChildren()[1]);
				pBtn->Create( "../media/image/Connecting_2.tga" );
			}
			else if( m_acceseInfo.nId == 2 && packetInfo.nId == 1 )
			{
				m_access = packetInfo.bAccess;
				cButton* pBtn = dynamic_cast<cButton*>(m_scene->GetChildren()[0]);
				pBtn->Create( "../media/image/Connecting_2.tga" );
			}
		}
	}

	if( m_access )
	{
		fTick2 += elapseTime;

		if( fTick2 > 3.f )
		{
			fTick2 = 0.f;
		//	GetStageMgr()->SetStage( GetStageMgr()->INGAME );
			GetStageMgr()->SetStage( GetStageMgr()->INGAMESTART );
			GetStageMgr()->GetStage()->Init(m_acceseInfo.nId);
		}
	}
}

//void cStage_Select::Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_NetworkLoading::Render(const float elapseTime)
{
/*
	usCount += 2;
	if( usCount >= 77 )
		usCount = 7;
	char buff[8];
	::_itoa_s( usCount, buff, sizeof(buff), 10 );

	string str1("../media/image/main_movie_I");
	string str2( buff );
	str2.append( ".tga" );

	m_scene->Create( str1 + str2 );
*/
	m_scene->Render( Matrix44() );
}

bool cStage_NetworkLoading::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_scene->MessageProc(message, wParam, lParam);
}

bool cStage_NetworkLoading::PacketSend()
{
	char buff[ 128];  //배열 크기는 미리 정해진 약속이어야 한다. 즉, 채우려는 데이터가 적어도 배열크기로 다 채워야한다.
	ZeroMemory(buff, sizeof(buff));
	memcpy(buff, &m_acceseInfo, sizeof(m_acceseInfo));  //데이터 채우기

	const int result = send( GetStageMgr()->GetSocket(), buff, sizeof(buff), 0 );
	if (result == INVALID_SOCKET)
	{	
		//if( false == GetStageMgr()->SetSocket() )
			return false;
	}

	return true;
}

bool cStage_NetworkLoading::PacketReceive(OUT network::AccessProtocol& out, const u_int uDelay)
{
	const timeval t = {0, uDelay};  //select 함수가 얼마나 대기해서 패킷을 기다릴지를 나타내는 변수입니다.
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

			const network::AccessProtocol* protocol = (network::AccessProtocol*)buff;
	
			if( m_acceseInfo.nId != protocol->nId && protocol->bAccess == true )
			{
				out.nId = protocol->nId;
				out.bAccess = protocol->bAccess;
				return true;
			}
		}
	}

	return false;
}