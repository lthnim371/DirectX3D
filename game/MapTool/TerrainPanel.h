#pragma once


// cTerrainPanel ��ȭ �����Դϴ�.

class cTerrainPanel : public CDialogEx
{

public:
	cTerrainPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~cTerrainPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_TERRAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
