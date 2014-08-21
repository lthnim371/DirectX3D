#include "stdafx.h"
#include "stagemanager.h"

#include <fstream>

using namespace framework;

cStageMgr::cStageMgr()
{
	cStage_Main* pMain = new cStage_Main;
	m_list.insert( std::make_pair(MAIN, pMain) );
	cStage_CharacterSelect* pCharSel = new cStage_CharacterSelect;
	m_list.insert( std::make_pair(CHARACTER_SELECT, pCharSel) );
	cStage_NetworkSelect* pNetSel = new cStage_NetworkSelect;
	m_list.insert( std::make_pair(NETWORK_SELECT, pNetSel) );
	cStage_NetworkLoading* pNetload = new cStage_NetworkLoading;
	m_list.insert( std::make_pair(NETWORK_LOADING, pNetload) );
	cStage_IngameStart* pInSta = new cStage_IngameStart;
	m_list.insert( std::make_pair(INGAMESTART, pInSta) );
	cStage_Ingame* pIn = new cStage_Ingame;
	m_list.insert( std::make_pair(INGAME, pIn) );
	cStage_IngameEnd* pInEnd = new cStage_IngameEnd;
	m_list.insert( std::make_pair(INGAMEEND, pInEnd) );
	cStage_Ending* pEnding = new cStage_Ending;
	m_list.insert( std::make_pair(ENDING, pEnding) );

	m_stage = pMain;
	m_currentStage = MAIN;

	m_socket = 0;
//	m_ip.SetAddress(192,168,168,101);
	m_port = 10000;
	m_ip = "192.168.168.101";  //우리집 ip주소

//외부에서 편집한 ip주소 읽어오기
	ifstream fsIn("../media/ip.txt");
	if( fsIn.fail() )
		return;

	string buff1, buff2, buff3;
	::getline( fsIn, buff1 );
	::getline( fsIn, buff1 );
	fsIn >> buff1;
	fsIn >> buff2;
	fsIn >> buff3;
	m_ip = buff3;

	InitSound();

	m_buttonChange1 = false;
	m_buttonChange2 = false;
}

cStageMgr::~cStageMgr()
{
	for(auto it = m_list.begin(); it != m_list.end();  )
	{
		delete it->second;
		it = m_list.erase(it);
	}
}

void cStageMgr::Release()
{
	closesocket(m_socket);
}

cStage* cStageMgr::FindStage(const int nId)
{
	auto it = m_list.find(nId);
	
	if( it == m_list.end() )
		return NULL;

	return it->second;
}

bool cStageMgr::SetSocket()
{
	/*
	DWORD address;
	m_ip.GetAddress(address);

	std::stringstream ss;
	ss << ((address & 0xff000000) >> 24) << "." 
		<< ((address & 0x00ff0000) >> 16) << "." 
		<< ((address & 0x0000ff00) >> 8) << "." 
		<< (address & 0x000000ff);
	
	const string ip = ss.str();
	*/
	if ( false == network::LaunchClient( m_ip, m_port, m_socket ) )
		return false;

	return true;
}

void cStageMgr::SetButtonChange(const short sBtnNumber, const bool bChange)
{
	switch(sBtnNumber)
	{
	case 1:
		m_buttonChange1 = bChange;
		break;
	case 2:
		m_buttonChange2 = bChange;
		break;
	}
}

bool cStageMgr::GetButtonChange(const short sBtnNumber) const
{
	switch(sBtnNumber)
	{
	case 1:
		return m_buttonChange1;
		break;
	case 2:
		return m_buttonChange2;
		break;
	}

	return false;
}

