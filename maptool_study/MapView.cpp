// MapView.cpp : 구현 파일입니다.
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

const int WINSIZE_X = 1024;		//초기 윈도우 가로 크기
const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기

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


// CMapView 그리기입니다.

void CMapView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
}

// CMapView 진단입니다.

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


//다이렉트 디바이스를 전역으로 두었음
LPDIRECT3DDEVICE9 g_pDevice;
LPDIRECT3DDEVICE9 graphic::GetDevice()
{
	return g_pDevice;
}

// CMapView 메시지 처리기입니다.
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
		0, // 활성화/ 비활성화 하려는 광원 리스트 내의 요소
		true); // true = 활성화 ， false = 비활성화

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

	//화면 청소
	if (SUCCEEDED(GetDevice()->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(150, 150, 150),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		GetDevice()->BeginScene();

//		graphic::GetRenderer()->RenderFPS();
//		RenderGrid();
	//순서 중요
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

		GetDevice()->SetRenderState(D3DRS_LIGHTING, false);  //노말벡터가 없으므로 라이트 기능을 끈다

		//랜더링 끝
		GetDevice()->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CView::OnLButtonUp(nFlags, point);
}


void CMapView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetFocus();  //현재 윈도우창에서 이벤트를 발생시킴
	SetCapture();  //윈도우창 밖으로 마우스 위치가 적용 가능. 단, 볼일을 다 보고 나면 ReleaseCapture를 꼭 해줘야 한다.
	m_RButtonDown = true;
	m_curPos = point;

	CView::OnRButtonDown(nFlags, point);
}


void CMapView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ReleaseCapture();  //SetCapture를 사용하였으면 꼭 릴레이즈해줘야 한다.
	m_RButtonDown = false;

	CView::OnRButtonUp(nFlags, point);
}

void CMapView::OnMButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	SetFocus();
	SetCapture();
	m_MButtonDown = true;
	m_curPos = point;

	CView::OnMButtonDown(nFlags, point);
}


void CMapView::OnMButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	ReleaseCapture();
	m_MButtonDown = false;
	CView::OnMButtonUp(nFlags, point);

	CView::OnMButtonUp(nFlags, point);
}

void CMapView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
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