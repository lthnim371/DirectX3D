#pragma once


// cBrushPanel ��ȭ �����Դϴ�.

class cBrushPanel : public CDialogEx
{
	
public:
	cBrushPanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~cBrushPanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_BRUSH };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};