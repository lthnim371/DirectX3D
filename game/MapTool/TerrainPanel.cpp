// cTerrainPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TerrainPanel.h"
#include "afxdialogex.h"


// cTerrainPanel ��ȭ �����Դϴ�.

cTerrainPanel::cTerrainPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(cTerrainPanel::IDD, pParent)
{

}

cTerrainPanel::~cTerrainPanel()
{
}

void cTerrainPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cTerrainPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &cTerrainPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cTerrainPanel::OnBnClickedCancel)
END_MESSAGE_MAP()


// cTerrainPanel �޽��� ó�����Դϴ�.


void cTerrainPanel::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CDialogEx::OnOK();
}


void cTerrainPanel::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CDialogEx::OnCancel();
}
