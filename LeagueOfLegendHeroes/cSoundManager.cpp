#include "StdAfx.h"
#include "cSoundManager.h"

cSoundManager::cSoundManager(void)
{
	System_Create(&m_ppSystem);

	//ä�μ� ����
	m_ppSystem->init(TOTALSOUNDBUFFER, FMOD_INIT_NORMAL, 0);

	//ä�μ��� ���� ������ �����ش�!!
	m_ppSound = new Sound*[TOTALSOUNDBUFFER];
	m_ppChannel = new Channel*[TOTALSOUNDBUFFER];

	//�ʱ�ȭ
	ZeroMemory(m_ppSound, sizeof(Sound*) * (TOTALSOUNDBUFFER));
	ZeroMemory(m_ppChannel, sizeof(Channel*) * (TOTALSOUNDBUFFER));
}

cSoundManager::~cSoundManager(void)
{
}


//flac, mp3, wave, midi, ogg, m4a, aac, aif, aiff, wma
void cSoundManager::AddSound(string keyName, string soundName, bool background, bool loop)
{
	//���R�� �����?
	if (loop)
	{
		//bgm!!
		if (background)
		{
			//��������϶�...
			m_ppSystem->createStream(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &m_ppSound[m_mapTotalSounds.size()]);
		}
		else
		{
			//�Ϲ� �����....
			m_ppSystem->createSound(soundName.c_str(), FMOD_LOOP_NORMAL, 0, &m_ppSound[m_mapTotalSounds.size()]);
		}
	}
	else
	{
		m_ppSystem->createSound(soundName.c_str(), FMOD_DEFAULT, 0, &m_ppSound[m_mapTotalSounds.size()]);
	}

	//�ʿ� ���带 Ű���� �Բ� �־��ش�
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
			//�÷���
			m_ppSystem->playSound(FMOD_CHANNEL_FREE, *iter->second, false, &m_ppChannel[count]);
			m_ppChannel[count]->setVolume(volume);
			break;
		}
	}
}

//�Ͻ� ����
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

//�Ͻ� ������ ���� �ٽ� ���
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

//�Ͻ� ���� �Ǿ��°�?
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

//���� ������̳�?
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

//fmod �ý��� ����...
void cSoundManager::Update(void)
{
	m_ppSystem->update();
}

//������ ���� �� �����´� (�и��겫 : 1000 == 1��)
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

//������ �������� �����Ѵ�
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

//������� ������ ���� ��ġ�� �����´�
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

	//�޸� �����
	SAFE_DELETE_ARRAY(m_ppChannel);
	SAFE_DELETE_ARRAY(m_ppSound);

	if (m_ppSystem != NULL)
	{
		m_ppSystem->release();
		m_ppSystem->close();
	}

	m_mapTotalSounds.clear();
}
