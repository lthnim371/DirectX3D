#pragma once


// CTopPanel �������Դϴ�.
class CTabPanel;
class CTopPanel : public CMiniFrameWnd
{
public:
	CTopPanel();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CTopPanel();

	void Init();


protected:
	CTabPanel *m_tabPanel;


protected:
	DECLARE_MESSAGE_MAP()
};


