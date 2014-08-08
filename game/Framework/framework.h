
#pragma once


#include "gamemain/GameMain.h"

#pragma comment( lib, "winmm.lib" )


// �����ӿ�ũ �ʱ�ȭ. 
// WinMain �� �����Ѵ�.
#define INIT_FRAMEWORK(frameWorkClassName) \
	framework::cGameMain* framework::CreateFrameWork()\
	{\
		return new frameWorkClassName();\
	}\
	\
	int APIENTRY WinMain(HINSTANCE hInstance, \
		HINSTANCE hPrevInstance, \
		LPSTR lpCmdLine, \
		int nCmdShow)\
	{\
		framework::FrameWorkWinMain(hInstance, hPrevInstance, lpCmdLine, nCmdShow);\
	}

// Include
#include "ui/uidef.h"
#include "ui/event.h"
#include "ui/eventhandler.h"
#include "ui/window.h"
#include "ui/button.h"


//�߰�
#include "keyManager.h"
#define InputMgr framework::keyManager::Get()

#include "TestScene.h"
#include "stage.h"
#include "stage_main.h"
#include "stage_select.h"
#include "stage_ingame.h"
#include "stagemanager.h"