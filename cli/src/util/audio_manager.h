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

	void startUp();
	void shutDown();

	bool isSupport() { return supportAL_; }

	void addBGM(const std::string &name, const BGM &bgm);

private:
	bool supportAL_;
	BgmDictType bgmDict_;
};

//stl 안에 들어가는 클래스이므로 생성자/소멸자에 로직이 있으면 망한다
class BGM {
public:
	BGM() : bgmBuf_(0), bgmSrc_(0) {}
	~BGM() {}
	
	void startUp(const std::string &file);

	bool open();
	bool close();
	void play();
	void pause();


private:
	std::string file_;
	ALuint bgmBuf_;
	ALuint bgmSrc_;
};