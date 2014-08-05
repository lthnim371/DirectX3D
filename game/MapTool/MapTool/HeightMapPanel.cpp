// HeightMapPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "HeightMapPanel.h"
#include "afxdialogex.h"


// CHeightMapPanel ��ȭ �����Դϴ�.

IMPLEMENT_DYNAMIC(CHeightMapPanel, CDialogEx)

CHeightMapPanel::CHeightMapPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CHeightMapPanel::IDD, pParent)
,	m_heightMap(NULL)
{

}

CHeightMapPanel::~CHeightMapPanel()
{
}

void CHeightMapPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HEIGHTMAP, m_HeightMapList);
}


BEGIN_MESSAGE_MAP(CHeightMapPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHeightMapPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHeightMapPanel::OnBnClickedCancel)
	ON_LBN_DBLCLK(IDC_LIST_HEIGHTMAP, &CHeightMapPanel::OnDblclkListHeightmap)
	ON_LBN_SELCHANGE(IDC_LIST_HEIGHTMAP, &CHeightMapPanel::OnSelchangeListHeightmap)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// CHeightMapPanel �޽��� ó�����Դϴ�.
BOOL CHeightMapPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateHeightMapList();

	return TRUE;
}


void CHeightMapPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (m_heightMap)
	{
		Graphics *graph = Graphics::FromHDC(dc.GetSafeHdc());

		// �ؽ��� ���.
		if (CWnd *pictureCtlr = GetDlgItem(IDC_STATIC_TEXTURE))
		{
			CRect cr;
			pictureCtlr->GetWindowRect(cr);
			ScreenToClient(cr);

			const Rect dest(cr.left, cr.top, cr.Width(), cr.Height());
			graph->DrawImage(m_heightMap, dest );
		}
	}
}


void CHeightMapPanel::OnBnClickedOk()
{
	//CDialogEx::OnOK();
}


void CHeightMapPanel::OnBnClickedCancel()
{
	//CDialogEx::OnCancel();
}


void CHeightMapPanel::UpdateHeightMapList()
{
	// ����Ʈ �ڽ� �ʱ�ȭ.
	while (0 < m_HeightMapList.GetCount())
		m_HeightMapList.DeleteString(0);

	// ���� ã��.
	list<string> extList;
	extList.push_back("jpg");
	extList.push_back("png");
	extList.push_back("bmp");
	list<string> heightMapFiles;
	common::CollectFiles(extList, "../../media/", heightMapFiles);

	BOOST_FOREACH(auto &fileName, heightMapFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_HeightMapList.InsertString(m_HeightMapList.GetCount(), wstr.c_str());
	}
}


void CHeightMapPanel::OnDblclkListHeightmap()
{
	CString fileName;
	m_HeightMapList.GetText(m_HeightMapList.GetCurSel(), fileName);
	SAFE_DELETE(m_heightMap);
	m_heightMap = Image::FromFile(fileName);
	InvalidateRect(NULL, FALSE);

	string asciiFileName = wstr2str((wstring)fileName);
	cMapController::Get()->LoadHeightMap(asciiFileName);
}


void CHeightMapPanel::OnSelchangeListHeightmap()
{
	CString fileName;
	m_HeightMapList.GetText(m_HeightMapList.GetCurSel(), fileName);
	SAFE_DELETE(m_heightMap);
	m_heightMap = Image::FromFile(fileName);
	InvalidateRect(NULL, FALSE);
}
