#pragma once


// CModelView ���Դϴ�.

class CModelView : public CView
{
//	DECLARE_DYNCREATE(CModelView)

//protected:
public:
	CModelView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CModelView();

	void Render();

private:  //�������
	POINT m_mousePt;
	CString m_str;

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.  //hdc �߻�ȭ������
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
};


