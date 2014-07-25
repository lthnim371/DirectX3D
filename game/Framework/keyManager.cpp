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

//Ű�� �ѹ��� �����ִ��� �˻�
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

//Ű�� �ѹ��� �������� �˻�
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

//��� ������ ������
bool keyManager::isStayKey(int key)
{
	if (GetAsyncKeyState(key) & 0x8000) return true;
	return false;
}

//���Ű
bool keyManager::isToggleKey(int key)
{
	if (GetAsyncKeyState(key) & 0x0001) return true;
	return false;
}

