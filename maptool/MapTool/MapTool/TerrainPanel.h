#pragma once
#include "afxeditbrowsectrl.h"


// CTerrainPanel ��ȭ �����Դϴ�.

class CTerrainPanel : public CDialogEx
								, public common::iObserver2
{
public:
	CTerrainPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CTerrainPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_TERRAIN };


public:
	virtual void Update(int type) override;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButtonLoadTerrain();
	afx_msg void OnBnClickedButtonCrterrain();
	afx_msg void OnBnClickedButtonSaveTerrain();
	int m_colCellCount;
	int m_rowCellCount;
	float m_cellSize;
	CMFCEditBrowseCtrl m_textureBrowser;
	virtual BOOL OnInitDialog();
	afx_msg void OnChangeMfceditbrowseTexture();
};
