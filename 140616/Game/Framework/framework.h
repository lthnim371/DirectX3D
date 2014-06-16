
#pragma once


#include "gamemain/GameMain.h"

#pragma comment( lib, "winmm.lib" )


// 프레임워크 초기화. 
// WinMain 을 실행한다.
#define INIT_FRAMEWORK(frameWorkClassName) \  //2. 
	framework::cGameMain* framework::CreateFrameWork()\  //CreateFrameWork 함수를 정의한다
	{\
		return new frameWorkClassName();\  //넘겨받은 클래스타입으로 동적할당한다
	}\
	\
	int APIENTRY WinMain(HINSTANCE hInstance, \
		HINSTANCE hPrevInstance, \
		LPSTR lpCmdLine, \
		int nCmdShow)\
	{\
		framework::FrameWorkWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);\
	}
