#pragma once
#include "afxcmn.h"


// cMainPanelTab ��ȭ �����Դϴ�.
class cTerrainPanel;
class cBrushPanel;
class cMainPanelTab : public CDialogEx
{

public:
	cMainPanelTab(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~cMainPanelTab();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MAINPANELTAB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
	CTabCtrl m_Tab;

protected:
	cTerrainPanel* m_terrain;
	cBrushPanel* m_brush;
};
