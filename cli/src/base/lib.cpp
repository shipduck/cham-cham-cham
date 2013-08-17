// Ŭnicode please 
#include "stdafx.h"
#include "lib.h"
#include "template_lib.h"
#include "console/irr_console.h"
#include "console/console_event_receiver.h"
#include "irr/debug_drawer.h"
#include "irr/head_tracker.h"
#include "irr/hmd_stereo_render.h"
#include "irr/hmd_event_receiver.h"
#include "util/audio_manager.h"
#include "util/event_receiver_manager.h"
#include "util/console_func.h"
#include "util/cvar_key.h"
#include "game/key_mapping.h"
#include "game/sequence.h"
#include "irr/leapmotion.h"

using namespace std;
using namespace irr;

const int MAX_STRING_CHARS = 4096;

AudioManager audioManagerLocal;
AudioManager *Lib::audio = &audioManagerLocal;

HeadTracker headTrackerLocal;
HeadTracker *Lib::headTracker = &headTrackerLocal;

Leap::Controller leapControllerLocal;
Leap::Controller *Lib::leapController = &leapControllerLocal;

IrrLeapListener leapListenerLocal;
IrrLeapListener *Lib::leapListener = &leapListenerLocal;

EventReceiverManager eventReceiverMgrLocal;
EventReceiverManager *Lib::eventReceiver = &eventReceiverMgrLocal;

irr::IrrlichtDevice *Lib::device = nullptr;
irr::video::IVideoDriver *Lib::driver = nullptr;
irr::gui::IGUIEnvironment *Lib::guienv = nullptr;
irr::scene::ISceneManager *Lib::smgr = nullptr;

// 콘솔은 g_console기반으로 만들어놓은게 많아서 신경끄자
IrrConsole *Lib::console = nullptr;

// hmd 이벤트는 포인터만 갖고있으면 된다
HMDEventReceiver *Lib::hmdEventReceiver = nullptr;

std::unique_ptr<HMDDescriptorBind> hmdDescriptorBind;
HMDStereoRender *Lib::stereoRenderer = nullptr;

//내부에서 CVar로 변수 연결하는게 AttachCVar로 되어있어서
//CVar저장하기 전까지 keyMappingLocal이 뒤지면 안된다
//게다가 key mapping은 싱글턴
//그래서 전역변수로 선언해서 포인터만 연결해놧다
KeyMapping keyMappingLocal;
KeyMapping *Lib::keyMapping = &keyMappingLocal;

irr::video::SColor Lib::backgroundColor = video::SColor(255, 10, 10, 10);

std::unique_ptr<Sequence> Lib::sequence;

// 게임에서 사용할 특수 기능을 그래픽카드에서 지원하는지 미리 검증하는 함수
// 최초에 미리 검증하면 실제 게임작동중에는 기능을 지원하는지 신경쓰지 않고 막써도 된다
bool checkDeviceSupport(irr::IrrlichtDevice *device)
{
	auto driver = device->getVideoDriver();

	//렌더타켓이 없으면 오큘러스 지원은 하늘로
	map<video::E_VIDEO_DRIVER_FEATURE, string> featureMap;
	featureMap[video::EVDF_RENDER_TO_TARGET] = "Render to Target not support";
	featureMap[video::EVDF_ARB_GLSL] = "GLSL not support";
	featureMap[video::EVDF_TEXTURE_NPOT] = "Texture npot not support";
	featureMap[video::EVDF_TEXTURE_NSQUARE] = "non-square textures supported";
	for(auto x : featureMap) {
		if(driver->queryFeature(x.first) == false) {
			fprintf(stderr, "%s\n", x.second.c_str());
			return false;
		}
	}

	return true;
}

EngineParam::EngineParam()
	: fullscreen(CVarUtils::GetCVarRef<int>(CVAR_ENGINE_DISPLAY_FULLSCREEN)),
	screenWidth(CVarUtils::GetCVarRef<int>(CVAR_ENGINE_DISPLAY_WIDTH)),
	screenHeight(CVarUtils::GetCVarRef<int>(CVAR_ENGINE_DISPLAY_HEIGHT)),
	showFps(CVarUtils::GetCVarRef<int>(CVAR_ENGINE_DISPLAY_SHOW_FPS))
{
}