void cStageMgr::InitSound()
{
	framework::Sound* pSound1 = new framework::Sound();
	pSound1->Load( "../media/sound/main_tema_01.mp3" );
	pSound1->SetLoop(true);
	SndDepot->push( "MainTema1", pSound1 );
	
	framework::Sound* pSound2 = new framework::Sound();
	pSound2->Load( "../media/sound/main_tema_02.mp3" );
	pSound2->SetLoop(false);
	SndDepot->push( "MainTema2", pSound2 );
	
	framework::Sound* pSound3 = new framework::Sound();
	pSound3->Load( "../media/sound/main_tema_03.mp3" );
	pSound3->SetLoop(false);
	SndDepot->push( "MainTema3", pSound3 );

	framework::Sound* pSound4 = new framework::Sound();
	pSound4->Load( "../media/sound/main_tema_04.mp3" );
	pSound4->SetLoop(false);
	SndDepot->push( "MainTema4", pSound4 );

	framework::Sound* pSound5 = new framework::Sound();
	pSound5->Load( "../media/sound/main_tema_05.mp3" );
	pSound5->SetLoop(false);
	SndDepot->push( "MainTema5", pSound5 );

	framework::Sound* pSound6 = new framework::Sound();
	pSound6->Load( "../media/sound/Victory.ogg" );
	pSound6->SetLoop(false);
	SndDepot->push( "Victory", pSound6 );

	framework::Sound* pSound7 = new framework::Sound();
	pSound7->Load( "../media/sound/Defeat.ogg" );
	pSound7->SetLoop(false);
	SndDepot->push( "Defeat", pSound7 );

	framework::Sound* pSound8 = new framework::Sound();
	pSound8->Load( "../media/sound/Click_Button.ogg" );
	pSound8->SetLoop(false);
	SndDepot->push( "Button", pSound8 );

	framework::Sound* pSound9 = new framework::Sound();
	pSound9->Load( "../media/sound/Valle_Shout_02.ogg" );
	pSound9->SetLoop(false);
	SndDepot->push( "0", pSound9 );

	framework::Sound* pSound10 = new framework::Sound();
	pSound10->Load( "../media/sound/Valle_Shout_03.ogg" );
	pSound10->SetLoop(false);
	SndDepot->push( "1", pSound10 );

	framework::Sound* pSound11 = new framework::Sound();
	pSound11->Load( "../media/sound/Valle_Shout_04.ogg" );
	pSound11->SetLoop(false);
	SndDepot->push( "2", pSound11 );

	framework::Sound* pSound12 = new framework::Sound();
	pSound12->Load( "../media/sound/Valle_Shout_11.ogg" );
	pSound12->SetLoop(false);
	SndDepot->push( "3", pSound12 );

	framework::Sound* pSound13 = new framework::Sound();
	pSound13->Load( "../media/sound/Valle_Groan_10.ogg" );
	pSound13->SetLoop(false);
	SndDepot->push( "10", pSound13 );

	framework::Sound* pSound14 = new framework::Sound();
	pSound14->Load( "../media/sound/Valle_Groan_12.ogg" );
	pSound14->SetLoop(false);
	SndDepot->push( "11", pSound14 );

	framework::Sound* pSound15 = new framework::Sound();
	pSound15->Load( "../media/sound/Valle_Groan_14.ogg" );
	pSound15->SetLoop(false);
	SndDepot->push( "12", pSound15 );

	framework::Sound* pSound16 = new framework::Sound();
	pSound16->Load( "../media/sound/Valle_Groan_15.ogg" );
	pSound16->SetLoop(false);
	SndDepot->push( "13", pSound16 );

	framework::Sound* pSound17 = new framework::Sound();
	pSound17->Load( "../media/sound/Valle_Guard_Start.ogg" );
	pSound17->SetLoop(false);
	SndDepot->push( "Guard", pSound17 );

	framework::Sound* pSound18 = new framework::Sound();
	pSound18->Load( "../media/sound/Ready.ogg" );
	pSound18->SetLoop(false);
	SndDepot->push( "Ready", pSound18 );

	framework::Sound* pSound19 = new framework::Sound();
	pSound19->Load( "../media/sound/Start_2.ogg" );
	pSound19->SetLoop(false);
	SndDepot->push( "Start", pSound19 );
}