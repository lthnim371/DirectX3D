#pragma once


// CMapView ���Դϴ�.

class CMapView : public CView
{

public:
	CMapView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMapView();

	bool Init();
	void Update(float elapseT);
	void Render();
	void UpdateCamera();

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

private:
	Matrix44 m_rotateTm;
	Vector3 m_camPos;
	Vector3 m_lookAtPos;
	Matrix44 m_proj;
	Matrix44 m_view;
	Vector3 m_mouseRay;

	bool m_dxInit;
	bool m_LButtonDown;
	bool m_RButtonDown;
	bool m_MButtonDown;
	CPoint m_curPos;

	graphic::cGrid2 m_grid;
	graphic::cCube m_cube;
	Ray m_ray;

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnMButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


