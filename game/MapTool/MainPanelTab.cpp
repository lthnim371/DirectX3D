// MainPanelTab.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MainPanelTab.h"
#include "afxdialogex.h"

#include "TerrainPanel.h"
#include "BrushPanel.h"

// cMainPanelTab 대화 상자입니다.


cMainPanelTab::cMainPanelTab(CWnd* pParent /*=NULL*/)
	: CDialogEx(cMainPanelTab::IDD, pParent)
{

}

cMainPanelTab::~cMainPanelTab()
{
}

void cMainPanelTab::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(cMainPanelTab, CDialogEx)
	ON_BN_CLICKED(IDOK, &cMainPanelTab::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cMainPanelTab::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &cMainPanelTab::OnSelchangeTab1)
END_MESSAGE_MAP()


// cMainPanelTab 메시지 처리기입니다.

void cMainPanelTab::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnOK();
}


void cMainPanelTab::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnCancel();
}

BOOL cMainPanelTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_Tab.InsertItem(0, L"Terrain1");
	m_Tab.InsertItem(1, L"Terrain2");
	m_Tab.InsertItem(2, L"Terrain3");

	CRect cr;
	m_Tab.GetClientRect(cr);

	m_terrain = new cTerrainPanel(this);
	m_terrain->Create( cTerrainPanel::IDD, this );
	m_terrain->MoveWindow(CRect(0, 30, cr.Width(), cr.Height()));
	m_terrain->ShowWindow(SW_SHOW);

	m_brush = new cBrushPanel(this);
	m_brush->Create( cBrushPanel::IDD, this );
	m_brush->MoveWindow(CRect(0, 30, cr.Width(), cr.Height()));
	m_brush->ShowWindow(SW_HIDE);


	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void cMainPanelTab::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	switch( m_Tab.GetCurSel() )
	{
	case 0:
		m_terrain->ShowWindow(SW_SHOW);
		m_brush->ShowWindow(SW_HIDE);
		break;
	
	case 1:
		m_terrain->ShowWindow(SW_HIDE);
		m_brush->ShowWindow(SW_SHOW);
		break;
	}

	*pResult = 0;
}