#include "stdafx.h"
#include "camera.h"

using namespace graphic;

cCamera::cCamera()
	: m_pos(0,250,-250), m_look(0,0,0), m_up(0,1,0), m_test(0,0,0)
{	
	m_lookPos = m_look - m_pos;

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

/*
int cCamera::Move(const Matrix44& character)
{
	if( (::GetAsyncKeyState('W') & 0x8000) == 0x8000 )
	{		
		SetTranslation(5.f);
		m_look = character.GetPosition();
		return 1;
	}
	else if( (::GetAsyncKeyState('S') & 0x8000) == 0x8000 )
	{		
		SetTranslation(-5.f);		
		m_look = character.GetPosition();
		return 2;
	}

	return 0;
}
*/

/*
void cCamera::SetTranslation(const float step)
{
	Update();

	m_pos += ( Vector3(m_dir.x, 0.f, m_dir.z) * step );
	//m_look += ( Vector3(m_dir.x, 0.f, m_dir.z) * step );
//	m_pos += ( m_dir * step );
//	m_look += ( m_dir * step );

//	SetView();
}
*/

void cCamera::SetRotation(const Matrix44& character, const float x, const float y)
{
	Update();

	if( x != 0 )
	{ // rotate Y-Axis
		Quaternion q( m_up, -x * 0.005f ); 
		Matrix44 m = q.GetMatrix();
		Vector3 temp1(m_pos - m_look);
		temp1 *= m;
		m_pos = m_look + temp1;
	}

	if( y != 0 )
	{ // rotate X-Axis		
		Quaternion q( m_right, y * 0.005f ); 
		Matrix44 m = q.GetMatrix();
		m_pos *= m;

//		Vector3 currPos(m_pos);
//		currPos *= m_rot[1];
//		if(currPos.y >= 100.f && currPos.y <= 250.f)  //상하 카메라 방향 고정
//		{
//			m_pos *= m_rot[1];
////			m_look *= m;
//		}
	}
//	m_test = (character.GetPosition() - m_look);
//	m_pos += m_test;
}

void cCamera::SetView()
{
	Update();
		
	Matrix44 view;
	view.SetView( m_pos, m_dir, m_up );
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&view);
}

void cCamera::SetPosition(const Vector3& characterPos)
{
	m_look = characterPos;
	m_pos = m_look - m_lookPos;
}