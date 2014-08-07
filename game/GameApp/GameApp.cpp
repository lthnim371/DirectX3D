
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
	character2 = new graphic::cCharacter(1);
}

cGameApp::~cGameApp()
{
	SAFE_DELETE(character);
	SAFE_DELETE(character2);
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

	character->Create( "..\\media\\mesh\\valle\\valle_character1.dat" );
	character->LoadWeapon( "..\\media\\mesh\\valle\\valle_weapon1.dat" );
//test
//	character->SetRenderBoundingBox(true);

	character->SetAnimation( "..\\media\\ani\\valle\\valle1_backward.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_forward.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_dash.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_LA.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_LLA.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_LLLA.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_LRA.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_LLRA.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_RA.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_RRA.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_RRRA.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_jump1.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_jump2.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_jump3.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_JLA.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back1.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_back2.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front1.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_hit_front2.ani" );
	character->SetAnimation( "..\\media\\ani\\valle\\valle1_normal.ani" );

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

	sRect rc;
	::GetClientRect(m_hWnd, &rc);
	POINT lt = {rc.left, rc.top};
	POINT rb = {rc.right, rc.bottom};
	::ClientToScreen(m_hWnd, &lt);
	::ClientToScreen(m_hWnd, &rb);
	rc = sRect(lt.x, lt.y, rb.x, rb.y);
	::ClipCursor( &rc );

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
	
	//프로그램 테스트용
	else if( InputMgr->isOnceKeyDown('1') )
	{
		graphic::GetCamera()->SetHeight(-10.f);
	}
	else if( InputMgr->isOnceKeyDown('2') )
	{
		graphic::GetCamera()->SetHeight(10.f);
	}

	if( InputMgr->isOnceKeyDown( VK_LBUTTON ) )
	{
		character->Update( character->LATTACK );
	}
	else if( InputMgr->isOnceKeyDown( VK_RBUTTON ) )
	{
		character->Update( character->RATTACK );
	}
	else if( InputMgr->isStayKey('W') )
	{	
		if( InputMgr->isStayKey(VK_SHIFT) )
			character->Update( character->DASH );
		else if( InputMgr->isOnceKeyDown(VK_SPACE) )
			character->Update( character->FRONTJUMP );
		else
			character->Update( character->FORWARD );
	}
	else if( InputMgr->isStayKey('S') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			character->Update( character->BACKJUMP );
		else
			character->Update( character->BACKWARD );
	}
	else if( InputMgr->isStayKey('A') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			character->Update( character->LEFTJUMP );
		else
			character->Update( character->LEFTWARD );
	}
	else if( InputMgr->isStayKey('D') )
	{	
		if( InputMgr->isOnceKeyDown(VK_SPACE) )
			character->Update( character->RIGHTJUMP );
		else
			character->Update( character->RIGHTWARD );
	}
	else if( InputMgr->isOnceKeyDown(VK_SPACE) )
	{	
		character->Update( character->JUMP );
	}
	else
	{
		character->Update( character->NORMAL );
	}
}

void cGameApp::OnUpdate(const float elapseT)
{
//	graphic::GetRenderer()->Update(elapseT);  //fps 업데이트
	
	bool bAniState = character->Move(elapseT);
	bool bAniState2 = character2->Move(elapseT);
//	graphic::GetCamera()->SetPosition( character->GetTM() );

	if( character->GetCubeCheck() == true )
	{
		if( true == character2->CollisionCheck( *(character->GetWeaponCube()), graphic::GetCamera()->GetLook() ) )
		{
			character->SetAttackSuccess();
		}
	}
	else if( character2->GetCubeCheck() == true )
	{
		if ( true == character->CollisionCheck( *(character2->GetWeaponCube()), graphic::GetCamera()->GetLook() ) )
		{
			character2->SetAttackSuccess();
		}
	}

	if( character->GetMode() == character->BEHIT )
	{
		if( true == character->CollisionCheck( *(character2->GetCharacterCube()), graphic::GetCamera()->GetLook(), graphic::GetCamera()->GetDirection() ) )
			character->UpdateBeHit( bAniState, graphic::GetCamera()->GetDirection(), character2->GetAniPosGap()  );
	}
	else if( character2->GetMode() == character2->BEHIT )
	{
		if( true == character2->CollisionCheck( *(character->GetCharacterCube()), graphic::GetCamera()->GetLook(), graphic::GetCamera()->GetDirection() ) )
			character2->UpdateBeHit( bAniState2, graphic::GetCamera()->GetDirection(), character->GetAniPosGap() );
	}

	graphic::GetCamera()->SetView();
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

//		character->Render();
//		character2->Render();

	//test
		character->RenderShader( graphic::GetCamera()->GetShader() );
		character2->RenderShader( graphic::GetCamera()->GetShader() );				

	//test
		graphic::GetCamera()->Render( character->GetHP(), character2->GetHP() );

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