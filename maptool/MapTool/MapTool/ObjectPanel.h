#pragma once
#include "afxwin.h"


// CObjectPanel 대화 상자입니다.

class CObjectPanel : public CDialogEx
								, public common::iObserver
{
//	DECLARE_DYNAMIC(CObjectPanel)

public:
	CObjectPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjectPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_OBJECT };

public:
	virtual void Update() override;  //iObserver가상함수 재정의
	void UpdateObjectList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	CListBox m_objectList;
	afx_msg void OnLbnSelchangeListObject();
	afx_msg void OnLbnSelchangeListObjectSetup();
	CListBox m_objectSetUpList;
};
