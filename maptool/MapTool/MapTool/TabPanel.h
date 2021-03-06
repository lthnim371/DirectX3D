#pragma once
#include "afxcmn.h"


// CTabPanel 대화 상자입니다.
class CHeightMapPanel;
class CTerrainPanel;
class CBrushPanel;
class CModelPanel;
//추가
class CObjectPanel;
class CTabPanel : public CDialogEx
{
public:
	CTabPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CTabPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TABPANEL };


protected:
	CHeightMapPanel *m_heightMapPanel;
	CTerrainPanel *m_terrainPanel;
	CBrushPanel *m_brushPanel;
	CModelPanel *m_modelPanel;
//추가
	CObjectPanel* m_objectPanel;
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CTabCtrl m_Tab;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeTab1(NMHDR *pNMHDR, LRESULT *pResult);
};
