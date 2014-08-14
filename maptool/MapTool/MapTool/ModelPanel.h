#pragma once
#include "afxeditbrowsectrl.h"
#include "afxwin.h"
#include "afxcmn.h"


// CModelPanel ��ȭ �����Դϴ�.

class CModelPanel : public CDialogEx
								, public iObserver2
{
public:
	CModelPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CModelPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MODEL };


public:
	virtual void Update(int type) override;
	void UpdateModelList(const string &directoryPath);
	void UpdatePlaceModelList();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CMFCEditBrowseCtrl m_modelBrowser;
	CListBox m_modelList;
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeMfceditbrowseModel();
	afx_msg void OnSelchangeListModel();
	CListCtrl m_placeModelList;
};
