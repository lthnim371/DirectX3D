// MainPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MainPanel.h"
#include "afxdialogex.h"

#include "MapView.h"
#include <fstream>

CMainPanel* g_mainPanel = NULL;  //���� ��������
// CMainPanel ��ȭ �����Դϴ�.

CMainPanel::CMainPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMainPanel::IDD, pParent)
{
	g_mainPanel = this;
}

CMainPanel::~CMainPanel()
{
}

void CMainPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_CUBE, m_cubeList);
}


BEGIN_MESSAGE_MAP(CMainPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CMainPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CMainPanel::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST_CUBE, &CMainPanel::OnLbnSelchangeListCube)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMainPanel::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &CMainPanel::OnBnClickedButtonLoad)
END_MESSAGE_MAP()


// CMainPanel �޽��� ó�����Դϴ�.


void CMainPanel::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnOK();
}


void CMainPanel::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	CDialogEx::OnCancel();
}


void CMainPanel::InsertCube(graphic::cCube& cube)
{
	CString str;
	str.Format( L"%d", m_cubeList.GetCount());
	m_cubeList.InsertString( m_cubeList.GetCount(), str);
}

void CMainPanel::OnLbnSelchangeListCube()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	int idx = m_cubeList.GetCurSel();
	g_mapView->SelectCube(idx);
}



void CMainPanel::OnBnClickedButtonSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	FILE* fp = fopen("mapdata.mda", "wb");  //w : write //r : read  //b : binary
//	fwrite
//	fclose(fp);  //�������� �ݾ���� ��
	using namespace std;

	ofstream fs;
	fs.open("mapdata.mda", ios_base::binary);

	int cubeSize = g_cubes.size();
	fs.write((char*)&cubeSize, sizeof(int));

	for(int i=0; i<(int)g_cubes.size(); ++i)
	{
		fs.write((char*)&g_cubes[i]->GetMin(), sizeof(Vector3));
		fs.write((char*)&g_cubes[i]->GetMax(), sizeof(Vector3));
		fs.write((char*)&g_cubes[i]->GetTransform(), sizeof(Matrix44));
	}

	::AfxMessageBox(L"Save Complete");
}


void CMainPanel::OnBnClickedButtonLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	using namespace std;

	for(int i=0; i<(int)g_cubes.size(); ++i)
	{
		delete g_cubes[i];
	}
	g_cubes.clear();
	
	while( 0 < m_cubeList.GetCount())
	{
		m_cubeList.DeleteString(0);
	}
	

	ifstream fs;
	fs.open("mapdata.mda", ios_base::binary);

	int cubeSize;
	fs.read((char*)&cubeSize, sizeof(int));

	for(int i=0; i<cubeSize; ++i)
	{
		Vector3 min, max;
		Matrix44 tm;
		fs.read((char*)&min, sizeof(Vector3));
		fs.read((char*)&max, sizeof(Vector3));
		fs.read((char*)&tm, sizeof(Matrix44));

		graphic::cCube* newCube = new graphic::cCube();
		newCube->SetCube(min, max);
		newCube->SetTransform(tm);
		newCube->SetColor(0xFF0000FF);
		g_cubes.push_back(newCube);

		CString str;
		str.Format(L"%d", m_cubeList.GetCount());  //printf�� ����ϴٰ� ���� ��(����� �ƴ�)
		m_cubeList.InsertString(m_cubeList.GetCount(), str);
	}

//	::AfxMessageBox(L"Save Complete");
}
