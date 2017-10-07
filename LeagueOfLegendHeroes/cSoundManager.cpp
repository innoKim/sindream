#include "StdAfx.h"
#include "cSoundManager.h"

cSoundManager::cSoundManager(void)
{
	System_Create(&m_ppSystem);

	//채널수 설정
	m_ppSystem->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//채널수와 사운드 갯수는 맞춰준다!!
	m_ppSound = new Sound*[TOTALSOUNDBUFFER];
	m_ppChannel = new Channel*[TOTALSOUNDBUFFER];

	//초기화
	ZeroMemory(m_ppSound, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	ZeroMemory(m_ppChannel, sizeof(Channel*) * (TOTALSOUNDBUFFER));
}

cSoundManager::~cSoundManager(void)
{
}


//flac, mp3, wave, midi, ogg, m4a, aac, aif, aiff, wma
void cSoundManager::AddSound(string keyName, string soundName, bool background, bool loop)
{
	//뺑뻉이 사운드냐?
	if (loop)
	{
		//bgm!!
		if (background)
		{
			//배경음악일때...
			m_ppSystem->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &m_ppSound[m_mapTotalSounds.size()]);
		}
		else
		{
			//일반 사운드면....
			m_ppSystem->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &m_ppSound[m_mapTotalSounds.size()]);
		}
	}
	else
	{
		m_ppSystem->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &m_ppSound[m_mapTotalSounds.size()]);
	}

	//맵에 사운드를 키값과 함께 넣어준다
	m_mapTotalSounds.insert(make_pair(keyName, &m_ppSound[m_mapTotalSounds.size()]));
}

void cSoundManager::Play(string keyName)
{
	arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_ppSystem->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &m_ppChannel[count]);
			m_ppChannel[count]->setVolume(0.3f);
			break;
		}
	}
}

void cSoundManager::Play(string keyName, float volume)
{
	arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			//플레이
			m_ppSystem->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &m_ppChannel[count]);
			m_ppChannel[count]->setVolume(volume);
			break;
		}
	}
}

//일시 정지
void cSoundManager::Pause(string keyName)
{
	arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_ppChannel[count]->setPaused(true);
			break;
		}
	}
}

//일시 정지된 음악 다시 재생
void cSoundManager::Resume(string keyName)
{
	arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_ppChannel[count]->setPaused(false);
			break;
		}
	}
}

void cSoundManager::Stop(string keyName)
{
	arrSoundsIter iter = m_mapTotalSounds.begin();

	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_ppChannel[count]->stop();
			break;
		}
	}
}

//일시 정지 되었는가?
bool cSoundManager::IsPauseSound(string keyName)
{
	arrSoundsIter iter = m_mapTotalSounds.begin();
	bool isPaused;
	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_ppChannel[count]->getPaused(&isPaused);
			break;
		}
	}

	return isPaused;
}

//현재 재생중이냐?
bool cSoundManager::IsPlaySound(string keyName)
{
	arrSoundsIter iter = m_mapTotalSounds.begin();
	bool isPlaying = false;
	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_ppChannel[count]->isPlaying(&isPlaying);
			break;
		}
	}

	return isPlaying;
}

//fmod 시스템 갱신...
void cSoundManager::Update(void)
{
	m_ppSystem->update();
}

//음악의 길이 값 가져온다 (밀리쎄껀 : 1000 == 1초)
unsigned int cSoundManager::GetLength(string keyName)
{
	arrSoundsIter iter = m_mapTotalSounds.begin();

	for (iter; iter != m_mapTotalSounds.end(); ++iter)
	{
		if (keyName == iter->first)
		{
			unsigned int length = 0;
			(*iter->second)->getLength(&length, FMOD_TIMEUNIT_MS);
			return length;
		}
	}

	return NULL;
}

//음악의 시작지점 설정한다
void cSoundManager::SetPosition(string keyName, unsigned int ms)
{
	arrSoundsIter iter = m_mapTotalSounds.begin();
	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			m_ppChannel[count]->setPosition(ms, FMOD_TIMEUNIT_MS);
			break;
		}
	}
}

//재생중인 음악의 현재 위치를 가져온다
unsigned int cSoundManager::GetPosition(string keyName)
{
	arrSoundsIter iter = m_mapTotalSounds.begin();
	int count = 0;

	for (iter; iter != m_mapTotalSounds.end(); ++iter, count++)
	{
		if (keyName == iter->first)
		{
			unsigned int position = 0;
			m_ppChannel[count]->getPosition(&position, FMOD_TIMEUNIT_MS);

			return position;
		}
	}

	return NULL;
}

void cSoundManager::Destroy()
{
	if (m_ppChannel != NULL || m_ppSound != NULL)
	{
		for (int i = 0; i < TOTALSOUNDBUFFER; i++)
		{
			if (m_ppChannel != NULL)
			{
				if (m_ppChannel[i]) m_ppChannel[i]->stop();
			}

			if (m_ppSound != NULL)
			{
				if (m_ppSound[i]) m_ppSound[i]->release();
			}
		}
	}

	//메모리 지운다
	SAFE_DELETE_ARRAY(m_ppChannel);
	SAFE_DELETE_ARRAY(m_ppSound);

	if (m_ppSystem != NULL)
	{
		m_ppSystem->release();
		m_ppSystem->close();
	}

	m_mapTotalSounds.clear();
}
