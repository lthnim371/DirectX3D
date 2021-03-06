
#pragma once


#include "gamemain/GameMain.h"

#pragma comment( lib, "winmm.lib" )


// 프레임워크 초기화. 
// WinMain 을 실행한다.
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


//추가
#include "keyManager.h"
#include "Sound.h"
#define InputMgr framework::keyManager::Get()
#define SndDepot framework::SoundManager::Get()

#include "TestScene.h"
#include "stage.h"
#include "stage_main.h"
#include "stage_CharacterSelect.h"
#include "stage_NetworkSelect.h"
#include "stage_NetworkLoading.h"
#include "stage_IngameStart.h"
#include "stage_ingame.h"
#include "stage_IngameEnd.h"
#include "stage_Ending.h"
#include "stagemanager.h"