// MainPanelTab.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MainPanelTab.h"
#include "afxdialogex.h"

#include "TerrainPanel.h"
#include "BrushPanel.h"

// cMainPanelTab ��ȭ �����Դϴ�.


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


// cMainPanelTab �޽��� ó�����Դϴ�.

void cMainPanelTab::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CDialogEx::OnOK();
}


void cMainPanelTab::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CDialogEx::OnCancel();
}

BOOL cMainPanelTab::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

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
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}

void cMainPanelTab::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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