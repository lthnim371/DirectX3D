// MapView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "MapTool.h"
#include "MapView.h"

#include "etc\Utility.h"
#include "MainPanel.h"

using namespace graphic;
// CMapView

CMapView* g_mapView = NULL;

vector<graphic::cCube*> g_cubes;

const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��

CMapView::CMapView()
	: m_dxInit(false)
	, m_LButtonDown(false)
	, m_RButtonDown(false)
	, m_MButtonDown(false)
{
	g_mapView = this;
}

CMapView::~CMapView()
{
}

BEGIN_MESSAGE_MAP(CMapView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()


// CMapView �׸����Դϴ�.

void CMapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}

// CMapView �����Դϴ�.

#ifdef _DEBUG
void CMapView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CMapView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


//���̷�Ʈ ����̽��� �������� �ξ���
LPDIRECT3DDEVICE9 g_pDevice;
LPDIRECT3DDEVICE9 graphic::GetDevice()
{
	return g_pDevice;
}

// CMapView �޽��� ó�����Դϴ�.
bool CMapView::Init()
{	
	if (!graphic::InitDirectX(m_hWnd, WINSIZE_X, WINSIZE_Y, g_pDevice))
	{
		return 0;
	}

	m_camPos = Vector3(100,100,-500);
	m_lookAtPos = Vector3(0,0,0);
	UpdateCamera();

//	Matrix44 proj;
	m_proj.SetProjection(D3DX_PI / 4.f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 10000.0f) ;
	GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&m_proj) ;

	GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	GetDevice()->LightEnable (
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ

	m_grid.Create(50, 50, 50);
	m_cube.SetCube(Vector3(-10,-10,-10), Vector3(10,10,10));
	m_cube.SetColor( 0xFFFF00FF );

	::GetCursorPos(&m_curPos);
	::ScreenToClient(m_hWnd, &m_curPos);
	
	m_dxInit = true;

	return true;
}

void CMapView::Update(float elapseT)
{

}

void CMapView::Render()
{
	if(!m_dxInit)
		return;

	//ȭ�� û��
	if (SUCCEEDED(GetDevice()->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(150, 150, 150),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
		GetDevice()->BeginScene();

//		graphic::GetRenderer()->RenderFPS();
//		RenderGrid();
	//���� �߿�
		Matrix44 mat;
		GetDevice()->SetTransform( D3DTS_WORLD, (D3DXMATRIX*)&mat );
		m_grid.Render();
		RenderAxis();
		mat.SetTranslate( m_mouseRay );
		m_cube.SetTransform( mat );
		m_cube.Render( Matrix44() );
		
		for(auto it = g_cubes.begin(); it < g_cubes.end(); ++it)
		{
			(*it)->Render( Matrix44() );
		}

		GetDevice()->SetRenderState(D3DRS_LIGHTING, false);  //�븻���Ͱ� �����Ƿ� ����Ʈ ����� ����

		//������ ��
		GetDevice()->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		GetDevice()->Present( NULL, NULL, NULL, NULL );
	}
}

void CMapView::UpdateCamera()
{
//	Matrix44 V;
	Vector3 dir = m_lookAtPos - m_camPos;
	dir.Normalize();
	m_view.SetView(m_camPos, dir, Vector3(0,1,0));
	GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_view);
}

void CMapView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	Vector3 pickPos;
	m_ray.Create(point.x, point.y, WINSIZE_X, WINSIZE_Y, m_proj, m_view);
	if( m_grid.Pick(m_ray.orig, m_ray.dir, pickPos) )
	{
		graphic::cCube* newCube = new graphic::cCube();
		newCube->SetCube(Vector3(-10,-10,-10), Vector3(10,10,10));
		newCube->SetColor(0xFFFF00FF);
		//Matrix44 mat;
		//mat.SetTranslate(pickPos);
		newCube->SetTransform(m_cube.GetTransform());
		g_cubes.push_back(newCube);
		g_mainPanel->InsertCube( *newCube );
	}


	CView::OnLButtonDown(nFlags, point);
}


void CMapView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.

	CView::OnLButtonUp(nFlags, point);
}


void CMapView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetFocus();  //���� ������â���� �̺�Ʈ�� �߻���Ŵ
	SetCapture();  //������â ������ ���콺 ��ġ�� ���� ����. ��, ������ �� ���� ���� ReleaseCapture�� �� ����� �Ѵ�.
	m_RButtonDown = true;
	m_curPos = point;

	CView::OnRButtonDown(nFlags, point);
}


void CMapView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	ReleaseCapture();  //SetCapture�� ����Ͽ����� �� ������������� �Ѵ�.
	m_RButtonDown = false;

	CView::OnRButtonUp(nFlags, point);
}

void CMapView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	SetFocus();
	SetCapture();
	m_MButtonDown = true;
	m_curPos = point;

	CView::OnMButtonDown(nFlags, point);
}


void CMapView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	ReleaseCapture();
	m_MButtonDown = false;
	CView::OnMButtonUp(nFlags, point);

	CView::OnMButtonUp(nFlags, point);
}

void CMapView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (m_RButtonDown)
	{
		CPoint pos = point  - m_curPos;
		m_curPos = point;

		{ // rotate Y-Axis
			Quaternion q(Vector3(0,1,0), pos.x * 0.005f); 
			Matrix44 m = q.GetMatrix();
			m_camPos *= m;
		}

		{ // rotate X-Axis
			Quaternion q(Vector3(1,0,0), pos.y * 0.005f); 
			Matrix44 m = q.GetMatrix();
			m_camPos *= m;
		}

		UpdateCamera();
	}
	else if (m_MButtonDown)
	{
		CPoint pos = point  - m_curPos;
		m_curPos = point;

		Vector3 v = m_lookAtPos - m_camPos;
		const float len = v.Length();
		v.Normalize();

		Vector3 up = Vector3(0,1,0);
		Vector3 right = up.CrossProduct(v);
		right.Normalize();

		m_lookAtPos += right * pos.x * (len * -0.001f);
		m_camPos += right * pos.x * (len * -0.001f);
		m_lookAtPos += up * pos.y * (len * 0.001f);
		m_camPos += up * pos.y * (len * 0.001f);

		UpdateCamera();
	}
	else
	{
		m_ray.Create(point.x, point.y, WINSIZE_X, WINSIZE_Y, m_proj, m_view);
		m_grid.Pick(m_ray.orig, m_ray.dir, m_mouseRay);
	}

	CView::OnMouseMove(nFlags, point);
}


void CMapView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	if (nChar == VK_TAB)
	{
		static bool flag = false;
		GetDevice()->SetRenderState(D3DRS_CULLMODE, flag);
		GetDevice()->SetRenderState(D3DRS_FILLMODE, flag? D3DFILL_SOLID : D3DFILL_WIREFRAME);
		flag = !flag;
	}

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMapView::SelectCube(int index)
{
	g_cubes[index]->SetColor(0xFFFF0000);
}