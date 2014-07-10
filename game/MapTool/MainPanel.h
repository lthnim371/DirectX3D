#pragma once


// CMainPanel 프레임입니다.
class cMainPanelTab;
class cMainPanel : public CMiniFrameWnd
{
public:
	cMainPanel();           // 동적 만들기에 사용되는 protected 생성자입니다.
	virtual ~cMainPanel();

	void Init();

protected:
	DECLARE_MESSAGE_MAP()

	cMainPanelTab* m_mainPanel;

};


