#include "stdafx.h"
#include "stage_NetworkLoading.h"

using namespace framework;

cStage_NetworkLoading::cStage_NetworkLoading()
{
	ZeroMemory(&m_acceseInfo, sizeof(m_acceseInfo));
	fTick = 0.f;
//	usCount = 7;
}
cStage_NetworkLoading::~cStage_NetworkLoading()
{
	Release();
}

//void cStage_Select::Init()
void cStage_NetworkLoading::Init(const int nId, tagIngameInfo* sIngameInfo)
{
//debug
//	GetStageMgr()->SetStage( GetStageMgr()->INGAME );
//	GetStageMgr()->GetStage()->Init(nId);
//
	m_acceseInfo.nId = nId;
	m_acceseInfo.bAccess = true;

/*
	if( PacketReceive(100000) )
	{
		PacketSend();
		GetStageMgr()->Release();
		GetStageMgr()->SetStage( GetStageMgr()->INGAME );
		GetStageMgr()->GetStage()->Init(m_acceseInfo.nId);
	}
*/

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

//void cStage_Select::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_NetworkLoading::Input(const float elapseTime)
{
	fTick += elapseTime;
	if( fTick >= 0.1f )
	{
		fTick = 0.f;

		PacketSend();
	}
}

//void cStage_Select::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_NetworkLoading::Update(const float elapseTime)
{
	network::AccessProtocol packetInfo;

	if( PacketReceive(packetInfo, 10) )
	{
		PacketSend();

		if( m_acceseInfo.nId == 1 && packetInfo.nId == 2 )
		{
			cButton* pBtn = dynamic_cast<cButton*>(m_scene->GetChildren()[1]);
			pBtn->Create( "../media/image/Connecting_2.tga" );
		}
		else if( m_acceseInfo.nId == 2 && packetInfo.nId == 1 )
		{
			cButton* pBtn = dynamic_cast<cButton*>(m_scene->GetChildren()[0]);
			pBtn->Create( "../media/image/Connecting_2.tga" );
		}

	//	m_scene->Render( Matrix44() );

	//	Release();
		GetStageMgr()->SetStage( GetStageMgr()->INGAME );
		GetStageMgr()->GetStage()->Init(m_acceseInfo.nId);

		m_scene->Render( Matrix44() );
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

void cStage_NetworkLoading::Release()
{
	SAFE_DELETE(m_button1);
	SAFE_DELETE(m_button2);
	SAFE_DELETE(m_scene);
	m_sprite->Release();
}

bool cStage_NetworkLoading::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_scene->MessageProc(message, wParam, lParam);
}

bool cStage_NetworkLoading::PacketSend()
{
	char buff[ 128];  //�迭 ũ��� �̸� ������ ����̾�� �Ѵ�. ��, ä����� �����Ͱ� ��� �迭ũ��� �� ä�����Ѵ�.
	ZeroMemory(buff, sizeof(buff));
	memcpy(buff, &m_acceseInfo, sizeof(m_acceseInfo));  //������ ä���

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
	const timeval t = {0, uDelay};  //select �Լ��� �󸶳� ����ؼ� ��Ŷ�� ��ٸ����� ��Ÿ���� �����Դϴ�.
	fd_set readSockets;
	FD_ZERO(&readSockets);
	FD_SET(GetStageMgr()->GetSocket(), &readSockets);

	const int ret = select( readSockets.fd_count, &readSockets, NULL, NULL, &t );  //�����κ��� ��Ŷ�� �Դ��� �Ǵ� (= peekmessage�� ���)
	if (ret != 0 && ret != SOCKET_ERROR)
	{
		char buff[ 128];
		const int result = recv( readSockets.fd_array[ 0], buff, sizeof(buff), 0);  //�����κ��� ������ �޴´�. ���� ���� ������ ������ ��� ��ٸ��� //readSockets(����)�� ������ � ���� Ȥ�� Ŭ���̾�Ʈ�� ����Ǿ����� �� �� �ִ�.
		if (result == SOCKET_ERROR || result == 0) // ���� ��Ŷ����� 0�̸� ������ ������ ����ٴ� �ǹ̴�.
		{
			GetStageMgr()->Release();
			//if( false == GetStageMgr()->SetSocket() )
				return false;
		}
		else
		{
		//	ParsePacket(buff);  //��Ŷ�� �Դٸ� ȣ��

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