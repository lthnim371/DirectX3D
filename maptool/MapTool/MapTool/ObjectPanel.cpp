// ObjectPanel.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MapTool.h"
#include "ObjectPanel.h"
#include "afxdialogex.h"


// CObjectPanel 대화 상자입니다.

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
END_MESSAGE_MAP()


// CObjectPanel 메시지 처리기입니다.


void CObjectPanel::OnBnClickedOk()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnOK();
}


void CObjectPanel::OnBnClickedCancel()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
//	CDialogEx::OnCancel();
}


BOOL CObjectPanel::OnInitDialog()
{
	__super::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	UpdateObjectList();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


void CObjectPanel::OnLbnSelchangeListObjectLoad()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
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
	// 리스트 박스 초기화.
	while (0 < m_objectList.GetCount())
		m_objectList.DeleteString(0);

	// 파일 찾기.
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
