//use debug
//	Vector3 test( m_look );
//	dbg::Print( "%f,%f,%f", test.x,test.y,test.z);

#include "stdafx.h"
#include "camera.h"

using namespace graphic;

cCamera::cCamera()
	: m_pos(0,300,300), m_look(0,0,0), m_up(0,1,0)
{
	SetView();  //set view
	
	//set projection
	const int WINSIZE_X = 1024;	//�ʱ� ������ ���� ũ��
	const int WINSIZE_Y = 768;	//�ʱ� ������ ���� ũ��
	
	Matrix44 proj;
	proj.SetProjection(D3DX_PI * 0.5f, (float)WINSIZE_X / (float) WINSIZE_Y, 1.f, 1000.0f) ;
	graphic::GetDevice()->SetTransform(D3DTS_PROJECTION, (D3DXMATRIX*)&proj) ;

	m_font = NULL;
	HRESULT hr = D3DXCreateFontA( ::GetDevice(), 50, 0, FW_BOLD, 1, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, 
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "����", &m_font );
}
cCamera::~cCamera()
{
	SAFE_RELEASE(m_font);
}

void cCamera::Update()
{
	m_dir = m_look - m_pos;
//	dbg::Print( "%f,%f", m_dir.x, m_dir.z);
//	m_dir.Normalize();  //dir�� ũ�Ⱑ �ʿ��� ���� �����Ƿ� ����ȭ���� ����
	m_right = m_up.CrossProduct( m_dir.Normal() );
	m_right.Normalize();
}

void cCamera::Render(const int hp, const int sp)
{
	RET(!m_font);

	char buff[32];
	::_itoa_s( hp, buff, sizeof(buff), 10 );
	string str("HP : ");
	str.append( buff );
	sRect rect(10,740,110,840);
	m_font->DrawTextA( NULL, str.c_str(), -1, (RECT*)&rect,
		DT_NOCLIP, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );

	::_itoa_s( sp, buff, sizeof(buff), 10 );
	str.assign("SP : ");
	str.append( buff );
	rect.SetX(1050);
	rect.SetY(740);
	m_font->DrawTextA( NULL, str.c_str(), -1, (RECT*)&rect,
		DT_NOCLIP, D3DXCOLOR( 1.0f, 0.0f, 0.0f, 1.0f ) );
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


void cCamera::SetRotation(const float x, const float y)  //x = 0, y = 0
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

//���α׷� �׽�Ʈ��
void cCamera::SetHeight(const float number)
{
	if( m_pos.y <= 100.f )
		m_pos.y += 10.f;
	else if( m_pos.y >= 500.f )
		m_pos.y -= 10.f;
	else
		m_pos.y += number;	
}