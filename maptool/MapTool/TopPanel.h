#pragma once


// CTopPanel �������Դϴ�.
class CMainPanel;
class CTopPanel : public CMiniFrameWnd
{
public:
	CTopPanel();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CTopPanel();

	void Init();

public:
	CMainPanel* m_mainPanel;

protected:
	DECLARE_MESSAGE_MAP()
};


