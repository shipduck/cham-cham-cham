// Ŭnicode please 
#include "stdafx.h"
#include "audio_manager.h"

AudioManager audioManagerLocal;
AudioManager *g_audioMgr = &audioManagerLocal;

AudioManager::AudioManager()
	: SupportAL(false)
{
}
AudioManager::~AudioManager()
{
}
void AudioManager::startUp()
{
	ALboolean useSound = alutInit(NULL, nullptr);
	if(useSound == false) {
        puts(alutGetErrorString(alutGetError()));
        puts("Failed to init sound");
    } else {
		SupportAL = true;
	}
}
void AudioManager::shutDown()
{
	if(isSupport() == false) {
		return;
	}
	auto it = BgmDict.begin();
	auto endit = BgmDict.end();
	for( ; it != endit ; ++it) {
		it->second.pause();
		it->second.close();
	}
    alutExit();
	SupportAL = false;
}

void AudioManager::addBGM(const std::string &name, const BGM &bgm)
{
	auto found = BgmDict.find(name);
	SR_ASSERT(found == BgmDict.end() && "name conflict");
	BgmDict[name] = bgm;
}

void BGM::startUp(const std::string &file)
{
	this->File = file;
}

bool BGM::open()
{
	if((BgmBuf = alutCreateBufferFromFile(this->File.c_str())) == AL_NONE) {
        puts(alutGetErrorString(alutGetError()));
		return false;
    }
    alGenSources(1, &BgmSrc);
    alSourcei(BgmSrc, AL_BUFFER, BgmBuf);
	return true;
}

bool BGM::close()
{
	if(BgmBuf == 0) {
		return false;
	}

	alDeleteSources(1, &BgmSrc);
	alDeleteBuffers(1, &BgmBuf);
	BgmSrc = 0;
	BgmBuf = 0;
	return true;
}
void BGM::play()
{
	if(BgmSrc != 0) {
		alSourcePlay(BgmSrc);
	}
}
void BGM::pause()
{
	if (BgmSrc != 0) {
        alSourcePause(BgmSrc);
    }
}
