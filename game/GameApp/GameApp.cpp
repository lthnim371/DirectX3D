
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
		0, // Ȱ��ȭ/ ��Ȱ��ȭ �Ϸ��� ���� ����Ʈ ���� ���
		true); // true = Ȱ��ȭ �� false = ��Ȱ��ȭ

/*  ���콺 ���α�
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
//���콺 �ʱ� ��ġ �޾ƿ���
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
	if( m_hWnd == ::GetFocus() )  //Ȱ��ȭ�� �� Ŭ���̾�Ʈ�� �Է�Ű �ްԲ� ����
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
					case 0:  //MAIN
						{
							framework::cStage_Main* pMain = dynamic_cast<framework::cStage_Main*>( framework::GetStageMgr()->GetStage() );
							pMain->MessageProc(message, wParam, lParam);
						}
					break;
					case 1:  //NETWORK_SELECT
						{
							framework::cStage_NetworkSelect* pNetSel = dynamic_cast<framework::cStage_NetworkSelect*>( framework::GetStageMgr()->GetStage() );
							pNetSel->MessageProc(message, wParam, lParam);
						}
					break;
					case 2:  //NETWORK_LOADING
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