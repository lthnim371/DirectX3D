//과제 : 이미지 파일 읽어와서 


// ModelView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "Viewer.h"
#include "ModelView.h"


// CModelView

//IMPLEMENT_DYNCREATE(CModelView, CView)

CModelView::CModelView()
	: m_bmouseRButton(false)
{
}

CModelView::~CModelView()
{
}

BEGIN_MESSAGE_MAP(CModelView, CView)
	ON_WM_MOUSEMOVE()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
END_MESSAGE_MAP()


// CModelView 그리기입니다.

void CModelView::OnDraw(CDC* pDC)
{
//	CDocument* pDoc = GetDocument();
	// TODO: 여기에 그리기 코드를 추가합니다.
//	pDC->TextOutW(10,10, m_str);

//	Render();

}


// CModelView 진단입니다.

#ifdef _DEBUG
void CModelView::AssertValid() const
{
	CView::AssertValid();
}

#ifndef _WIN32_WCE
void CModelView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif
#endif //_DEBUG


// CModelView 메시지 처리기입니다.

void CModelView::OnInitialUpdate()
{
	CView::OnInitialUpdate();

	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.

//	graphic::cRenderer::Get()->CreateDirectX(m_hWnd, 500, 500);
}

void CModelView::Render()
{
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(150, 150, 150),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		graphic::GetDevice()->BeginScene();

		graphic::GetRenderer()->RenderAxis();
		graphic::GetRenderer()->RenderGrid();
		
		//랜더링 끝
		graphic::GetDevice()->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}
}

void CModelView::Init()
{
//	m_camPos.SetTranslate(Vector3(100,100,-500));
	test = Vector3(100, 200, -300);
	UpdateCamera();

	Matrix44 proj;
	proj.SetProjection(D3DX_PI * 0.5f, (float)800 / (float) 600, 1.f, 1000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;
}

void CModelView::UpdateCamera()
{
	Vector3 dir = Vector3(0,0,0) - m_camPos.GetPosition();//Vector3(100,250,-250);
	dir.Normalize();
	m_camPos.SetView(m_camPos.GetPosition(), dir, Vector3(0,1,0));
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_camPos);
}

void CModelView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_bmouseRButton = true;
	m_ptMousePrev = point;
	
	CView::OnRButtonDown(nFlags, point);
}

void CModelView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
//	m_str.Format(L"%d, %d", point.x, point.y);
//	CWnd::Invalidate(true);
	
	if(m_bmouseRButton)
	{
		CPoint ptMouseCurr = point - m_ptMousePrev;
		m_ptMousePrev = point;
		Matrix44 temp;

		if(ptMouseCurr.x != 0)
		{
			Matrix44 rotateX;
			float testX = -ptMouseCurr.x * 0.005f;
			rotateX.SetRotationX(testX);
			m_camPos *= rotateX;
		}
		else if(ptMouseCurr.y != 0)
		{
			Matrix44 rotateY;
			float testY = -ptMouseCurr.x * 0.005f;
			rotateY.SetRotationY(testY);		
			m_camPos *= rotateY;
		}

//		m_camPos *= temp;
//		test *= m_camPos;

		UpdateCamera();
	}

	CView::OnMouseMove(nFlags, point);
}


void CModelView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	m_bmouseRButton = false;

	CView::OnRButtonUp(nFlags, point);
}
