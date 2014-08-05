#pragma once
#include "PanelBase.h"
#include "afxwin.h"


namespace graphic  {
	class cBoneNode;
}


class CFilePanel : public CPanelBase
	, public common::iObserver
{
public:
	CFilePanel(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CFilePanel();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_FILE };

	virtual void Update() override;
	
	void UpdateModelFiles();
	void UpdateAnimationFiles();


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CListBox m_ModelFileList;
	CListBox m_AnimationFileList;
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDblclkListAnimation();
	afx_msg void OnDblclkListModel();
	CString m_textModelFile;
	CString m_textAnimationFiles;
};
