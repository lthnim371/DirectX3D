#include "stdafx.h"
#include "stagemanager.h"

using namespace framework;

cStageMgr::cStageMgr()
{
	cStage_Main* pMain = new cStage_Main;
	m_list.insert( std::make_pair(MAIN, pMain) );
	cStage_Select* pSel = new cStage_Select;
	m_list.insert( std::make_pair(SELECT, pSel) );
	cStage_Ingame* pIn = new cStage_Ingame;
	m_list.insert( std::make_pair(INGAME, pIn) );

	m_stage = pMain;
	m_currentStage = MAIN;

	m_socket = 0;
//	m_ip.SetAddress(192,168,168,101);
	m_ip = "192.168.168.101";
	m_port = 10000;
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