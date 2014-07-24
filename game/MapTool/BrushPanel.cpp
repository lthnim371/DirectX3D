// cBrushPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "BrushPanel.h"
#include "afxdialogex.h"


// cBrushPanel 대화 상자입니다.

cBrushPanel::cBrushPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(cBrushPanel::IDD, pParent)
{

}

cBrushPanel::~cBrushPanel()
{
}

void cBrushPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cBrushPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &cBrushPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &cBrushPanel::OnBnClickedCancel)
END_MESSAGE_MAP()


// cBrushPanel 메시지 처리기입니다.


void cBrushPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnOK();
}


void cBrushPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnCancel();
}
