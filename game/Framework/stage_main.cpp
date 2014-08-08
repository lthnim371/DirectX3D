#include "stdafx.h"
#include "stage_main.h"

using namespace framework;

cStage_Main::cStage_Main()
{
}

cStage_Main::~cStage_Main()
{
	SAFE_DELETE(m_scene);
	SAFE_DELETE(m_sprite);
}

//void cStage_Main::Init()
void cStage_Main::Init(const int nId)
{
	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);

	m_scene = new cTestScene(m_sprite, 0, "MainScene");
	m_scene->Create("../media/image/UI_Main_Scene.tga");
	m_scene->SetPos( Vector3(0.f, 0.f, 0.f) );
	
	cButton* pBtn1 = new cButton(m_sprite, 1, "Button1");
	pBtn1->Create( "../media/image/UI_Main_TitleButton.tga" );
	pBtn1->SetPos( Vector3(150.f, 300.f, 0.f ) );
	m_scene->InsertChild( pBtn1 );
	pBtn1->EventConnect(pBtn1, EVENT::BUTTON_CLICK, 1, (EventFunction)&cTestScene::Button1Click);
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
	m_scene->Render( Matrix44() );
}


bool cStage_Main::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_scene->MessageProc(message, wParam, lParam);
}