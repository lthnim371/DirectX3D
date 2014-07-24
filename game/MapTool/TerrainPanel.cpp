// cTerrainPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "TerrainPanel.h"
#include "afxdialogex.h"


// cTerrainPanel 대화 상자입니다.

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


// cTerrainPanel 메시지 처리기입니다.


void cTerrainPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnOK();
}


void cTerrainPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnCancel();
}
