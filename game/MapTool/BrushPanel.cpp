// cBrushPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "BrushPanel.h"
#include "afxdialogex.h"


// cBrushPanel ��ȭ �����Դϴ�.

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


// cBrushPanel �޽��� ó�����Դϴ�.


void cBrushPanel::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CDialogEx::OnOK();
}


void cBrushPanel::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CDialogEx::OnCancel();
}
