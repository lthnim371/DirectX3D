
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
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ

	//���콺 ���α� �� �����
	bClipCursor = true;
	POINT lt = {m_windowRect.left, m_windowRect.top};
	POINT rb = {m_windowRect.right, m_windowRect.bottom};
	::ClientToScreen(m_hWnd, &lt);
	::ClientToScreen(m_hWnd, &rb);
	RECT rc = sRect(lt.x, lt.y, rb.x - 20, rb.y - 20);
	::ClipCursor( &rc );
	::ShowCursor(FALSE);

	//���콺 �ʱ� ��ġ �޾ƿ���
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
	const string& strCurrSound = framework::GetStageMgr()->GetSoundName();
		
	if( ::GetFocus() == m_hWnd )
	{
		framework::GetStageMgr()->GetStage()->Input( elapseT );

	//���콺 ���α� �� �����
		if(	InputMgr->isOnceKeyDown( VK_BACK ) )
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

		if(SndDepot->get( strCurrSound )->IsPaused() == true)
		{
			SndDepot->get( strCurrSound )->Pause(false);
		}
		else if(SndDepot->get( strCurrSound )->IsPlaying() == false)
		{
			string strNextSound;

			if( strCurrSound == "MainTema2")
				strNextSound.assign( "MainTema3" );
			else if( strCurrSound == "MainTema3")
				strNextSound.assign( "MainTema4" );
			else if( strCurrSound == "MainTema4")
				strNextSound.assign( "MainTema5" );
			else if( strCurrSound == "MainTema5")
				strNextSound.assign( "MainTema2" );
		//	else if( strCurrSound == "MainTema1" )
		//		return;
			SndDepot->get( strCurrSound )->Stop();
			SndDepot->get( strNextSound )->Play();
			framework::GetStageMgr()->SetSoundName( strNextSound );
		}
	}
	else if(SndDepot->get( strCurrSound )->IsPlaying() == true)
	{
		SndDepot->get( strCurrSound )->Pause(true);
	}

//	if( m_hWnd == ::GetFocus() )  //Ȱ��ȭ�� �� Ŭ���̾�Ʈ�� �Է�Ű �ްԲ� ����
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
		
//		framework::GetStageMgr()->GetStage()->Render( elapseT, character1, character2  );
		framework::GetStageMgr()->GetStage()->Render( elapseT );

		POINT ptCurrMouse;
		::GetCursorPos( &ptCurrMouse );
		::ScreenToClient( m_hWnd, &ptCurrMouse );
		m_mouseCursor->SetPos( Vector3( (float)ptCurrMouse.x, (float)ptCurrMouse.y, 0.f) );
		m_mouseCursor->Render( Matrix44() );

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

	//buttonŬ������ �浹�� �����ϱ� ����(ex : ���콺Ŭ��)
		case WM_MOUSEMOVE:
		case WM_LBUTTONDOWN:
			if( framework::GetStageMgr()->GetStage() )
			{
				switch( framework::GetStageMgr()->GetCurrentStage() )
				{
					case framework::cStageMgr::MAIN:  //MAIN
						{
							framework::cStage_Main* pMain = dynamic_cast<framework::cStage_Main*>( framework::GetStageMgr()->GetStage() );
							pMain->MessageProc(message, wParam, lParam);
						}
					break;
					case framework::cStageMgr::CHARACTER_SELECT:  //CHARACTER_SELECT
						{
							framework::cStage_CharacterSelect* pCharSel = dynamic_cast<framework::cStage_CharacterSelect*>( framework::GetStageMgr()->GetStage() );
							pCharSel->MessageProc(message, wParam, lParam);
						}
					break;
					case framework::cStageMgr::NETWORK_SELECT:  //NETWORK_SELECT
						{
							framework::cStage_NetworkSelect* pNetSel = dynamic_cast<framework::cStage_NetworkSelect*>( framework::GetStageMgr()->GetStage() );
							pNetSel->MessageProc(message, wParam, lParam);
						}
					break;
					case framework::cStageMgr::NETWORK_LOADING:  //NETWORK_LOADING
						{
							framework::cStage_NetworkLoading* pNetLoad = dynamic_cast<framework::cStage_NetworkLoading*>( framework::GetStageMgr()->GetStage() );
							pNetLoad->MessageProc(message, wParam, lParam);
						}
					break;
					case framework::cStageMgr::ENDING:  //ENDING
						{
							framework::cStage_Ending* pEnding = dynamic_cast<framework::cStage_Ending*>( framework::GetStageMgr()->GetStage() );
							pEnding->MessageProc(message, wParam, lParam);
						}
					break;
				}
			}
		break;
		
	}

}