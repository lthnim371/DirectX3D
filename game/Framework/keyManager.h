#pragma once
#include <bitset>

using namespace std;

#define KEYMAX 256

namespace framework
{
	class keyManager : public common::cSingleton<keyManager>
	{
	private:
		bitset<KEYMAX> _keyUp;
		bitset<KEYMAX> _keyDown;

	public:
		HRESULT init(void);
		void release(void);
		bool isOnceKeyDown(int key);
		bool isOnceKeyUp(int key);
		bool isStayKey(int key);
		bool isToggleKey(int key);

		bitset<KEYMAX> getKeyUp() {return _keyUp;}
		bitset<KEYMAX> getKeyDown() {return _keyDown;}

		void setKeyUp(int key, bool state) {_keyUp.set(key, state);}
		void setKeyDown(int key, bool state) {_keyDown.set(key, state);}

		keyManager(void);
		~keyManager(void);
	};
}