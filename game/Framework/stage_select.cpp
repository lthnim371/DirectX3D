#include "stdafx.h"
#include "stage_select.h"

using namespace framework;

cStage_Select::cStage_Select()
{
}
cStage_Select::~cStage_Select()
{
	SAFE_DELETE(m_scene);
	SAFE_DELETE(m_sprite);
}

//void cStage_Select::Init()
void cStage_Select::Init(const int nId)
{
	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);

	m_scene = new cTestScene(m_sprite, 0, "MainScene");
	m_scene->Create("../media/image/UI_Main_Scene.tga");
	m_scene->SetPos( Vector3(0.f, 0.f, 0.f) );
	
	cButton* pBtn1 = new cButton(m_sprite, 1, "Button2");
	pBtn1->Create( "../media/image/Connecting_1.tga" );
	pBtn1->SetPos( Vector3(200.f, 300.f, 0.f ) );
	m_scene->InsertChild( pBtn1 );
	pBtn1->EventConnect(pBtn1, EVENT::BUTTON_CLICK, 1, (EventFunction)&cTestScene::Button2Click);

	cButton* pBtn2 = new cButton(m_sprite, 2, "Button3");
	pBtn2->Create( "../media/image/Connecting_1.tga" );
	pBtn2->SetPos( Vector3(500.f, 300.f, 0.f ) );
	m_scene->InsertChild( pBtn2 );
	pBtn2->EventConnect(pBtn2, EVENT::BUTTON_CLICK, 2, (EventFunction)&cTestScene::Button3Click);

}

//void cStage_Select::Input(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Select::Input(const float elapseTime)
{
}

//void cStage_Select::Update(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Select::Update(const float elapseTime)
{
}

//void cStage_Select::Render(const float elapseTime, graphic::cCharacter* character1, graphic::cCharacter* character2)
void cStage_Select::Render(const float elapseTime)
{
	m_scene->Render( Matrix44() );
}


bool cStage_Select::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_scene->MessageProc(message, wParam, lParam);
}