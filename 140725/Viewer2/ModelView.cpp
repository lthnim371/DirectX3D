// ModelView.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "Viewer2.h"
#include "ModelView.h"



// CModelView

CModelView::CModelView()
{
	m_LButtonDown = false;
	m_RButtonDown = false;
	m_MButtonDown = false;
	
}

CModelView::~CModelView()
{

}

BEGIN_MESSAGE_MAP(CModelView, CView)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_MBUTTONDOWN()
	ON_WM_MBUTTONUP()
END_MESSAGE_MAP()


// CModelView �׸����Դϴ�.

void CModelView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	// TODO: ���⿡ �׸��� �ڵ带 �߰��մϴ�.
}


// CModelView �����Դϴ�.

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


// CModelView �޽��� ó�����Դϴ�.

void CModelView::Init()
{
	m_filePath = "../media/data.dat";

	m_camPos = Vector3(100,100,-500);
	m_lookAtPos = Vector3(0,0,0);
	UpdateCamera();

	const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
	const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
	m_matProj.SetProjection(D3DX_PI / 4.f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 10000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&m_matProj) ;

	graphic::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	graphic::GetDevice()->LightEnable (
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ

	

	//m_shader.Create( "../media/shader/hlsl_viewer2.fx", "TShader" );
	m_shader.Create( "../media/shader/hlsl_skinning.fx", "TShader" );
}


void CModelView::Update(const float elapseT)
{
	graphic::GetRenderer()->Update(elapseT);
	cController::Get()->Update(elapseT);
}


void CModelView::Render()
{
	//ȭ�� û��
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(150, 150, 150),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
		graphic::GetDevice()->BeginScene();

		graphic::GetRenderer()->RenderFPS();
		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();

		if (graphic::cCharacter *character = cController::Get()->GetCharacter())
		{
			character->SetTM(m_rotateTm);
		}

		m_shader.SetMatrix( "mVP", m_matView * m_matProj);
		m_shader.SetVector( "vLightDir", Vector3(0,-1,0) );
		m_shader.SetMatrix( "mWorld", m_rotateTm );

		cController::Get()->RenderShader(m_shader);
		//cController::Get()->Render();

		//������ ��
		graphic::GetDevice()->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}
}


void CModelView::UpdateCamera()
{
	Vector3 dir = m_lookAtPos - m_camPos;
	dir.Normalize();
	m_matView.SetView(m_camPos, dir, Vector3(0,1,0));
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&m_matView);
}


void CModelView::OnLButtonDown(UINT nFlags, CPoint point)
{
	SetCapture();
	SetFocus();
	m_LButtonDown = true;
	m_curPos = point;
	CView::OnLButtonDown(nFlags, point);
}


void CModelView::OnLButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_LButtonDown = false;
	CView::OnLButtonUp(nFlags, point);
}


void CModelView::OnMouseMove(UINT nFlags, CPoint point)
{
	if (m_LButtonDown)
	{
		CPoint pos = point  - m_curPos;
		m_curPos = point;

		Matrix44 mat1;
		mat1.SetRotationY( -pos.x * 0.01f );
		Matrix44 mat2;
		mat2.SetRotationX( -pos.y * 0.01f );

		m_rotateTm *= (mat1 * mat2);
	}	
	else if (m_RButtonDown)
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

	CView::OnMouseMove(nFlags, point);
}


BOOL CModelView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	dbg::Print( "%d %d", nFlags, zDelta);

	Vector3 dir = m_lookAtPos - m_camPos;
	const float len = dir.Length();
	dir.Normalize();

	float zoomLen = (len > 100)? 50 : (len/4.f);
	if (nFlags & 0x4)
		zoomLen = zoomLen/10.f;

	m_camPos += (zDelta<0)? dir*-zoomLen : dir*zoomLen;

	UpdateCamera();

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CModelView::OnRButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_RButtonDown = true;
	m_curPos = point;
	CView::OnRButtonDown(nFlags, point);
}


void CModelView::OnRButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_RButtonDown = false;
	CView::OnRButtonUp(nFlags, point);
}


void CModelView::OnMButtonDown(UINT nFlags, CPoint point)
{
	SetFocus();
	SetCapture();
	m_MButtonDown = true;
	m_curPos = point;
	CView::OnMButtonDown(nFlags, point);
}


void CModelView::OnMButtonUp(UINT nFlags, CPoint point)
{
	ReleaseCapture();
	m_MButtonDown = false;
	CView::OnMButtonUp(nFlags, point);
}


bool CModelView::LoadFile(const string &fileName)
{
	m_rotateTm.SetIdentity();
	m_filePath = fileName;

	cController::Get()->LoadFile(fileName);
	graphic::cCharacter *character = cController::Get()->GetCharacter();
	character->LoadWeapon( "../media/weapon.dat");

	return false;
}

