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
	cStage_Ingame* pIn = new cStage_Ingame;
	m_list.insert( std::make_pair(INGAME, pIn) );
	cStage_IngameEnd* pInEnd = new cStage_IngameEnd;
	m_list.insert( std::make_pair(INGAMEEND, pInEnd) );

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
	fsIn >> buff1;
	fsIn >> buff2;
	fsIn >> buff3;
	m_ip = buff3;
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