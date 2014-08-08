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
}

cStageMgr::~cStageMgr()
{
	for(auto it = m_list.begin(); it != m_list.end();  )
	{
		delete it->second;
		it = m_list.erase(it);
	}
}

cStage* cStageMgr::FindStage(const int nId)
{
	auto it = m_list.find(nId);
	
	if( it == m_list.end() )
		return NULL;

	return it->second;
}