bool Lib::startUp(const EngineParam &param)
{
	//init c library
	srand(time(NULL));

	bool stencil = true;
	bool vsync = true;

	device = createDevice(
		video::EDT_OPENGL, 
		core::dimension2d<u32>(param.screenWidth, param.screenHeight), 
		32, 
		(param.fullscreen > 0), stencil, vsync, nullptr);
	if(device == nullptr) {
		return false;
	}

	driver = device->getVideoDriver();
	guienv = device->getGUIEnvironment();
	smgr = device->getSceneManager();

	bool deviceSupport = checkDeviceSupport(device);
	SR_ASSERT(deviceSupport == true);

	//모든 초기화 중에서 콘솔이 가장 우선
	console = g_console;
	setUpConsole(device);

	//key mapping 로딩. 키매핑은 1개로 충분하니까 싱글턴
	keyMapping->startUp();

	//이벤트 시스템을 엔진에 장착
	//이것이 제대로 달라붙어야 일리히트에서 발생한 로그 이벤트가 콘솔에 뜬다
	eventReceiver->startUp(device);
	auto joystickDev = eventReceiver->getJoystickDev();
	joystickDev.showInfo();
	//엔진에 바로 붙은 이벤트 리스너는 이거로 강제함
	SR_ASSERT(device->getEventReceiver() == nullptr);
	device->setEventReceiver(eventReceiver);

	// 이벤트 잡는게 콘솔 이벤트 잡는거는 필수요소
	eventReceiver->attachReceiver(new ConsoleEventReceiver(), 0);

	// f2로 hmd모드 전환하는거. 나중에 분리 가능
	hmdEventReceiver = new HMDEventReceiver();
	eventReceiver->attachReceiver(hmdEventReceiver, 0);

	//Oculus Rift Head Tracking. stereo render보다 먼저 초기화 되어야한다
	headTracker->startUp();

	//set oculus renderer
	hmdDescriptorBind.reset(new HMDDescriptorBind());
	HMDDescriptor descriptor = hmdDescriptorBind->convert();
	stereoRenderer = new HMDStereoRender(device, descriptor, 10);

	//Leapmotion Initialize
	leapController->addListener(*leapListener);
	leapController->enableGesture(Leap::Gesture::TYPE_SWIPE);

	// 12/14 폰트는 얻는 함수를 따로 만들어놧었다. 그거 쓰면 코드 중복을 제거 가능
	// 로딩이 느리니까 일단 꺼놓고 필요해지면 살리기
	//getNormalFont12();
	//getNormalFont14();

	//사운든는 우선순위 가장 낮음
	audio->startUp();

	return true;
}

void Lib::shutDown()
{
	sequence.reset(nullptr);
	safeDelete(stereoRenderer);
	keyMapping->shutDown();
	audio->shutDown();
	headTracker->shutDown();
	eventReceiver->shutDown();
	leapController->removeListener(*leapListener);
}

void Lib::updateStereoRenderer()
{
	HMDDescriptor descriptor = hmdDescriptorBind->convert();
	stereoRenderer->setHMD(descriptor);
}

void Lib::printf(const char *fmt, ...)
{
	SR_ASSERT(console != nullptr);
	va_list argptr;
	char text[MAX_STRING_CHARS];
	va_start( argptr, fmt );
	vsnprintf(text, sizeof( text ), fmt, argptr);
	va_end( argptr );

	console->EnterLogLine(text, LINEPROP_DEBUG);
}
void Lib::error(const char *fmt, ...)
{
	SR_ASSERT(console != nullptr);
	va_list argptr;
	char text[MAX_STRING_CHARS];
	va_start( argptr, fmt );
	vsnprintf(text, sizeof( text ), fmt, argptr);
	va_end( argptr );

	console->EnterLogLine(text, LINEPROP_ERROR);
}
void Lib::warning(const char *fmt, ...)
{
	SR_ASSERT(console != nullptr);
	va_list argptr;
	char text[MAX_STRING_CHARS];
	va_start( argptr, fmt );
	vsnprintf(text, sizeof( text ), fmt, argptr);
	va_end( argptr );

	console->EnterLogLine(text, LINEPROP_WARNING);
}

void Lib::printf(const std::string &text)
{
	console->EnterLogLine(text, LINEPROP_DEBUG);
}

void Lib::error(const std::string &text)
{
	console->EnterLogLine(text, LINEPROP_ERROR);
}

void Lib::warning(const std::string &text)
{
	console->EnterLogLine(text, LINEPROP_WARNING);
}
