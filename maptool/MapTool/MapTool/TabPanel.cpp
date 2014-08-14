// TabPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TabPanel.h"
#include "afxdialogex.h"
#include "HeightMapPanel.h"
#include "TerrainPanel.h"
#include "BrushPanel.h"
#include "ModelPanel.h"
//�߰�
#include "ObjectPanel.h"

// CTabPanel ��ȭ �����Դϴ�.
CTabPanel::CTabPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTabPanel::IDD, pParent)
,	m_modelPanel(NULL)
{

}

CTabPanel::~CTabPanel()
{
	SAFE_DELETE(m_heightMapPanel);
	SAFE_DELETE(m_terrainPanel);
	SAFE_DELETE(m_brushPanel);
	SAFE_DELETE(m_modelPanel);
//�߰�
	SAFE_DELETE(m_objectPanel);
}

void CTabPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB1, m_Tab);
}


BEGIN_MESSAGE_MAP(CTabPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CTabPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CTabPanel::OnBnClickedCancel)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB1, &CTabPanel::OnSelchangeTab1)
END_MESSAGE_MAP()


// CTabPanel �޽��� ó�����Դϴ�.


void CTabPanel::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CTabPanel::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}


BOOL CTabPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	m_Tab.InsertItem(0, L"HeightMap");
	m_Tab.InsertItem(1,  L"Terrain");
	m_Tab.InsertItem(2,  L"Brush");
	m_Tab.InsertItem(3, L"Model");
//�߰�
	m_Tab.InsertItem(4,  L"Object");

	CRect cr;
	GetClientRect(cr);

	m_heightMapPanel = new CHeightMapPanel(this);
	m_heightMapPanel->Create(CHeightMapPanel::IDD, this);
	m_heightMapPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_heightMapPanel->ShowWindow(SW_SHOW);
	cMapController::Get()->AddObserver(m_heightMapPanel);


	m_terrainPanel = new CTerrainPanel(this);
	m_terrainPanel->Create(CTerrainPanel::IDD, this);
	m_terrainPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_terrainPanel->ShowWindow(SW_HIDE);
	cMapController::Get()->AddObserver(m_terrainPanel);


	m_brushPanel = new CBrushPanel(this);
	m_brushPanel->Create(CBrushPanel::IDD, this);
	m_brushPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_brushPanel->ShowWindow(SW_HIDE);
	cMapController::Get()->AddObserver(m_brushPanel);


	m_modelPanel = new CModelPanel(this);
	m_modelPanel->Create(CModelPanel::IDD, this);
	m_modelPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_modelPanel->ShowWindow(SW_HIDE);
	cMapController::Get()->AddObserver(m_modelPanel);

//�߰�
	m_objectPanel = new CObjectPanel(this);
	m_objectPanel->Create(CObjectPanel::IDD, this);
	m_objectPanel->MoveWindow(CRect(0, 25, cr.Width(), cr.Height()));
	m_objectPanel->ShowWindow(SW_HIDE);
	cMapController::Get()->AddObserver(m_objectPanel);

	return TRUE;
}


void CTabPanel::OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult)
{
	switch (m_Tab.GetCurSel())
	{
	case 0:
		m_heightMapPanel->ShowWindow(SW_SHOW);
		m_terrainPanel->ShowWindow(SW_HIDE);
		m_brushPanel->ShowWindow(SW_HIDE);
		m_modelPanel->ShowWindow(SW_HIDE);
		m_objectPanel->ShowWindow(SW_HIDE);
		cMapController::Get()->ChangeEditMode(EDIT_MODE::MODE_HEIGHTMAP);
		break;

	case 1:
		m_terrainPanel->ShowWindow(SW_SHOW);
		m_heightMapPanel->ShowWindow(SW_HIDE);
		m_brushPanel->ShowWindow(SW_HIDE);
		m_modelPanel->ShowWindow(SW_HIDE);
		m_objectPanel->ShowWindow(SW_HIDE);
		cMapController::Get()->ChangeEditMode(EDIT_MODE::MODE_TERRAIN);
		break;

	case 2:
		m_brushPanel->ShowWindow(SW_SHOW);
		m_terrainPanel->ShowWindow(SW_HIDE);
		m_heightMapPanel->ShowWindow(SW_HIDE);
		m_modelPanel->ShowWindow(SW_HIDE);
		m_objectPanel->ShowWindow(SW_HIDE);
		cMapController::Get()->ChangeEditMode(EDIT_MODE::MODE_BRUSH);
		break;

	case 3:
		m_modelPanel->ShowWindow(SW_SHOW);
		m_brushPanel->ShowWindow(SW_HIDE);
		m_terrainPanel->ShowWindow(SW_HIDE);
		m_heightMapPanel->ShowWindow(SW_HIDE);
		m_objectPanel->ShowWindow(SW_HIDE);
		cMapController::Get()->ChangeEditMode(EDIT_MODE::MODE_MODEL);
		break;

//�߰� : ���� �ڵ忡�� �߰�
	case 4:
		m_objectPanel->ShowWindow(SW_SHOW);
		m_terrainPanel->ShowWindow(SW_HIDE);
		m_heightMapPanel->ShowWindow(SW_HIDE);
		m_brushPanel->ShowWindow(SW_HIDE);
		m_modelPanel->ShowWindow(SW_HIDE);
		cMapController::Get()->ChangeEditMode(EDIT_MODE::MODE_OBJECT);
		break;
	}

	*pResult = 0;
}
