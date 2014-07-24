
#include "stdafx.h"
#include "GameApp.h"

#include <windowsx.h>  //for GET_X_LPARAM & GET_Y_LPARAM

INIT_FRAMEWORK(cGameApp);

cGameApp::cGameApp()
{
	m_windowName = L"GameApp";
	const RECT r = {0, 0, 1280, 800};
	m_windowRect = r;

	test = new graphic::cCharacter(0);
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

//	test->Create( "..\\media\\valle(new)\\valle1.dat" );
	test->Create( "..\\media\\valle(new)\\valle2.dat" );

	return true;
}


void cGameApp::OnUpdate(const float elapseT)
{
//	graphic::GetRenderer()->Update(elapseT);  //fps ������Ʈ
	::GetCursorPos(&currMouse);
	::ScreenToClient(m_hWnd, &currMouse);
	graphic::GetCamera()->SetView();

	test->Move(elapseT);
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

		test->Render();

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
	switch (message)
	{
		/*case WM_KEYDOWN:
			if( (::GetAsyncKeyState('W') & 0x8000) == 0x8000 )
			{		
				graphic::GetCamera()->SetTranslation(5.f);
				test->Action(test->FORWARD);
			}
			else if( (::GetAsyncKeyState('S') & 0x8000) == 0x8000 )
			{		
				graphic::GetCamera()->SetTranslation(-5.f);
				test->Action(test->BACKWARD);
			}
		break;*/

		case WM_MOUSEMOVE:
			{	
				prevMouse = currMouse;
				currMouse.x = GET_X_LPARAM(lParam);
				currMouse.y = GET_Y_LPARAM(lParam);

				float x_axis = currMouse.x - prevMouse.x;
				float y_axis = currMouse.y - prevMouse.y;

				graphic::GetCamera()->SetRotation(test->GetTM(), x_axis, y_axis );
				//graphic::GetCamera()->SetView();
			}
		break;
	}
}