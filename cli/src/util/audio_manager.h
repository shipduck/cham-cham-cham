// Ŭnicode please 
#pragma once

class BaseSound;
class AudioManager;

class AudioManager {
public:
	typedef std::map<std::string, BaseSound> BgmDictType;
public:
	AudioManager();
	~AudioManager();

	void startUp();
	void shutDown();
	void update();

	bool isSupport() { return supportAL_; }

	void add(const std::string &name, const BaseSound &bgm);

private:
	bool supportAL_;
	BgmDictType bgmDict_;
};

//stl 안에 들어가는 클래스이므로 생성자/소멸자에 로직이 있으면 망한다
class BaseSound {
public:
	BaseSound() : buf_(0), src_(0) {}
	virtual ~BaseSound() {}

	void startUp(const std::string &file);
	bool open(bool loop=false);
	bool close();
	void play();
	void pause();
	bool isPlaying() const;
	
private:
	std::string file_;
	ALuint buf_;
	ALuint src_;
};
