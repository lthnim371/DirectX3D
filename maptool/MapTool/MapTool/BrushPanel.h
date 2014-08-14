#pragma once
#include "afxwin.h"
#include "afxeditbrowsectrl.h"
#include "afxcmn.h"


// CBrushPanel ��ȭ �����Դϴ�.

class CBrushPanel : public CDialogEx
								, public common::iObserver2
{
public:
	CBrushPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CBrushPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_BRUSH };


public:
	virtual void Update(int type) override;


protected:
	void UpdateTextureFiles( const string &directoryPath );
	void UpdateLayerList();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	Image *m_texture;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_TextureFiles;
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListTextureFiles();
	afx_msg void OnPaint();
	CMFCEditBrowseCtrl m_textureBrowser;
	afx_msg void OnChangeMfceditbrowseTexture();
	CListCtrl m_layerList;
	CSliderCtrl m_innerRSlider;
	CSliderCtrl m_outerRSlider;
	float m_innerRadius;
	float m_outerRadius;
	afx_msg void OnNMCustomdrawSliderInnerRadius(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMCustomdrawSliderOuterRadius(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditInnerRadius();
	afx_msg void OnEnChangeEditOuterRadius2();
};
