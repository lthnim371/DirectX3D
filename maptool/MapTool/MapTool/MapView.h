#pragma once


// CMapView ���Դϴ�.

class cTerrainCursor;
class CMapView : public CView
{
public:
	CMapView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CMapView();

	bool Init();
	void Render();
	void Update(float elapseT);


protected:
	bool m_dxInit;
	string m_filePath;
	Matrix44 m_rotateTm;

	graphic::cGrid m_grid;
	graphic::cCube m_cube;
	graphic::cShader m_terrainShader;
	graphic::cShader m_terrainShader2;
	graphic::cShader *m_modelShader; // reference
	graphic::cLine m_line;
	common::Ray m_ray;

	CPoint m_curPos;
	bool m_LButtonDown;
	bool m_RButtonDown;
	bool m_MButtonDown;

//�߰�
	graphic::cShader m_objectShader;
	int m_objectCount;
	graphic::cModel* m_currSelectObj;

public:
	virtual void OnDraw(CDC* pDC);      // �� �並 �׸��� ���� �����ǵǾ����ϴ�.
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

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
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};


