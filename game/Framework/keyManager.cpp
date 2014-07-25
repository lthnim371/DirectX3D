#include "stdafx.h"
#include "keyManager.h"

using namespace framework;

keyManager::keyManager(void)
{
}

keyManager::~keyManager(void)
{
}

HRESULT keyManager::init(void)
{
	for (int i = 0; i < KEYMAX; i++)
	{
		this->getKeyUp().set(i, false);
		this->getKeyDown().set(i, false);
	}
	
	return S_OK;
}

void keyManager::release(void)
{
}

//키가 한번만 눌려있는지 검사
bool keyManager::isOnceKeyDown(int key)
{                                   
	if (GetAsyncKeyState(key) & 0x8000)
	{
		if (!this->getKeyDown()[key])
		{
			this->setKeyDown(key, true);
			return true;
		}
	}
	else this->setKeyDown(key, false);
	return false;
}

//키가 한번만 떼었을때 검사
bool keyManager::isOnceKeyUp(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) this->setKeyUp(key, true);
	else
	{
		if (this->getKeyUp()[key])
		{
			this->setKeyUp(key, false);
			return true;
		}
	}
	return false;
}

//계속 누르고 있을때
bool keyManager::isStayKey(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

//토글키
bool keyManager::isToggleKey(int key)
{
	if (GetAsyncKeyState(key) & 0x0001) return true;
	return false;
}

