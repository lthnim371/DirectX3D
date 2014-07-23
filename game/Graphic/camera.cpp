#include "stdafx.h"
#include "camera.h"

using namespace graphic;

cCamera::cCamera()
	: m_pos(0,250,-150), m_look(0,0,1000), m_up(0,1,0)
{
	SetView();
	
	//set projection
	const int WINSIZE_X = 1024;	//초기 윈도우 가로 크기
	const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
	
	Matrix44 proj;
	proj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;
}
cCamera::~cCamera()
{}

void cCamera::Update()
{
	m_dir = m_look - m_pos;
	m_dir.Normalize();
	m_right = m_up.CrossProduct(m_dir);
	m_right.Normalize();
}

bool cCamera::Move()
{
	if( (::GetAsyncKeyState('W') & 0x8000) == 0x8000 )
	{		
		SetTranslation(5.f);
		return true;
	}
	else if( (::GetAsyncKeyState('S') & 0x8000) == 0x8000 )
	{		
		SetTranslation(-5.f);		
		return true;
	}

	return false;
}

void cCamera::SetTranslation(const float step)
{
	Update();

	m_pos += ( Vector3(m_dir.x, 0.f, m_dir.z) * step );
	m_look += ( Vector3(m_dir.x, 0.f, m_dir.z) * step );

	SetView();
}

void cCamera::SetRotation(const float x, const float y)
{
	Update();

	{ // rotate Y-Axis
		Quaternion q( m_up, x * 0.005f ); 
		Matrix44 m = q.GetMatrix();
		m_look = (m_dir * m) + m_pos;
//		m_pos *= m;		
		//m_look *= m;		
	}

	{ // rotate X-Axis		
		Quaternion q( m_right, y * 0.005f ); 
		Matrix44 m = q.GetMatrix();
		Vector3 currPos(m_pos);
		currPos *= m;
		if(currPos.y >= 100.f && currPos.y <= 250.f)  //상하 카메라 방향 고정
		{
			m_pos *= m;
			m_look *= m;
		}
//		dbg::Print( "%f", m_look.y );
	}

	SetView();
}

void cCamera::SetView()
{
	Update();

	Matrix44 view;
	view.SetView( m_pos, m_dir, m_up );
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&view);
}