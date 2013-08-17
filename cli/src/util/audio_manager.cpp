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

void AudioManager::update()
{
	std::vector<std::string> endList;
	for(auto bgm : bgmDict_) {
		if(bgm.second.isPlaying() == false) {
			endList.push_back(bgm.first);
			bgm.second.pause();
			bgm.second.close();
		}
	}
	
	for(auto name : endList) {
		bgmDict_.erase(name);
	}
}

void AudioManager::add(const std::string &name, const BaseSound &bgm)
{
	auto found = bgmDict_.find(name);
	SR_ASSERT(found == bgmDict_.end() && "name conflict");
	bgmDict_[name] = bgm;
}

void BaseSound::startUp(const std::string &file)
{
	this->file_ = file;
}

bool BaseSound::open(bool loop)
{
	if((buf_ = alutCreateBufferFromFile(this->file_.c_str())) == AL_NONE) {
        puts(alutGetErrorString(alutGetError()));
		return false;
    }
    alGenSources(1, &src_);
    alSourcei(src_, AL_BUFFER, buf_);
	if(loop) {
		alSourcei(src_, AL_LOOPING, AL_TRUE);
	}
	return true;
}

bool BaseSound::close()
{
	if(buf_ == 0) {
		return false;
	}

	alDeleteSources(1, &src_);
	alDeleteBuffers(1, &buf_);
	src_ = 0;
	buf_ = 0;
	return true;
}
void BaseSound::play()
{
	if(src_ != 0) {
		alSourcePlay(src_);
	}
}
void BaseSound::pause()
{
	if (src_ != 0) {
        alSourcePause(src_);
    }
}
bool BaseSound::isPlaying() const
{
	ALint state;
	alGetSourcei(src_, AL_SOURCE_STATE, &state);
    if (state == AL_PLAYING) {
        return true;
	} else {
		return false;
	}
}