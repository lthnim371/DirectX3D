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
	m_showSkybox = false;
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
	const int WINSIZE_X = 1024;		//�ʱ� ������ ���� ũ��
	const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
	m_camera.SetCamera(Vector3(100,300,-500), Vector3(0,0,0), Vector3(0,1,0));
	m_camera.SetProjection( D3DX_PI / 4.f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 10000.0f) ;


	graphic::GetDevice()->SetRenderState(D3DRS_NORMALIZENORMALS, TRUE);

	graphic::GetDevice()->LightEnable (
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ

	
	//m_shader.Create( "../media/shader/hlsl_skinning_using_color.fx", "TShader" );
	m_shader.Create( "../media/shader/hlsl_skinning_using_texcoord.fx", "TShader" );
	m_skybox.Create( "../media/skybox" );

	cController::Get()->AddObserver(this);
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

		if (m_showSkybox)
			m_skybox.Render();
		graphic::GetRenderer()->RenderFPS();
		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();

		if (graphic::cCharacter *character = cController::Get()->GetCharacter())
		{
			character->SetTM(m_rotateTm);
		}


		m_shader.SetMatrix( "mVP", m_camera.GetViewProjectionMatrix() );
		m_shader.SetVector( "vLightDir", Vector3(0,-1,0) );
		m_shader.SetVector( "vEyePos", m_camera.GetEyePos());

		cController::Get()->RenderShader(m_shader);
		//cController::Get()->Render();

		//������ ��
		graphic::GetDevice()->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}
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
		const CPoint pos = point  - m_curPos;
		m_curPos = point;

		Quaternion q1(m_camera.GetRight(), -pos.y * 0.01f);
		Quaternion q2(m_camera.GetUpVector(), -pos.x * 0.01f);

		m_rotateTm *= (q2.GetMatrix() * q1.GetMatrix());
	}	
	else if (m_RButtonDown)
	{
		const CPoint pos = point  - m_curPos;
		m_curPos = point;

		m_camera.Yaw2( pos.x * 0.005f );
		m_camera.Pitch2( pos.y * 0.005f );
	}
	else if (m_MButtonDown)
	{
		const CPoint pos = point  - m_curPos;
		m_curPos = point;

		const float len = m_camera.GetDistance();
		m_camera.MoveRight( -pos.x * len * 0.001f );
		m_camera.MoveUp( pos.y * len * 0.001f );
	}

	CView::OnMouseMove(nFlags, point);
}


BOOL CModelView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	dbg::Print( "%d %d", nFlags, zDelta);

	const float len = m_camera.GetDistance();
	float zoomLen = (len > 100)? 50 : (len/4.f);
	if (nFlags & 0x4)
		zoomLen = zoomLen/10.f;

	m_camera.Zoom( (zDelta<0)? -zoomLen : zoomLen );

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


void CModelView::Update()
{
	m_rotateTm.SetIdentity();

	//// ���� �� �ε�.
	//graphic::cCharacter *character = cController::Get()->GetCharacter();
	//if (character)
	//	character->LoadWeapon( "../media/max script/valle_weapon4.dat");
}
