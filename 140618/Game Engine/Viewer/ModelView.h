#pragma once


// CModelView ���Դϴ�.

class CModelView : public CView
{
//	DECLARE_DYNCREATE(CModelView)

//protected:
public:
	CModelView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CModelView();

	void Init();
	void UpdateCamera();
	void Render();

private:  //�������
	Vector3 test;
	Matrix44 m_camPos;
	bool m_bmouseRButton;
	POINT m_ptMousePrev;
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
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
};


