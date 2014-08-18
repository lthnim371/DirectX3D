#include "stdafx.h"
#include "stage_main.h"

using namespace framework;

cStage_Main::cStage_Main()
{
	usCount = 7;
}

cStage_Main::~cStage_Main()
{
	Release();
}

//void cStage_Main::Init()
void cStage_Main::Init(const int nId, tagIngameInfo* sIngameInfo)
{
	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);

	m_scene = new cTestScene(m_sprite, 0, "MainScene");
	m_scene->Create("../media/image/main_movie_I7.tga");
	m_scene->SetPos( Vector3(0.f, 0.f, 0.f) );
	
/*
	cButton* pBtn1 = new cButton(m_sprite, 1, "Button1");
	pBtn1->Create( "../media/image/UI_Main_TitleButton.tga" );
	pBtn1->SetPos( Vector3(150.f, 300.f, 0.f ) );
	m_scene->InsertChild( pBtn1 );
	pBtn1->EventConnect(pBtn1, EVENT::BUTTON_CLICK, 1, (EventFunction)&cTestScene::Button_main_1Click);
*/
	m_button1 = new cButton(m_sprite, 1, "ArcheBlade");
	m_button1->Create( "../media/image/UI_Main_TitleButton.tga" );
	m_button1->SetPos( Vector3(150.f, 300.f, 0.f ) );
	m_scene->InsertChild( m_button1 );
	m_button1->EventConnect(m_button1, EVENT::BUTTON_CLICK, 1, (EventFunction)&cTestScene::Button_main_1Click);
}

void cStage_Main::Release()
{
	m_button1->EventDisconnect( m_button1, EVENT::BUTTON_CLICK , 1 );
	SAFE_DELETE(m_scene);
	m_sprite->Release();
}

//void cStage_Main::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Main::Input(const float elapseTime)
{
}

//void cStage_Main::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Main::Update(const float elapseTime)
{
}

//void cStage_Main::Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Main::Render(const float elapseTime)
{
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
}

bool cStage_Main::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_scene->MessageProc(message, wParam, lParam);
}