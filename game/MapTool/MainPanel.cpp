// MainPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MainPanel.h"

#include "MainPanelTab.h"

// cMainPanel

cMainPanel::cMainPanel()
{

}

cMainPanel::~cMainPanel()
{
}


BEGIN_MESSAGE_MAP(cMainPanel, CMiniFrameWnd)
END_MESSAGE_MAP()


// cMainPanel �޽��� ó�����Դϴ�.


void cMainPanel::Init()
{
	CRect cr;
	GetClientRect(cr);

	m_mainPanel = new cMainPanelTab(this);
	m_mainPanel->Create( cMainPanelTab::IDD, this );
	m_mainPanel->MoveWindow(cr);
	m_mainPanel->ShowWindow(SW_SHOW);
}