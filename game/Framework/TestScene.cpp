
#include "stdafx.h"
#include "TestScene.h"

using namespace framework;

cTestScene::cTestScene(LPD3DXSPRITE sprite, const int nId, const string& sName) :
	framework::cWindow(sprite, nId, sName)
{
//	EventConnect(this, framework::EVENT::BUTTON_CLICK, 1, (framework::EventFunction)&cTestScene::Button1Click);
//	m_clickLimit = false;
}

cTestScene::~cTestScene()
{

}

void cTestScene::Button_main_1Click(framework::cEvent &event)
{
//	GetStageMgr()->GetStage()->Release();
	GetStageMgr()->SetStage( GetStageMgr()->CHARACTER_SELECT );
	GetStageMgr()->GetStage()->Init();
}

void cTestScene::Button_CS_1Click(framework::cEvent &event)
{	
//	GetStageMgr()->GetStage()->Release();
	GetStageMgr()->SetStage( GetStageMgr()->NETWORK_SELECT );
	GetStageMgr()->GetStage()->Init();
}

void cTestScene::Button_NS_1Click(framework::cEvent &event)
{
	//if( m_clickLimit )
	//	return;
	//else if( !m_clickLimit )
	//	m_clickLimit = true;

	if( false == GetStageMgr()->SetSocket() )
		return;

//	GetStageMgr()->GetStage()->Release();
	GetStageMgr()->SetStage( GetStageMgr()->NETWORK_LOADING );
	GetStageMgr()->GetStage()->Init(1);
}

void cTestScene::Button_NS_2Click(framework::cEvent &event)
{
	//if( m_clickLimit )
	//	return;
	//else if( !m_clickLimit )
	//	m_clickLimit = true;

	if( false == GetStageMgr()->SetSocket() )
		return;

//	GetStageMgr()->GetStage()->Release();
	GetStageMgr()->SetStage( GetStageMgr()->NETWORK_LOADING );
	GetStageMgr()->GetStage()->Init(2);
}