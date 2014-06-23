
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


CViewerDlg::CViewerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CViewerDlg::IDD, pParent),
	m_bLoop(true) ,m_pModelView(NULL)
//	, m_strOK(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_FILE_LIST, m_fileList);
}

BEGIN_MESSAGE_MAP(CViewerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
//	ON_BN_CLICKED(IDC_BUTTON_A, &CViewerDlg::OnBnClickedButtonA)
//	ON_WM_LBUTTONDOWN()
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_FILE_LIST, &CViewerDlg::OnItemchangedFileList)
	ON_BN_CLICKED(IDCANCEL, &CViewerDlg::OnBnClickedCancel)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()


// CViewerDlg �޽��� ó����

BOOL CViewerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �ý��� �޴��� "����..." �޴� �׸��� �߰��մϴ�.

	// IDM_ABOUTBOX�� �ý��� ��� ������ �־�� �մϴ�.
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

	MoveWindow(0, 0, 1024, 768);

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	m_pModelView = new CModelView();
	m_pModelView->Create(NULL, L"CView", WS_CHILDWINDOW, 
		CRect(0,0,800,600), this, 0);

	
	graphic::cRenderer::Get()->CreateDirectX(m_pModelView->GetSafeHwnd(), 800, 600);
	m_pModelView->Init();
	
	m_pModelView->ShowWindow(SW_SHOW);

	m_fileList.InsertColumn(0, L"Path");  //��� �߰�
	m_fileList.SetColumnWidth(0, 300);
	//m_fileList.InsertItem(0, L"Test1");  //������ �߰�
	//m_fileList.InsertItem(1, L"Test2");
	//m_fileList.InsertItem(2, L"Test3");
	//m_fileList.InsertItem(3, L"Test4");

	DragAcceptFiles(TRUE);


	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

void CViewerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
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

BOOL CViewerDlg::PreTranslateMessage(MSG* pMsg)  //�޽����� ���� �� �޴� �Լ�(���ν����Լ����ٵ�)
{
	// TODO: ���⿡ Ư��ȭ�� �ڵ带 �߰� ��/�Ǵ� �⺻ Ŭ������ ȣ���մϴ�.

	return CDialogEx::PreTranslateMessage(pMsg);
}


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

		if( m_pModelView )
			m_pModelView->Render();

		::Sleep(0);
	}
}

void CViewerDlg::OnItemchangedFileList(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	*pResult = 0;

//	CString str = m_fileList.GetItemText(pNMLV->iItem, 0);
//	AfxMessageBox(str);
}


void CViewerDlg::OnBnClickedCancel()
{
	m_bLoop = false;
	CDialogEx::OnCancel();
}


void CViewerDlg::OnDropFiles(HDROP hDropInfo)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	HDROP hdrop = hDropInfo;
	WCHAR filePath[MAX_PATH];
	const UINT size = DragQueryFile(hdrop, 0, filePath, MAX_PATH);  //������ ������ ���� ������
	if(size == 0)
		return;

	m_fileList.InsertItem(m_fileList.GetItemCount(), filePath);

	int AnsiStrSize = WideCharToMultiByte(CP_ACP, 0, filePath, -1, NULL, 0, NULL, NULL); 
	//�ش� ������ ��ŭ �Ҵ� ���ְ�
	char *AnsiStr = new char[AnsiStrSize];
	//��ȯ�ϼ���
	WideCharToMultiByte(CP_ACP, 0, filePath, -1, AnsiStr, AnsiStrSize, 0, 0);
		
	m_pModelView->FileLoad( format(AnsiStr) );

	delete []AnsiStr;

	CDialogEx::OnDropFiles(hDropInfo);  //������ ������ ������ �� ������ ��ΰ� �����
}
