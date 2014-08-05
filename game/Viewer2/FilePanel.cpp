// FilePanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "FilePanel.h"
#include "afxdialogex.h"


// CFilePanel ��ȭ �����Դϴ�.
CFilePanel::CFilePanel(CWnd* pParent /*=NULL*/)
	: CPanelBase(CFilePanel::IDD, pParent)
	, m_textModelFile(_T(""))
	, m_textAnimationFiles(_T(""))
{

}

CFilePanel::~CFilePanel()
{
}

void CFilePanel::DoDataExchange(CDataExchange* pDX)
{
	CPanelBase::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MODEL, m_ModelFileList);
	DDX_Control(pDX, IDC_LIST_ANIMATION, m_AnimationFileList);
	DDX_Text(pDX, IDC_STATIC_MODEL_FILES, m_textModelFile);
	DDX_Text(pDX, IDC_STATIC_ANIMATION_FILES, m_textAnimationFiles);
}


BEGIN_MESSAGE_MAP(CFilePanel, CPanelBase)
	ON_BN_CLICKED(IDOK, &CFilePanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CFilePanel::OnBnClickedCancel)
	ON_WM_SIZE()
	ON_LBN_DBLCLK(IDC_LIST_ANIMATION, &CFilePanel::OnDblclkListAnimation)
	ON_LBN_DBLCLK(IDC_LIST_MODEL, &CFilePanel::OnDblclkListModel)
END_MESSAGE_MAP()


BOOL CFilePanel::OnInitDialog()
{
	__super::OnInitDialog();

	// ../../media  �������� *.dat, *.ani ������ ã�Ƽ� �������Ѵ�.
	UpdateModelFiles();
	UpdateAnimationFiles();

	return TRUE; 
}


void CFilePanel::Update()
{
	// ���� �ϴ� �� ����.
}


// CFilePanel �޽��� ó�����Դϴ�.
void CFilePanel::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


void CFilePanel::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
}


// ../media  �������� *.dat ������ ã�Ƽ� �������Ѵ�.
void CFilePanel::UpdateModelFiles()
{
	// ����Ʈ �ڽ� �ʱ�ȭ.
	while (0 < m_ModelFileList.GetCount())
		m_ModelFileList.DeleteString(0);

	// ���� ã��.
	list<string> extList;
	extList.push_back("dat");
	list<string> modelFiles;
	common::CollectFiles(extList, "../media/", modelFiles);

	BOOST_FOREACH(auto &fileName, modelFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_ModelFileList.InsertString(m_ModelFileList.GetCount(), wstr.c_str());
	}

	m_textModelFile.Format( L"Model Files : %d", modelFiles.size());
	UpdateData(FALSE);
}


// ../media  �������� *.ani ������ ã�Ƽ� �������Ѵ�.
void CFilePanel::UpdateAnimationFiles()
{
	// ����Ʈ �ڽ� �ʱ�ȭ.
	while (0 < m_AnimationFileList.GetCount())
		m_AnimationFileList.DeleteString(0);

	// ���� ã��.
	list<string> extList;
	extList.push_back("ani");
	list<string> aniFiles;
	common::CollectFiles(extList, "../media/", aniFiles);

	BOOST_FOREACH(auto &fileName, aniFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_AnimationFileList.InsertString(m_AnimationFileList.GetCount(), wstr.c_str());
	}

	m_textAnimationFiles.Format( L"Animation Files : %d", aniFiles.size());
	UpdateData(FALSE);
}



void CFilePanel::OnSize(UINT nType, int cx, int cy)
{
	__super::OnSize(nType, cx, cy);

	MoveChildCtrlWindow(m_ModelFileList, cx, cy);
	MoveChildCtrlWindow(m_AnimationFileList, cx, cy);	
}


// �ִϸ��̼� ������ ����.
void CFilePanel::OnDblclkListAnimation()
{
	const int selIdx = m_AnimationFileList.GetCurSel();
	if (selIdx < 0)
		return;

	CString text;
	m_AnimationFileList.GetText(selIdx, text);
	const string fileName = wstr2str((wstring)text);
	cController::Get()->LoadFile(fileName);	
}


// �� ������ ����.
void CFilePanel::OnDblclkListModel()
{
	const int selIdx = m_ModelFileList.GetCurSel();
	if (selIdx < 0)
		return;

	CString text;
	m_ModelFileList.GetText(selIdx, text);
	const string fileName = wstr2str((wstring)text);

	ShowLoadingDialog();
	cController::Get()->LoadFile(fileName);
}
