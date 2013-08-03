// Ŭnicode please 
#include "stdafx.h"
#include "audio_manager.h"

AudioManager::AudioManager()
	: supportAL_(false)
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
		supportAL_ = true;
	}
}
void AudioManager::shutDown()
{
	if(isSupport() == false) {
		return;
	}
	auto it = bgmDict_.begin();
	auto endit = bgmDict_.end();
	for( ; it != endit ; ++it) {
		it->second.pause();
		it->second.close();
	}
    alutExit();
	supportAL_ = false;
}

void AudioManager::addBGM(const std::string &name, const BGM &bgm)
{
	auto found = bgmDict_.find(name);
	SR_ASSERT(found == bgmDict_.end() && "name conflict");
	bgmDict_[name] = bgm;
}

void BGM::startUp(const std::string &file)
{
	this->file_ = file;
}

bool BGM::open()
{
	if((bgmBuf_ = alutCreateBufferFromFile(this->file_.c_str())) == AL_NONE) {
        puts(alutGetErrorString(alutGetError()));
		return false;
    }
    alGenSources(1, &bgmSrc_);
    alSourcei(bgmSrc_, AL_BUFFER, bgmBuf_);
	return true;
}

bool BGM::close()
{
	if(bgmBuf_ == 0) {
		return false;
	}

	alDeleteSources(1, &bgmSrc_);
	alDeleteBuffers(1, &bgmBuf_);
	bgmSrc_ = 0;
	bgmBuf_ = 0;
	return true;
}
void BGM::play()
{
	if(bgmSrc_ != 0) {
		alSourcePlay(bgmSrc_);
	}
}
void BGM::pause()
{
	if (bgmSrc_ != 0) {
        alSourcePause(bgmSrc_);
    }
}
