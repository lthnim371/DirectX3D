#include "stdafx.h"
#include "camera.h"

using namespace graphic;

cCamera::cCamera()
	: m_pos(0,50,-100), m_look(0,50,100), m_up(0,1,0)
{
	Update();

	m_right = m_up.CrossProduct(m_dir);
	m_right.Normalize();
	
	//set projection
	const int WINSIZE_X = 1024;	//초기 윈도우 가로 크기
	const int WINSIZE_Y = 768;	//초기 윈도우 세로 크기
	
	Matrix44 proj;
	proj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;
}
cCamera::~cCamera()
{}


void cCamera::SetTranslation(const Vector3& position)
{
	m_pos += position;
	m_look += position;
}

void cCamera::SetRotation(const float x_axis, const float y_axis)
{
	m_dir = m_look - m_pos;
	m_dir.Normalize();
	m_right = m_up.CrossProduct(m_dir);
	m_right.Normalize();

	{ // rotate Y-Axis
		Quaternion q( m_up, x_axis * 0.005f ); 
		Matrix44 m = q.GetMatrix();
		m_pos *= m;		
		m_look *= m;		
	}

	{ // rotate X-Axis		
		Quaternion q( m_right, y_axis * 0.005f ); 
		Matrix44 m = q.GetMatrix();
		Vector3 currPos(m_pos);
		currPos *= m;
		if(currPos.y >= 20.f && currPos.y <= 100.f)  //상하 카메라 방향 고정
		{
			m_pos *= m;
			m_look *= m;
		}
	}
}

void cCamera::Update()
{
	m_dir = m_look - m_pos;
	m_dir.Normalize();

	Matrix44 view;
	view.SetView( m_pos, m_dir, m_up );
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&view);
}