#pragma once
#include "afxwin.h"


// CObjectPanel 대화 상자입니다.

class CObjectPanel : public CDialogEx
								, public common::iObserver2
{
//	DECLARE_DYNAMIC(CObjectPanel)

public:
	CObjectPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CObjectPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_OBJECT };

public:
	virtual void Update(int type) override;  //iObserver가상함수 재정의
	void UpdateObjectList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_objectList;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListObjectLoad();
	afx_msg void OnBnClickedButtonSave();
};
