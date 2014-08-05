#pragma once
#include "afxcmn.h"


// CTabPanel ��ȭ �����Դϴ�.
class CHeightMapPanel;
class CTabPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CTabPanel)

public:
	CTabPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTabPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_TABPANEL };

protected:
	CHeightMapPanel *m_HeightMapPanel;

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CTabCtrl m_Tab;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
