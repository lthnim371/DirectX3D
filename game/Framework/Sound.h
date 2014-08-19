#pragma once

#pragma comment(lib, "fmod\\fmodex_vc.lib")

#include <windows.h>
#include <tchar.h>
#include "../Common/utility/Utility.h"
#include "../Common/etc/singleton.h"
#include "../Common/utility/Manager.hpp"
#include "../fmod/inc/fmod.hpp"

namespace framework
{
	class Sound
	{
	   enum {maxchannel=100};
	public :
	   Sound();
	   virtual ~Sound();
	public :
	   Sound* clone();
	   void Load(LPCSTR lpszFileName, bool bFixed = true);
	   void Play();
	   void Stop();
	   void Pause(bool pause = true);

	   void PadeOut();
   
	   bool IsPlaying();
	   bool IsPaused();
	   void SetLoop(bool loop = true);
	   void SetLoopRange(unsigned int from, unsigned int to);
	private :
	   FMOD::Sound* hSound;
	   FMOD::Channel* hChannel;
	   bool bLoop;
	   bool bLoopRange;
	   unsigned int range_from;
	   unsigned int range_to;
	};

	class SoundManager : public Manager<Sound>, public common::cSingleton<SoundManager>
	{
	   friend class singleton<SoundManager>;
	   enum {maxchannel=100};
	private :
	   SoundManager();
	   ~SoundManager();
	public :
	   FMOD::System* getSystem() const;
	   FMOD::System* operator -> () const;

	private :
	   FMOD::System *system;
	};
}

//#define SndDepot SoundManager::getReference()