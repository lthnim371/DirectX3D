#pragma once
#include "afxwin.h"
#include "afxcmn.h"


// CHeightMapPanel ��ȭ �����Դϴ�.

class CHeightMapPanel : public CDialogEx
									, public common::iObserver2
{
public:
	CHeightMapPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHeightMapPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_HEIGHTMAP };


public:
	virtual void Update(int type) override;
	void UpdateTerrainInfo();
	void UpdateHeightMapList();
	void UpdateTextureList();


private:
	Image *m_heightMap;
	Image *m_texture;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnDblclkListHeightmap();
	afx_msg void OnSelchangeListHeightmap();
	afx_msg void OnPaint();
	CListBox m_HeightMapList;
	CListBox m_TextureList;
	afx_msg void OnSelchangeListHeightmapTex();
	afx_msg void OnDblclkListHeightmapTex();
	CSliderCtrl m_heightSlider;
	float m_heightFactor;
	afx_msg void OnNMCustomdrawSliderHeightFactor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditHeightFactor();
	CSliderCtrl m_uvSlider;
	float m_uvFactor;
	afx_msg void OnNMCustomdrawSliderUvFactor(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEditUvFactor();
};
