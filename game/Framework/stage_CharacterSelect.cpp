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

	cButton* pBtn2 = new cButton(m_sprite, 2, "select_tiac");
	pBtn2->Create( "../media/image/large_tiac.tga" );
	pBtn2->SetPos( Vector3(250.f, 100.f, 0.f ) );
	m_scene->InsertChild( pBtn2 );

	cButton* pBtn3 = new cButton(m_sprite, 3, "select_elrath");
	pBtn3->Create( "../media/image/large_elrath.tga" );
	pBtn3->SetPos( Vector3(400.f, 100.f, 0.f ) );
	m_scene->InsertChild( pBtn3 );

	cButton* pBtn4 = new cButton(m_sprite, 4, "select_ridika");
	pBtn4->Create( "../media/image/large_Ridika.tga" );
	pBtn4->SetPos( Vector3(100.f, 250.f, 0.f ) );
	m_scene->InsertChild( pBtn4 );

	cButton* pBtn5 = new cButton(m_sprite, 5, "select_hyde");
	pBtn5->Create( "../media/image/large_Hyde.tga" );
	pBtn5->SetPos( Vector3(250.f, 250.f, 0.f ) );
	m_scene->InsertChild( pBtn5 );

	cButton* pBtn6 = new cButton(m_sprite, 6, "select_rehiney");
	pBtn6->Create( "../media/image/large_rehiney_G.tga" );
	pBtn6->SetPos( Vector3(400.f, 250.f, 0.f ) );
	m_scene->InsertChild( pBtn6 );

	cButton* pBtn7 = new cButton(m_sprite, 7, "select_elichea");
	pBtn7->Create( "../media/image/large_elichea.tga" );
	pBtn7->SetPos( Vector3(100.f, 400.f, 0.f ) );
	m_scene->InsertChild( pBtn7 );

	cButton* pBtn8 = new cButton(m_sprite, 8, "select_dick");
	pBtn8->Create( "../media/image/large_dick.tga" );
	pBtn8->SetPos( Vector3(250.f, 400.f, 0.f ) );
	m_scene->InsertChild( pBtn8 );

	cButton* pBtn9 = new cButton(m_sprite, 9, "select_renoah");
	pBtn9->Create( "../media/image/large_renoah.tga" );
	pBtn9->SetPos( Vector3(400.f, 400.f, 0.f ) );
	m_scene->InsertChild( pBtn9 );
		
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