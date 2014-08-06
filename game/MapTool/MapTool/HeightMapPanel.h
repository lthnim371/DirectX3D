#pragma once
#include "afxwin.h"


// CHeightMapPanel ��ȭ �����Դϴ�.

class CHeightMapPanel : public CDialogEx
{
	DECLARE_DYNAMIC(CHeightMapPanel)

public:
	CHeightMapPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CHeightMapPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_HEIGHTMAP };

	
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
};
