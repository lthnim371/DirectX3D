#include "stdafx.h"
#include "stage_NetworkSelect.h"

using namespace framework;

cStage_Ending::cStage_Ending()
{
}
cStage_Ending::~cStage_Ending()
{
	Release();
}

//void cStage_NetworkSelect::Init()
void cStage_Ending::Init(const int nId, tagIngameInfo* sIngameInfo)
{
	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);

	m_scene = new cTestScene(m_sprite, 0, "MainScene");
	m_scene->Create("../media/image/ending_screen.png");
	m_scene->SetPos( Vector3(0.f, 0.f, 0.f) );
		
	cButton* pBtn1 = new cButton(m_sprite, 1, "Ending_Valle");
	pBtn1->Create( "../media/image/CH_Valle.tga" );
	pBtn1->SetPos( Vector3(384.f, 38.f, 0.f ) );
	m_scene->InsertChild( pBtn1 );

	cButton* pBtn2 = new cButton(m_sprite, 2, "Score");
	pBtn2->Create( "../media/image/UI_Ending_Score.png" );
	pBtn2->SetPos( Vector3(475.f, 425.f, 0.f ) );
	m_scene->InsertChild( pBtn2 );

	m_button1 = new cButton(m_sprite, 3, "ToMain");
	m_button1->Create( "../media/image/valle_2.tga" );
	m_button1->SetPos( Vector3(1080.f, 520.f, 0.f ) );
	m_scene->InsertChild( m_button1 );
	m_button1->EventConnect(m_button1, EVENT::BUTTON_CLICK, 3, (EventFunction)&cTestScene::Button_ED_1Click);
}

//void cStage_NetworkSelect::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ending::Input(const float elapseTime)
{
}

//void cStage_NetworkSelect::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ending::Update(const float elapseTime)
{
}

//void cStage_NetworkSelect::Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Ending::Render(const float elapseTime)
{
	m_scene->Render( Matrix44() );
}

void cStage_Ending::Release()
{
	m_button1->EventDisconnect( m_button1, EVENT::BUTTON_CLICK , 3 );
	SAFE_DELETE(m_scene);
	m_sprite->Release();
}

bool cStage_Ending::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_scene->MessageProc(message, wParam, lParam);
}