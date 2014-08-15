// ObjectPanel.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "ObjectPanel.h"
#include "afxdialogex.h"

#include <fstream>

// CObjectPanel ��ȭ �����Դϴ�.

//IMPLEMENT_DYNAMIC(CObjectPanel, CDialogEx)

CObjectPanel::CObjectPanel(CWnd* pParent /*=NULL*/)
	: CDialogEx(CObjectPanel::IDD, pParent)
{

}

CObjectPanel::~CObjectPanel()
{
}

void CObjectPanel::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_OBJECT_LOAD, m_objectList);
}


BEGIN_MESSAGE_MAP(CObjectPanel, CDialogEx)
	ON_BN_CLICKED(IDOK, &CObjectPanel::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CObjectPanel::OnBnClickedCancel)
	ON_LBN_SELCHANGE(IDC_LIST_OBJECT_LOAD, &CObjectPanel::OnLbnSelchangeListObjectLoad)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CObjectPanel::OnBnClickedButtonSave)
END_MESSAGE_MAP()


// CObjectPanel �޽��� ó�����Դϴ�.


void CObjectPanel::OnBnClickedOk()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CDialogEx::OnOK();
}


void CObjectPanel::OnBnClickedCancel()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
//	CDialogEx::OnCancel();
}


BOOL CObjectPanel::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.
	UpdateObjectList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// ����: OCX �Ӽ� �������� FALSE�� ��ȯ�ؾ� �մϴ�.
}


void CObjectPanel::OnLbnSelchangeListObjectLoad()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	RET(m_objectList.GetCurSel() < 0);

	CString fileName;
	m_objectList.GetText(m_objectList.GetCurSel(), fileName);
	const string strFileName = wstr2str((wstring)fileName);

	cMapController::Get()->SetSelectObject( strFileName );
}

void CObjectPanel::Update(int type)
{
	switch (type)
	{
	case NOTIFY_TYPE::NOTIFY_ADD_PLACE_MODEL:
		UpdateObjectList();
		break;
	}

}

void CObjectPanel::UpdateObjectList()
{
	// ����Ʈ �ڽ� �ʱ�ȭ.
	while (0 < m_objectList.GetCount())
		m_objectList.DeleteString(0);

	// ���� ã��.
	list<string> extList;
	extList.push_back("dat");
	list<string> objectFiles;
	common::CollectFiles(extList, "../../media/mesh/map/", objectFiles);

	BOOST_FOREACH(auto &fileName, objectFiles)
	{
		const wstring wstr = str2wstr(fileName);
		m_objectList.InsertString(m_objectList.GetCount(), wstr.c_str());
	}
}


void CObjectPanel::OnBnClickedButtonSave()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	using namespace std;

	ofstream fs;
	fs.open("mapobject.map", ios_base::binary);

	vector<graphic::cModel*> rObject = cMapController::Get()->GetObject();

	int objectCount = (int)rObject.size();
	fs.write( (char*)&objectCount, sizeof(int) );

	for(int i=0; i<(int)rObject.size(); ++i)
	{
		int nSize = rObject[i]->GetFileName().length();
		char* cFileName = new char[nSize + 1];
		::strcpy_s( cFileName, nSize + 1, rObject[i]->GetFileName().c_str() );
		fs.write((char*)&nSize, sizeof(int));
	//	fs.write((char*)&rObject[i]->GetFileName(), nSize);
		fs.write(cFileName, nSize + 1);
		fs.write((char*)&rObject[i]->GetTM(), sizeof(Matrix44));

		delete[] cFileName;
	}

	::AfxMessageBox(L"Save Complete");
}
