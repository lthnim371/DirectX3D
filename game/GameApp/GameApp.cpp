
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
		0, // 활성화/ 비활성화 하려는 광원 리스트 내의 요소
		true); // true = 활성화 ， false = 비활성화

	test->Create( "..\\media\\valle(new)\\valle1.dat" );

	return true;
}


void cGameApp::OnUpdate(const float elapseT)
{
//	graphic::GetRenderer()->Update(elapseT);  //fps 업데이트
	::GetCursorPos(&currMouse);
	::ScreenToClient(m_hWnd, &currMouse);
	graphic::GetCamera()->SetView();

	test->Move(elapseT);
}


void cGameApp::OnRender(const float elapseT)
{
	//화면 청소
	if (SUCCEEDED(graphic::GetDevice()->Clear( 
		0,			//청소할 영역의 D3DRECT 배열 갯수		( 전체 클리어 0 )
		NULL,		//청소할 영역의 D3DRECT 배열 포인터		( 전체 클리어 NULL )
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,	//청소될 버퍼 플레그 ( D3DCLEAR_TARGET 컬러버퍼, D3DCLEAR_ZBUFFER 깊이버퍼, D3DCLEAR_STENCIL 스텐실버퍼
		D3DCOLOR_XRGB(255, 255, 255),			//컬러버퍼를 청소하고 채워질 색상( 0xAARRGGBB )
		1.0f,				//깊이버퍼를 청소할값 ( 0 ~ 1 0 이 카메라에서 제일가까운 1 이 카메라에서 제일 먼 )
		0					//스텐실 버퍼를 채울값
		)))
	{
		//화면 청소가 성공적으로 이루어 졌다면... 랜더링 시작
		graphic::GetDevice()->BeginScene();
		
	//fps 및 그리드 출력
		graphic::GetRenderer()->RenderFPS();
		graphic::GetRenderer()->RenderGrid();
		graphic::GetRenderer()->RenderAxis();

		test->Render();

		//랜더링 끝
		graphic::GetDevice()->EndScene();
		//랜더링이 끝났으면 랜더링된 내용 화면으로 전송
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
		case WM_MOUSEMOVE:
			{	
				prevMouse = currMouse;
				currMouse.x = GET_X_LPARAM(lParam);
				currMouse.y = GET_Y_LPARAM(lParam);

				float x_axis = currMouse.x - prevMouse.x;
				float y_axis = currMouse.y - prevMouse.y;

				graphic::GetCamera()->SetRotation( x_axis, y_axis );
				graphic::GetCamera()->SetView();
			}
		break;
	}
}