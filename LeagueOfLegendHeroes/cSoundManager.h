#pragma once

//fmod hpp추가!!
#include "inc/fmod.hpp"

//lib링크...
#pragma comment (lib, "lib/fmodex_vc.lib")

using namespace FMOD;

#define g_pSoundManager cSoundManager::GetInstance()

#define EXTRACHANNELBUFFER 5
#define SOUNDBUFFER 30

#define TOTALSOUNDBUFFER (EXTRACHANNELBUFFER + SOUNDBUFFER)

class cSoundManager
{
	SINGLETON(cSoundManager);

private:
	typedef map<string, Sound**> arrSounds;
	typedef map<string, Sound**>::iterator arrSoundsIter;
	typedef map<string, Channel**> arrChannels;
	typedef map<string, Channel**>::iterator arrChannelsIter;
	
private:
	System* m_ppSystem;
	Sound** m_ppSound;
	Channel** m_ppChannel;
	arrSounds m_mapTotalSounds;

public:
	
	//flac, mp3, wave, midi, ogg, m4a, aac, aif, aiff, wma
	void AddSound(string keyName, string soundName, bool background, bool loop);
	void Play(string keyName);

	void Play(string keyName, float volume);
	void Pause(string keyName);
	void Resume(string keyName);
	void Stop(string keyName);

	bool IsPauseSound(string keyName);
	bool IsPlaySound(string keyName);

	unsigned int GetLength(string keyName);
	void SetPosition(string keyName, unsigned int ms);

	unsigned int GetPosition(string keyName);

	//fmod 시스템 갱신...
	void Update(void);
	void Destroy();
};

