// Ŭnicode please 
#pragma once

class BGM;
class AudioManager;

class AudioManager {
public:
	typedef std::map<std::string, BGM> BgmDictType;
public:
	AudioManager();
	~AudioManager();

	void setUp();
	void shutDown();

	bool isSupport() { return SupportAL; }

	void addBGM(const std::string &name, const BGM &bgm);

private:
	bool SupportAL;
	BgmDictType BgmDict;
};

//stl 안에 들어가는 클래스이므로 생성자/소멸자에 로직이 있으면 망한다
class BGM {
public:
	BGM() : BgmBuf(0), BgmSrc(0) {}
	~BGM() {}
	
	void setUp(const std::string &file);

	bool open();
	bool close();
	void play();
	void pause();


private:
	std::string File;
	ALuint BgmBuf;
	ALuint BgmSrc;
};

extern AudioManager *gAudioMgr;