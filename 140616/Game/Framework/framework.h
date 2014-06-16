
#pragma once


#include "gamemain/GameMain.h"

#pragma comment( lib, "winmm.lib" )


// �����ӿ�ũ �ʱ�ȭ. 
// WinMain �� �����Ѵ�.
#define INIT_FRAMEWORK(frameWorkClassName) \  //2. 
	framework::cGameMain* framework::CreateFrameWork()\  //CreateFrameWork �Լ��� �����Ѵ�
	{\
		return new frameWorkClassName();\  //�Ѱܹ��� Ŭ����Ÿ������ �����Ҵ��Ѵ�
	}\
	\
	int APIENTRY WinMain(HINSTANCE hInstance, \
		HINSTANCE hPrevInstance, \
		LPSTR lpCmdLine, \
		int nCmdShow)\
	{\
		framework::FrameWorkWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);\
	}
