// TopPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TopPanel.h"
#include "MainPanel.h"

// CTopPanel

CTopPanel::CTopPanel()
{

}

CTopPanel::~CTopPanel()
{
}


BEGIN_MESSAGE_MAP(CTopPanel, CMiniFrameWnd)
END_MESSAGE_MAP()


// CTopPanel 메시지 처리기입니다.

void CTopPanel::Init()
{
	CRect cr;
	GetClientRect(cr);  //현재 객체의 (this = TopPanel) 윈도우창 사이즈

	m_mainPanel = new CMainPanel(this);  //자식 윈도우의 생성때 부모 주소를 넘긴다.(부모로 인지시킴, 단 자식 윈도우 속성 차일드로)
	m_mainPanel->Create( CMainPanel::IDD, this );
	m_mainPanel->MoveWindow(cr);
	m_mainPanel->ShowWindow(SW_SHOW);
}
