#pragma once


// CMainPanel �������Դϴ�.
class cMainPanelTab;
class cMainPanel : public CMiniFrameWnd
{
public:
	cMainPanel();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~cMainPanel();

	void Init();

protected:
	DECLARE_MESSAGE_MAP()

	cMainPanelTab* m_mainPanel;

};


