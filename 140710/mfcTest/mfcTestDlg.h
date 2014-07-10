
// mfcTestDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CmfcTestDlg 대화 상자
class CmfcTestDlg : public CDialogEx
{
// 생성입니다.
public:
	CmfcTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
	enum { IDD = IDD_MFCTEST_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
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
