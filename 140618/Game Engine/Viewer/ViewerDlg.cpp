
// ViewerDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "Viewer.h"
#include "ViewerDlg.h"
#include "afxdialogex.h"
#include "ModelView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();
	

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CViewerDlg ��ȭ ����




CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewerDlg::IDD, pParent),
	m_bLoop(true) ,p(NULL)
//	, m_strOK(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON_A, &CViewerDlg::OnBnClickedButtonA)
//	ON_WM_LBUTTONDOWN()
ON_BN_CLICKED(IDOK, &CViewerDlg::OnBnClickedOk)
ON_BN_CLICKED(IDCANCEL, &CViewerDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// CViewerDlg �޽��� ó����

BOOL CViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ���� ������ �־�� �մϴ�.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	MoveWindow(0, 0, 800, 600);

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	p = new CModelView();
	p->Create(NULL, L"CView", WS_CHILDWINDOW, 
		CRect(0,0,500,500), this, 0);

	graphic::cRenderer::Get()->CreateDirectX(p->GetSafeHwnd(), 500, 500);
	
	p->ShowWindow(SW_SHOW);


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CViewerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CViewerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



//void CViewerDlg::OnBnClickedButtonA()
//{
//	m_strOK = "asdjklf";
//	UpdateData(FALSE);  //UI??�� �����͸� �ްų� ������ ��� 
//	//AfxMessageBox(m_strOK);  //�޽��� ���
//}


LRESULT CViewerDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	//API�Լ� ����...�׷��� MFC������ �̷��� ���� �������Ӥ�
	/*switch(message)
	{
		case WM_LBUTTONDOWN:
		{
			int i = 10;
		}
		break;
	}*/

	return CDialogEx::WindowProc(message, wParam, lParam);
}

//BOOL CViewerDlg::PreTranslateMessage(MSG* pMsg)  //�޽����� ���� �� �޴� �Լ�(���ν����Լ����ٵ�)
//{
//	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.
//
//	return CDialogEx::PreTranslateMessage(pMsg);
//}


//void CViewerDlg::OnLButtonDown(UINT nFlags, CPoint point)
//{
//	CString str; 
//	ClientToScreen(&point);
//	str.Format(L"%d, %d", point.x, point.y);
////	AfxMessageBox(str);  //���ڿ� �ش��ϴ� ������ ����ϴ� �ڽ�
//
//	CDialogEx::OnLButtonDown(nFlags, point);
//}


void CViewerDlg::MainProc()
{	
	while( m_bLoop )
	{
		MSG msg;
		if(::PeekMessageA(&msg, NULL, 0, 0, PM_NOREMOVE))
		{
			if(!GetMessage(&msg, NULL, 0, 0))
			{
				break;
			}
			::TranslateMessage(&msg);
			::DispatchMessageA(&msg);
		}

		::Sleep(0);
	}
}

void CViewerDlg::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.

	m_bLoop = false;
	CDialogEx::OnOK();
}



void CViewerDlg::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	m_bLoop = false;
	CDialogEx::OnCancel();
}