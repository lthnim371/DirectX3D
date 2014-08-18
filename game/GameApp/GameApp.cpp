
#include "stdafx.h"
#include "GameApp.h"

//#include <windowsx.h>  //for GET_X_LPARAM & GET_Y_LPARAM

INIT_FRAMEWORK(cGameApp);

cGameApp::cGameApp()
{
	m_windowName = L"GameApp";
	const RECT r = {0, 0, 1280, 720};
	m_windowRect = r;

//	character1 = new graphic::cCharacter(0);
//	character2 = new graphic::cCharacter(1);

//	m_stage = framework::GetStageMgr()->FindStage(framework::GetStageMgr()->MAIN);
}

cGameApp::~cGameApp()
{
//	SAFE_DELETE(character1);
//	SAFE_DELETE(character2);
	SAFE_DELETE(m_mouseCursor);
	m_sprite->Release();
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

	//마우스 가두기 및 숨기기
	bClipCursor = true;
	POINT lt = {m_windowRect.left, m_windowRect.top};
	POINT rb = {m_windowRect.right, m_windowRect.bottom};
	::ClientToScreen(m_hWnd, &lt);
	::ClientToScreen(m_hWnd, &rb);
	RECT rc = sRect(lt.x, lt.y, rb.x - 20, rb.y - 20);
	::ClipCursor( &rc );
	::ShowCursor(FALSE);

	//마우스 초기 위치 받아오기
	POINT ptCurrMouse;
	::GetCursorPos( &ptCurrMouse );
	::ScreenToClient( m_hWnd, &ptCurrMouse );

	D3DXCreateSprite(graphic::GetDevice(), &m_sprite);
	m_mouseCursor = new framework::cTestScene(m_sprite, 0, "MouseCursor");
	m_mouseCursor->Create("../media/image/Cursor_I2.tga");
	m_mouseCursor->SetPos( Vector3( (float)ptCurrMouse.x, (float)ptCurrMouse.y, 0.f ) );

	framework::GetStageMgr()->SetWindowHandle(m_hWnd);
	framework::GetStageMgr()->GetStage()->Init();

	return true;
}

void cGameApp::OnInput(const float elapseT)
{
	//마우스 가두기 및 숨기기
	if(	InputMgr->isOnceKeyDown( VK_BACK ) )
	{
		if( ::GetFocus() == m_hWnd )
		{
			bClipCursor = !bClipCursor;

			if( bClipCursor )
			{
				POINT lt = {m_windowRect.left, m_windowRect.top};
				POINT rb = {m_windowRect.right, m_windowRect.bottom};
				::ClientToScreen(m_hWnd, &lt);
				::ClientToScreen(m_hWnd, &rb);
				RECT rc = sRect(lt.x, lt.y, rb.x - 20, rb.y - 20);
				::ClipCursor( &rc );
				::ShowCursor(FALSE);
			}
			else if( !bClipCursor )
			{
				::ClipCursor( NULL );
				::ShowCursor(TRUE);
			}
		}
	}

	if( m_hWnd == ::GetFocus() )  //활성화가 된 클라이언트만 입력키 받게끔 설정
		framework::GetStageMgr()->GetStage()->Input( elapseT );
	//	framework::GetStageMgr()->GetStage()->Input( elapseT, character1, character2 );
		
}

void cGameApp::OnUpdate(const float elapseT)
{
//	framework::GetStageMgr()->GetStage()->Update( elapseT, character1, character2  );
	framework::GetStageMgr()->GetStage()->Update( elapseT );
	
//	graphic::GetCamera()->SetView();
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
		
//		framework::GetStageMgr()->GetStage()->Render( elapseT, character1, character2  );
		framework::GetStageMgr()->GetStage()->Render( elapseT );

		POINT ptCurrMouse;
		::GetCursorPos( &ptCurrMouse );
		::ScreenToClient( m_hWnd, &ptCurrMouse );
		m_mouseCursor->SetPos( Vector3( (float)ptCurrMouse.x, (float)ptCurrMouse.y, 0.f) );
		m_mouseCursor->Render( Matrix44() );

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

	//button클래스의 충돌을 실행하기 위함(ex : 마우스클릭)
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
			if( framework::GetStageMgr()->GetStage() )
			{
				switch( framework::GetStageMgr()->GetCurrentStage() )
				{
					case 0:  //MAIN
						{
							framework::cStage_Main* pMain = dynamic_cast<framework::cStage_Main*>( framework::GetStageMgr()->GetStage() );
							pMain->MessageProc(message, wParam, lParam);
						}
					break;
					case 1:  //CHARACTER_SELECT
						{
							framework::cStage_CharacterSelect* pCharSel = dynamic_cast<framework::cStage_CharacterSelect*>( framework::GetStageMgr()->GetStage() );
							pCharSel->MessageProc(message, wParam, lParam);
						}
					break;
					case 2:  //NETWORK_SELECT
						{
							framework::cStage_NetworkSelect* pNetSel = dynamic_cast<framework::cStage_NetworkSelect*>( framework::GetStageMgr()->GetStage() );
							pNetSel->MessageProc(message, wParam, lParam);
						}
					break;
					case 3:  //NETWORK_LOADING
						{
							framework::cStage_NetworkLoading* pNetLoad = dynamic_cast<framework::cStage_NetworkLoading*>( framework::GetStageMgr()->GetStage() );
							pNetLoad->MessageProc(message, wParam, lParam);
						}
					break;
				}
			}
		break;
		
	}

}