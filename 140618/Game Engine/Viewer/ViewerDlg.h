
// ViewerDlg.h : ��� ����
//

#pragma once
#include "afxcmn.h"


// CViewerDlg ��ȭ ����
class CViewerDlg : public CDialogEx
{
// �����Դϴ�.
public:
	CViewerDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
		
	void MainProc();
	
// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_VIEWER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	bool m_bLoop;
	CModelView *m_pModelView;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
//	afx_msg void OnBnClickedButtonA();
//	CString m_strOK;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
//	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	CListCtrl m_fileList;
	afx_msg void OnItemchangedFileList(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedCancel();
	afx_msg void OnDropFiles(HDROP hDropInfo);
};
