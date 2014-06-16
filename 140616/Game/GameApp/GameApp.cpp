
#include "stdafx.h"
#include "GameApp.h"

INIT_FRAMEWORK(cGameApp)  //1. 정의된 매크로로 프레임워크에 사용가능한 클래스명을 넘긴다(윈메인 생성)


cGameApp::cGameApp()
{
	m_windowName = L"GameApp";
	const RECT r = {0, 0, 800, 600};
	m_windowRect = r;
}

cGameApp::~cGameApp()
{

}


bool cGameApp::OnInit()
{

	return true;
}


void cGameApp::OnUpdate(const float elapseT)
{

}


void cGameApp::OnRender(const float elapseT)
{

}


void cGameApp::OnShutdown()
{

}


void cGameApp::MessageProc( UINT message, WPARAM wParam, LPARAM lParam)
{

}
