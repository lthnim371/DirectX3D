#include "stdafx.h"
#include "stage_NetworkSelect.h"

using namespace framework;

cStage_CharacterSelect::cStage_CharacterSelect()
{
	m_restart = false;
	usCount = 7;
}
cStage_CharacterSelect::~cStage_CharacterSelect()
{
	Release();
}

//void cStage_NetworkSelect::Init()
void cStage_CharacterSelect::Init(const int nId, tagIngameInfo* sIngameInfo)
{
	if( m_restart )
		return;
	m_restart = true;

	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);

	m_scene = new cTestScene(m_sprite, 0, "MainScene");
	m_scene->Create("../media/image/main_movie_I7.tga");
	m_scene->SetPos( Vector3(0.f, 0.f, 0.f) );
	
/*
	cButton* pBtn1 = new cButton(m_sprite, 2, "select_valle");
	pBtn1->Create( "../media/image/select_valle.png" );
	pBtn1->SetPos( Vector3(100.f, 100.f, 0.f ) );
	m_scene->InsertChild( pBtn1 );
	pBtn1->EventConnect(pBtn1, EVENT::BUTTON_CLICK, 2, (EventFunction)&cTestScene::Button_CS_1Click);
*/
	m_button1 = new cButton(m_sprite, 1, "select_valle");
	m_button1->Create( "../media/image/select_valle.png" );
	m_button1->SetPos( Vector3(100.f, 100.f, 0.f ) );
	m_scene->InsertChild( m_button1 );
	m_button1->EventConnect(m_button1, EVENT::BUTTON_CLICK, 1, (EventFunction)&cTestScene::Button_CS_1Click);
		
}

//void cStage_NetworkSelect::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_CharacterSelect::Input(const float elapseTime)
{
}

//void cStage_NetworkSelect::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_CharacterSelect::Update(const float elapseTime)
{
}

//void cStage_NetworkSelect::Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_CharacterSelect::Render(const float elapseTime)
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

void cStage_CharacterSelect::Release()
{
	m_button1->EventDisconnect( m_button1, EVENT::BUTTON_CLICK , 1 );
	SAFE_DELETE(m_scene);
	m_sprite->Release();
}

bool cStage_CharacterSelect::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_scene->MessageProc(message, wParam, lParam);
}