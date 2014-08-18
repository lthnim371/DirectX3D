#include "stdafx.h"
#include "stage_NetworkSelect.h"

using namespace framework;

cStage_NetworkSelect::cStage_NetworkSelect()
{
//	usCount = 7;
}
cStage_NetworkSelect::~cStage_NetworkSelect()
{
	Release();
}

//void cStage_NetworkSelect::Init()
void cStage_NetworkSelect::Init(const int nId)
{
	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);

	m_scene = new cTestScene(m_sprite, 0, "MainScene");
	m_scene->Create("../media/image/select_screen_valle.png");
	m_scene->SetPos( Vector3(0.f, 0.f, 0.f) );
/*	
	cButton* pBtn1 = new cButton(m_sprite, 3, "Button1");
	pBtn1->Create( "../media/image/Connecting_1.tga" );
	pBtn1->SetPos( Vector3(200.f, 300.f, 0.f ) );
	m_scene->InsertChild( pBtn1 );
	pBtn1->EventConnect(pBtn1, EVENT::BUTTON_CLICK, 3, (EventFunction)&cTestScene::Button_NS_1Click);

	cButton* pBtn2 = new cButton(m_sprite, 4, "Button2");
	pBtn2->Create( "../media/image/Connecting_1.tga" );
	pBtn2->SetPos( Vector3(500.f, 300.f, 0.f ) );
	m_scene->InsertChild( pBtn2 );
	pBtn2->EventConnect(pBtn2, EVENT::BUTTON_CLICK, 4, (EventFunction)&cTestScene::Button_NS_2Click);
*/
	m_button1 = new cButton(m_sprite, 1, "Button1");
	m_button1->Create( "../media/image/Connecting_1.tga" );
	m_button1->SetPos( Vector3(150.f, 300.f, 0.f ) );
	m_scene->InsertChild( m_button1 );
	m_button1->EventConnect(m_button1, EVENT::BUTTON_CLICK, 1, (EventFunction)&cTestScene::Button_NS_1Click);

	m_button2 = new cButton(m_sprite, 2, "Button2");
	m_button2->Create( "../media/image/Connecting_1.tga" );
	m_button2->SetPos( Vector3(300.f, 300.f, 0.f ) );
	m_scene->InsertChild( m_button2 );
	m_button2->EventConnect(m_button2, EVENT::BUTTON_CLICK, 2, (EventFunction)&cTestScene::Button_NS_2Click);
}

//void cStage_NetworkSelect::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_NetworkSelect::Input(const float elapseTime)
{
}

//void cStage_NetworkSelect::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_NetworkSelect::Update(const float elapseTime)
{
}

//void cStage_NetworkSelect::Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_NetworkSelect::Render(const float elapseTime)
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
	m_scene->Render( Matrix44() );
*/

	m_scene->Render( Matrix44() );
}

void cStage_NetworkSelect::Release()
{
	m_button1->EventDisconnect( m_button1, EVENT::BUTTON_CLICK , 1 );
	m_button2->EventDisconnect( m_button2, EVENT::BUTTON_CLICK , 2 );
	SAFE_DELETE(m_scene);
	m_sprite->Release();
}

bool cStage_NetworkSelect::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_scene->MessageProc(message, wParam, lParam);
}