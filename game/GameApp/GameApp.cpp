
#include "stdafx.h"
#include "GameApp.h"

#include <windowsx.h>  //for GET_X_LPARAM & GET_Y_LPARAM

INIT_FRAMEWORK(cGameApp);

cGameApp::cGameApp()
{
	m_windowName = L"GameApp";
	const RECT r = {0, 0, 1280, 800};
	m_windowRect = r;

	character = new graphic::cCharacter(0);
}

cGameApp::~cGameApp()
{
}


bool cGameApp::OnInit()
{
	m_mtrl.InitRed();

	Vector4 color(1,1,1,1);
	m_light.Init( graphic::cLight::LIGHT_DIRECTIONAL, 
		color * 0.4f,
		color,
		color * 0.6f,
		Vector3(1,0,0));
	
	m_light.Bind(0);

	graphic::GetDevice()->LightEnable (
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ

	character->Create( "..\\media\\valle\\valle_character2.dat" );
	character->LoadWeapon( "..\\media\\valle\\valle_weapon2.dat" );
//debug��	
//	character->SetRenderBoundingBox(true);

	character->SetAnimation( "..\\media\\valle\\valle_backward.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_forward.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_LA.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_LLA.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_LLLA.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_LRA.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_LLRA.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_RA.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_RRA.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_RRRA.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_jump1.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_jump2.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_jump3.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_jump_LA.ani" );
	character->SetAnimation( "..\\media\\valle\\valle_normal.ani" );

	::GetCursorPos( &m_currMouse );
	::ScreenToClient( m_hWnd, &m_currMouse );
//	m_prevMouse = m_currMouse;
	m_bMouse = false;

	return true;
}

void cGameApp::OnInput(const float elapseT)
{
	m_prevMouse = m_currMouse;
	::GetCursorPos( &m_currMouse );
	::ScreenToClient( m_hWnd, &m_currMouse );
	if( m_currMouse.x != m_prevMouse.x || m_currMouse.y != m_prevMouse.y )
//		m_bMouse = true;
	{
		//character->Action( character->ROTATION, m_currMouse.x );
		POINT ptMouse;
		ptMouse.x = m_currMouse.x - m_prevMouse.x;
		ptMouse.y = m_currMouse.y - m_prevMouse.y;
		character->Update( character->ROTATION, ptMouse.x, ptMouse.y );
	}
		
	if( InputMgr->isStayKey('W') && InputMgr->isStayKey(VK_SHIFT) )
	{		
		character->Update( character->DASH );
	}
	else if( InputMgr->isStayKey('W') )
	{		
		character->Update( character->FORWARD );
	}
	else if( InputMgr->isStayKey('S') )
	{	
		character->Update( character->BACKWARD );
	}
	else if( InputMgr->isStayKey('A') )
	{	
		character->Update( character->LEFTWARD );
	}
	else if( InputMgr->isStayKey('D') )
	{	
		character->Update( character->RIGHTWARD );
	}
	else if( InputMgr->isStayKey(VK_SPACE) )
	{	
		character->Update( character->JUMP );
	}
	else if( InputMgr->isOnceKeyDown( VK_LBUTTON ) )
	{
		character->Update( character->LATTACK );
	}
	else if( InputMgr->isOnceKeyDown( VK_RBUTTON ) )
	{
		character->Update( character->RATTACK );
	}
	else
	{
		character->Update( character->NORMAL );
	}
}

void cGameApp::OnUpdate(const float elapseT)
{
//	graphic::GetRenderer()->Update(elapseT);  //fps ������Ʈ
	
	character->Move(elapseT);
//	graphic::GetCamera()->SetPosition( character->GetTM() );
	graphic::GetCamera()->SetView();
}


void cGameApp::OnRender(const float elapseT)
{
	//ȭ�� û��
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//û���� ������ D3DRECT �迭 ����		( ��ü Ŭ���� 0 )
		NULL,		//û���� ������ D3DRECT �迭 ������		( ��ü Ŭ���� NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//û�ҵ� ���� �÷��� ( D3DCLEAR_TARGET �÷�����, D3DCLEAR_ZBUFFER ���̹���, D3DCLEAR_STENCIL ���ٽǹ���
		D3DCOLOR_XRGB(255, 255, 255),			//�÷����۸� û���ϰ� ä���� ����( 0xAARRGGBB )
		1.0f,				//���̹��۸� û���Ұ� ( 0 ~ 1 0 �� ī�޶󿡼� ���ϰ���� 1 �� ī�޶󿡼� ���� �� )
		0					//���ٽ� ���۸� ä�ﰪ
		)))
	{
		//ȭ�� û�Ұ� ���������� �̷�� ���ٸ�... ������ ����
		graphic::GetDevice()->BeginScene();
		
	//fps �� �׸��� ���
		graphic::GetRenderer()->RenderFPS();
		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();

		character->Render();

		//������ ��
		graphic::GetDevice()->EndScene();
		//�������� �������� �������� ���� ȭ������ ����
		graphic::GetDevice()->Present( NULL, NULL, NULL, NULL );
	}
}


void cGameApp::OnShutdown()
{

}


void cGameApp::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{

//test
/*
	switch (message)
	{
		case WM_KEYDOWN:
			if( (::GetAsyncKeyState('W') & 0x8000) == 0x8000 )
			{		
				graphic::GetCamera()->SetTranslation(5.f);
				character->Action(character->FORWARD);
			}
			else if( (::GetAsyncKeyState('S') & 0x8000) == 0x8000 )
			{		
				graphic::GetCamera()->SetTranslation(-5.f);
				character->Action(character->BACKWARD);
			}
		break;
		
		
		case WM_MOUSEMOVE:
			{	
				if(m_bMouse)
				{
					m_prevMouse = m_currMouse;
					m_currMouse.x = GET_X_LPARAM(lParam);
					m_currMouse.y = GET_Y_LPARAM(lParam);
					
					float x_axis = m_currMouse.x - m_prevMouse.x;
					float y_axis = m_currMouse.y - m_prevMouse.y;

					graphic::GetCamera()->SetRotation( x_axis, y_axis );

				//	character->SetRotation(  x_axis, y_axis );
				}
			}
		break;
		
	}
*/
}