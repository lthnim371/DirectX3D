#include "stdafx.h"
#include "camera.h"

using namespace graphic;

cCamera::cCamera()
	: m_pos(0,250,-250), m_look(0,0,0), m_up(0,1,0)
{	
//	m_lookPos = m_look - m_pos;

	SetView();
	
	//set projection
	const int WINSIZE_X = 1024;	//�ʱ� ������ ���� ũ��
	const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
	
	Matrix44 proj;
	proj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;
}
cCamera::~cCamera()
{}

void cCamera::Update()
{
	m_dir = m_look - m_pos;
//	m_lookPos = m_dir;
//	m_dir.Normalize();
//	m_right = m_up.CrossProduct( m_dir );
	m_right = m_up.CrossProduct( m_dir.Normal() );
	m_right.Normalize();
}

void cCamera::SetTranslation(const Matrix44& character)
{

//	m_pos += ( Vector3(m_dir.x, 0.f, m_dir.z) * step );
//	m_look += ( Vector3(m_dir.x, 0.f, m_dir.z) * step );
//	m_pos += ( m_dir * step );
//	m_look += ( m_dir * step );
	m_look = character.GetPosition();// + Vector3(0,100.f,0);
//	m_pos = m_look - m_lookPos;
	m_pos = m_look - m_dir;
}


void cCamera::SetRotation(const float x, const float y)
{
//	Update();

	if( x != 0 )
	{ // rotate Y-Axis
		Quaternion q( m_up, -x * 0.005f ); 
		Matrix44 m = q.GetMatrix();
		Vector3 currDir(m_pos - m_look);
		currDir *= m;
		m_pos = m_look + currDir;
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
}

void cCamera::SetView()
{
	Update();
	
	Matrix44 view;
//	view.SetView( m_pos, m_dir, m_up );
	view.SetView( m_pos, m_dir.Normal(), m_up );
	graphic::GetDevice()->SetTransform(D3DTS_VIEW, (D3DXMATRIX*)&view);

	//dbg::Print("%f,%f,%f", m_pos.x,m_pos.y,m_pos.z);
}

/* backup1
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

/* backup2
void cCamera::SetPosition(const Vector3& characterPos)
{
	m_look = characterPos;
	m_pos = m_look - m_lookPos;
}
*/