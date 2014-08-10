
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
/*
	character1->Create( "..\\media\\mesh\\valle\\valle_character1.dat" );
	character1->LoadWeapon( "..\\media\\mesh\\valle\\valle_weapon1.dat" );
//test
//	character1->SetRenderBoundingBox(true);

	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_backward.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_dash.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LA.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LLA.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LLLA.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LRA.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_LLRA.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_RA.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_RRA.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_RRRA.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_jump1.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_jump2.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_jump3.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_JLA.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back1.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back2.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front1.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front2.ani" );
	character1->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );

//test
	character2->Create( "..\\media\\mesh\\valle\\valle_character1.dat" );
	character2->LoadWeapon( "..\\media\\mesh\\valle\\valle_weapon1.dat" );
	character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back1.ani" );
	character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back2.ani" );
	character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front1.ani" );
	character2->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front2.ani" );
	character2->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );
	Matrix44 pos;
	pos.SetTranslate( Vector3( 0, 0, 500.f) );
	character2->MultiplyTM( pos );
*/
/*  마우스 가두기
	sRect rc;
	::GetClientRect(m_hWnd, &rc);
	POINT lt = {rc.left, rc.top};
	POINT rb = {rc.right, rc.bottom};
	::ClientToScreen(m_hWnd, &lt);
	::ClientToScreen(m_hWnd, &rb);
	rc = sRect(lt.x, lt.y, rb.x, rb.y);
	::ClipCursor( &rc );
*/
/*
//마우스 초기 위치 받아오기
	::GetCursorPos( &m_currMouse );
	::ScreenToClient( m_hWnd, &m_currMouse );
//	m_prevMouse = m_currMouse;
	m_bMouse = false;
*/
	framework::GetStageMgr()->SetWindowHandle(m_hWnd);
	framework::GetStageMgr()->GetStage()->Init();

	return true;
}

void cGameApp::OnInput(const float elapseT)
{
//	framework::GetStageMgr()->GetStage()->Input( elapseT, character1, character2 );
	framework::GetStageMgr()->GetStage()->Input( elapseT );
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
		case WM_LBUTTONDOWN:
			if( framework::GetStageMgr()->GetStage() )
			{
				switch( framework::GetStageMgr()->GetCurrentStage() )
				{
					case 0:
						{
							framework::cStage_Main* pMain = dynamic_cast<framework::cStage_Main*>( framework::GetStageMgr()->GetStage() );
							pMain->MessageProc(message, wParam, lParam);
						}
					break;
					case 1:
						{
							framework::cStage_Select* pMain = dynamic_cast<framework::cStage_Select*>( framework::GetStageMgr()->GetStage() );
							pMain->MessageProc(message, wParam, lParam);
						}
					break;
				}
			}
		break;
		
	}

}