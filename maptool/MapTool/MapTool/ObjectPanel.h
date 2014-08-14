#pragma once
#include "afxwin.h"


// CObjectPanel ��ȭ �����Դϴ�.

class CObjectPanel : public CDialogEx
								, public common::iObserver
{
//	DECLARE_DYNAMIC(CObjectPanel)

public:
	CObjectPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CObjectPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_OBJECT };

public:
	virtual void Update() override;  //iObserver�����Լ� ������
	void UpdateObjectList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

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
