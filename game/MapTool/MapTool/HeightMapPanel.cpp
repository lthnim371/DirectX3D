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
,	m_texture(NULL)
{

}

CHeightMapPanel::~CHeightMapPanel()
{
}

void CHeightMapPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_HEIGHTMAP, m_HeightMapList);
	DDX_Control(pDX, IDC_LIST_HEIGHTMAP_TEX, m_TextureList);
}


BEGIN_MESSAGE_MAP(CHeightMapPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CHeightMapPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CHeightMapPanel::OnBnClickedCancel)
	ON_LBN_DBLCLK(IDC_LIST_HEIGHTMAP, &CHeightMapPanel::OnDblclkListHeightmap)
	ON_LBN_SELCHANGE(IDC_LIST_HEIGHTMAP, &CHeightMapPanel::OnSelchangeListHeightmap)
	ON_WM_PAINT()
	ON_LBN_SELCHANGE(IDC_LIST_HEIGHTMAP_TEX, &CHeightMapPanel::OnSelchangeListHeightmapTex)
	ON_LBN_DBLCLK(IDC_LIST_HEIGHTMAP_TEX, &CHeightMapPanel::OnDblclkListHeightmapTex)
END_MESSAGE_MAP()


// CHeightMapPanel �޽��� ó�����Դϴ�.
BOOL CHeightMapPanel::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	UpdateHeightMapList();
	UpdateTextureList();

	return TRUE;
}


void CHeightMapPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Graphics *graph = Graphics::FromHDC(dc.GetSafeHdc());

	if (m_heightMap)
	{
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

	if (m_texture)
	{
		// �ؽ��� ���.
		if (CWnd *pictureCtlr = GetDlgItem(IDC_STATIC_TEXTURE2))
		{
			CRect cr;
			pictureCtlr->GetWindowRect(cr);
			ScreenToClient(cr);

			const Rect dest(cr.left, cr.top, cr.Width(), cr.Height());
			graph->DrawImage(m_texture, dest );
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
	common::CollectFiles(extList, "../../media/terrain/", heightMapFiles);

	BOOST_FOREACH(auto &fileName, heightMapFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_HeightMapList.InsertString(m_HeightMapList.GetCount(), wstr.c_str());
	}
}


// ���� �ؽ��� ����Ʈ ���.
void CHeightMapPanel::UpdateTextureList()
{
	// ����Ʈ �ڽ� �ʱ�ȭ.
	while (0 < m_TextureList.GetCount())
		m_TextureList.DeleteString(0);

	// ���� ã��.
	list<string> extList;
	extList.push_back("jpg");
	extList.push_back("png");
	extList.push_back("bmp");
	list<string> textureFiles;
	common::CollectFiles(extList, "../../media/terrain/", textureFiles);

	BOOST_FOREACH(auto &fileName, textureFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_TextureList.InsertString(m_TextureList.GetCount(), wstr.c_str());
	}
}


// ���� ���̸� ����.
void CHeightMapPanel::OnDblclkListHeightmap()
{
	RET(m_HeightMapList.GetCurSel() < 0);

	CString fileName;
	m_HeightMapList.GetText(m_HeightMapList.GetCurSel(), fileName);
	SAFE_DELETE(m_heightMap);
	m_heightMap = Image::FromFile(fileName);
	InvalidateRect(NULL, FALSE);

	string asciiFileName = wstr2str((wstring)fileName);
	cMapController::Get()->LoadHeightMap(asciiFileName);
}


// ���� ���̸� �̹��� ������Ʈ
void CHeightMapPanel::OnSelchangeListHeightmap()
{
	RET(m_HeightMapList.GetCurSel() < 0);

	CString fileName;
	m_HeightMapList.GetText(m_HeightMapList.GetCurSel(), fileName);
	SAFE_DELETE(m_heightMap);
	m_heightMap = Image::FromFile(fileName);
	InvalidateRect(NULL, FALSE);
}


// ���� �ؽ��� �̹��� ������Ʈ
void CHeightMapPanel::OnSelchangeListHeightmapTex()
{
	RET(m_TextureList.GetCurSel() < 0);

	CString fileName;
	m_TextureList.GetText(m_TextureList.GetCurSel(), fileName);
	SAFE_DELETE(m_texture);
	m_texture = Image::FromFile(fileName);
	InvalidateRect(NULL, FALSE);	
}


// ���� �ؽ��� ����.
void CHeightMapPanel::OnDblclkListHeightmapTex()
{
	RET(m_TextureList.GetCurSel() < 0);

	CString fileName;
	m_TextureList.GetText(m_TextureList.GetCurSel(), fileName);
	SAFE_DELETE(m_texture);
	m_texture = Image::FromFile(fileName);
	InvalidateRect(NULL, FALSE);

	string asciiFileName = wstr2str((wstring)fileName);
	cMapController::Get()->LoadHeightMapTexture(asciiFileName);
}
