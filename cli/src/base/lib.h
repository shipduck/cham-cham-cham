// Ŭnicode please 
#pragma once

class IrrConsole;
class AudioManager;
class HeadTracker;
class EventReceiverManager;
class HMDEventReceiver;
class HMDStereoRender;
class KeyMapping;
class Sequence;

/*
전역변수로 존재하는 대부분의 항목에 직접 접근하는것 괜찮겟지만
그거는 상황에 따라서 적절한 헤더 찾고 함수 호출하기 귀찮을거같아서
미리 연결시켜놧다. 클래스로 선언되었지만 사실상 namespace 처럼 사용
idlib/idLib.h에서 베낀 컨셉
*/

class EngineParam {
public:
	EngineParam();

	int &fullscreen;
	int &screenWidth;
	int &screenHeight;
	int &showFps;
};

class Lib {
public:
	static IrrConsole *console;

	//console function
	static void printf(const char *fmt, ...);
	static void printf(const std::string &text);
	static void error(const char *fmt, ...);
	static void error(const std::string &text);
	static void warning(const char *fmt, ...);
	static void warning(const std::string &text);

	static bool startUp(const EngineParam &param);
	static void shutDown();

	// irrlicht
	static irr::IrrlichtDevice *device;
	static irr::video::IVideoDriver *driver;
	static irr::gui::IGUIEnvironment *guienv;
	static irr::scene::ISceneManager *smgr;

	static AudioManager *audio;

	//device관련 내용은 class Device같은 형태로 분리 가능성 존재
	//근데 귀찮은 관계로 당분간은 냅두자
	static HeadTracker *headTracker;

	//이벤트 잡아서 통제하는것도 전역변수같은 느낌으로 1개 필요
	//엔진에 붙여놓고 사용하는거
	static EventReceiverManager *eventReceiver;
	static HMDEventReceiver *hmdEventReceiver;

	// oculus stereo renderer는 시스템상에 단 1개만 존재해야한다
	static HMDStereoRender *stereoRenderer;
	static void updateStereoRenderer();

	//임시로 때려박은 렌더링 기본색
	//콘솔에서 바꾸게 하는게 있을필요가 있나?
	static irr::video::SColor backgroundColor;

	static KeyMapping *keyMapping;

	//현재 실행중인 시퀀스. 외부에서 접근해서 바꿀수 있어야해서 추가
	//일단은 여기에 넣지만 나중에 SequenceManager같은클래스 하나 추가해서 작업하는거 검토하기
	static std::unique_ptr<Sequence> sequence;
};