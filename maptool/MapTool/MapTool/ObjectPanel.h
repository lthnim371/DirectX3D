#pragma once
#include "afxwin.h"


// CObjectPanel ��ȭ �����Դϴ�.

class CObjectPanel : public CDialogEx
								, public common::iObserver2
{
//	DECLARE_DYNAMIC(CObjectPanel)

public:
	CObjectPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CObjectPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_OBJECT };

public:
	virtual void Update(int type) override;  //iObserver�����Լ� ������
	void UpdateObjectList();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_objectList;
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListObjectLoad();
	afx_msg void OnBnClickedButtonSave();
};
