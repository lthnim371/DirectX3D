//use debug
//	Vector3 test( m_look );
//	dbg::Print( "%f,%f,%f", test.x,test.y,test.z);

#include "stdafx.h"
#include "camera.h"

using namespace graphic;

cCamera::cCamera()
	: m_pos(0,250, 250), m_look(0,0,0), m_up(0,1,0)
{		
	SetView();  //set view
	
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
//	dbg::Print( "%f,%f", m_dir.x, m_dir.z);
//	m_dir.Normalize();  //dir의 크기가 필요한 곳이 있으므로 정규화하지 않음
	m_right = m_up.CrossProduct( m_dir.Normal() );
	m_right.Normalize();
}

void cCamera::SetPosition(const Matrix44& pos)
{
//	m_pos += ( Vector3(m_dir.x, 0.f, m_dir.z) * step );
//	m_look += ( Vector3(m_dir.x, 0.f, m_dir.z) * step );
	m_look = pos.GetPosition();// + Vector3(0,100.f,0);
	m_pos = m_look - m_dir;
	
	Update();
}

void cCamera::SetTranslation(const Vector3& pos)
//void cCamera::SetTranslation(const float z)
{
	m_look += pos;
//	m_look *= z;
//	m_look *= Vector3(m_dir.x, 0.f, m_dir.z);
	m_pos = m_look - m_dir;

//	dbg::Print( "%f,%f", pos.x,pos.z);

	Update();
}


void cCamera::SetRotation(const float x, const float y)
//void cCamera::SetRotation(const POINT& ptMouse)
{
	{  // rotate Y-Axis
		Quaternion q( m_up, -x * 0.005f ); 
		Matrix44 m = q.GetMatrix();
		Vector3 currDir(m_pos - m_look);
		currDir *= m;
		m_pos = m_look + currDir;
//		dbg::Print( "%f,%f", m_pos.x,m_pos.z);
	}

	//if( y != 0 )
	//{ // rotate X-Axis		
	//	if(m_pos.y >= 50.f && m_pos.y <= 300)
	//	{
	//		Quaternion q( m_right, y * 0.005f ); 
	//		Matrix44 m = q.GetMatrix();
	//		Vector3 currDir(m_pos - m_look);
	//		currDir *= m;
	//		m_pos = m_look + currDir;
	//	}
	//}

	Update();
}

void cCamera::SetView()  //set view
{
	Update();
	
	Matrix44 view;
	view.SetView( m_pos, m_dir.Normal(), m_up );
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&view);
}