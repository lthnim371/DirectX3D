#pragma once

enum FILETYPE
{
	DAT = 1, JPG,
};

// CModelView ���Դϴ�.

class CModelView : public CView
{
public:
	CModelView();           // ���� ����⿡ ���Ǵ� protected �������Դϴ�.
	virtual ~CModelView();

	void Init();
	void Render();
	void UpdateCamera();

	void FileLoad(short type, const string& fileName);

	CPoint m_mousePos;

	short m_sfileIndex;

protected:
	Matrix44 m_rotateTm;
	Vector3 m_camPos;
	Vector3 m_lookAtPos;

	bool m_LButtonDown;
	bool m_RButtonDown;
	CPoint m_curPos;


	graphic::cLight m_Light;
	graphic::cMaterial m_Mtrl;
	LPD3DXMESH m_Mesh;
	vector< graphic::cMaterial > m_Mtrls;
	vector< graphic::cTexture > m_Textures;

	graphic::cDX9Mesh m_mesh;


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
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	virtual void OnInitialUpdate();
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
};
