// BrushPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "BrushPanel.h"
#include "afxdialogex.h"


// CBrushPanel ��ȭ �����Դϴ�.

CBrushPanel::CBrushPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CBrushPanel::IDD, pParent)
,	m_texture(NULL)
{

}

CBrushPanel::~CBrushPanel()
{
}

void CBrushPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TEXTURE_FILES, m_TextureFiles);
	DDX_Control(pDX, IDC_MFCEDITBROWSE_TEXTURE, m_textureBrowser);
	DDX_Control(pDX, IDC_LIST_LAYER, m_layerList);
}


BEGIN_MESSAGE_MAP(CBrushPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CBrushPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CBrushPanel::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST_TEXTURE_FILES, &CBrushPanel::OnSelchangeListTextureFiles)
	ON_WM_PAINT()
	ON_EN_CHANGE(IDC_MFCEDITBROWSE_TEXTURE, &CBrushPanel::OnChangeMfceditbrowseTexture)
END_MESSAGE_MAP()


// CBrushPanel �޽��� ó�����Դϴ�.
BOOL CBrushPanel::OnInitDialog()
{
	__super::OnInitDialog();

	m_textureBrowser.EnableFolderBrowseButton();
	m_textureBrowser.SetWindowText( L"../../media/terrain/" );
	UpdateTextureFiles("../../media/terrain/");

	return TRUE;
}


void CBrushPanel::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnOK();
}


void CBrushPanel::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	//CDialogEx::OnCancel();
}


void CBrushPanel::Update()
{

}


void CBrushPanel::UpdateTextureFiles(const string &directoryPath)
{
	// ����Ʈ �ڽ� �ʱ�ȭ.
	while (0 < m_TextureFiles.GetCount())
		m_TextureFiles.DeleteString(0);

	// ���� ã��.
	list<string> extList;
	extList.push_back("jpg");
	extList.push_back("png");
	extList.push_back("bmp");
	list<string> textureFiles;
	common::CollectFiles(extList, directoryPath, textureFiles);

	BOOST_FOREACH(auto &fileName, textureFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_TextureFiles.InsertString(m_TextureFiles.GetCount(), wstr.c_str());
	}
}


void CBrushPanel::OnSelchangeListTextureFiles()
{
	RET(m_TextureFiles.GetCurSel() < 0);

	CString fileName;
	m_TextureFiles.GetText(m_TextureFiles.GetCurSel(), fileName);
	SAFE_DELETE(m_texture);
	m_texture = Image::FromFile(fileName);
	
	const string strFileName = wstr2str((wstring)fileName);
	cMapController::Get()->GetTerrainCursor().SelectBrushTexture(strFileName);

	InvalidateRect(NULL, FALSE);
}


void CBrushPanel::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	Graphics *graph = Graphics::FromHDC(dc.GetSafeHdc());

	if (m_texture)
	{
		// �ؽ��� ���.
		if (CWnd *pictureCtlr = GetDlgItem(IDC_STATIC_TEXTURE))
		{
			CRect cr;
			pictureCtlr->GetWindowRect(cr);
			ScreenToClient(cr);

			const Rect dest(cr.left, cr.top, cr.Width(), cr.Height());
			graph->DrawImage(m_texture, dest );
		}
	}
}


void CBrushPanel::OnChangeMfceditbrowseTexture()
{
	CString wfilePath;
	m_textureBrowser.GetWindowText(wfilePath);
	string filePath = common::wstr2str((wstring)wfilePath);
	filePath += "\\";
	UpdateTextureFiles(filePath);

}
