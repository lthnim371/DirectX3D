// TopPanel.cpp : ���� �����Դϴ�.
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


// CTopPanel �޽��� ó�����Դϴ�.

void CTopPanel::Init()
{
	CRect cr;
	GetClientRect(cr);  //���� ��ü�� (this = TopPanel) ������â ������

	m_mainPanel = new CMainPanel(this);  //�ڽ� �������� ������ �θ� �ּҸ� �ѱ��.(�θ�� ������Ŵ, �� �ڽ� ������ �Ӽ� ���ϵ��)
	m_mainPanel->Create( CMainPanel::IDD, this );
	m_mainPanel->MoveWindow(cr);
	m_mainPanel->ShowWindow(SW_SHOW);
}
