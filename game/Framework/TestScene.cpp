
#include "stdafx.h"
#include "TestScene.h"

using namespace framework;

cTestScene::cTestScene(LPD3DXSPRITE sprite, const int nId, const string& sName) :
	framework::cWindow(sprite, nId, sName)
{
//	EventConnect(this, framework::EVENT::BUTTON_CLICK, 1, (framework::EventFunction)&cTestScene::Button1Click);
}

cTestScene::~cTestScene()
{

}

void cTestScene::Button1Click(framework::cEvent &event)
{
	GetStageMgr()->SetStage( GetStageMgr()->SELECT );
	GetStageMgr()->GetStage()->Init();
}

void cTestScene::Button2Click(framework::cEvent &event)
{
	if( false == GetStageMgr()->SetSocket() )
		return;

	GetStageMgr()->SetStage( GetStageMgr()->INGAME );
	GetStageMgr()->GetStage()->Init(1);
}

void cTestScene::Button3Click(framework::cEvent &event)
{
	if( false == GetStageMgr()->SetSocket() )
		return;

	GetStageMgr()->SetStage( GetStageMgr()->INGAME );
	GetStageMgr()->GetStage()->Init(2);
}