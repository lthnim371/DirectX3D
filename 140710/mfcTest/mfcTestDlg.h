
// mfcTestDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CmfcTestDlg ��ȭ ����
class CmfcTestDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CmfcTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_MFCTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedCheck1();
	BOOL m_check1;
	afx_msg void OnBnClickedButton1();
	CString m_str;
	afx_msg void OnStnClickedStaticName();
	CString m_strEdit;
	afx_msg void OnEnChangeEdit1();
	CComboBox m_combo;
	afx_msg void OnSelchangeCombo1();
	CListBox m_list;
	afx_msg void OnSelchangeList1();
};
