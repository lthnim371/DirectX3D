
#pragma once


class CModelView;
class CAnimationController2;
class CViewer2Dlg : public CDialogEx
							, public common::iObserver
{
// �����Դϴ�.
public:
	CViewer2Dlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
	virtual ~CViewer2Dlg();
	enum { IDD = IDD_VIEWER2_DIALOG };

	void MainLoop();
	virtual void Update() override; // Observer Update


protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;
	CModelView *m_modelView;
	CAnimationController2 *m_aniController;
	bool m_loop;
	bool m_dxInit;

	// ������ �޽��� �� �Լ�
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedCheckWireframe();
	BOOL m_WireFrame;
	afx_msg void OnDropFiles(HDROP hDropInfo);
	afx_msg void OnBnClickedCheckBone();
	BOOL m_RenderBone;
	BOOL m_RenderMesh;
	afx_msg void OnBnClickedCheckMesh();
	afx_msg void OnBnClickedCheckBoundingbox();
	BOOL m_RenderBoundingBox;
	afx_msg void OnBnClickedCheckSkybox();
	BOOL m_ShowSkyBox;
};
