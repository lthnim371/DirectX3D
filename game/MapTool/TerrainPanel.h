#pragma once


// cTerrainPanel 대화 상자입니다.

class cTerrainPanel : public CDialogEx
{

public:
	cTerrainPanel(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~cTerrainPanel();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_TERRAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
};
