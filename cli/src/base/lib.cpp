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

using namespace std;

const int MAX_STRING_CHARS = 4096;

AudioManager audioManagerLocal;
AudioManager *Lib::audio = &audioManagerLocal;

HeadTracker headTrackerLocal;
HeadTracker *Lib::headTracker = &headTrackerLocal;

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

void initConsoleFunction()
{
	CVarUtils::CreateCVar("driver_info", console::driverInfo, "Display Irrlicht Driver Info");	
	CVarUtils::CreateCVar("play_bgm", console::playBGM, "Play test bgm");
}
void initConsoleVar()
{
}

void Lib::startUp(irr::IrrlichtDevice *dev)
{
	device = dev;
	driver = device->getVideoDriver();
	guienv = device->getGUIEnvironment();
	smgr = device->getSceneManager();

	//모든 초기화 중에서 콘솔이 가장 우선
	console = g_console;
	setUpConsole(device);
	initConsoleVar();
	initConsoleFunction();

	//이벤트 시스템을 엔진에 장착
	//이것이 제대로 달라붙어야 일리히트에서 발생한 로그 이벤트가 콘솔에 뜬다
	eventReceiver->startUp(device);
	auto joystickDev = eventReceiver->getJoystickDev();
	joystickDev.showInfo();
	//엔진에 바로 붙은 이벤트 리스너는 이거로 강제함
	SR_ASSERT(device->getEventReceiver() == nullptr);
	device->setEventReceiver(eventReceiver);

	// 이벤트 잡는게 콘솔 이벤트 잡는거는 필수요소
	eventReceiver->addReceiver(new ConsoleEventReceiver(), 0);

	// f2로 hmd모드 전환하는거. 나중에 분리 가능
	hmdEventReceiver = new HMDEventReceiver();
	eventReceiver->addReceiver(hmdEventReceiver, 0);

	//Oculus Rift Head Tracking
	headTracker->startUp();

	//set oculus renderer
	hmdDescriptorBind.reset(new HMDDescriptorBind());
	HMDDescriptor descriptor = hmdDescriptorBind->convert();
	stereoRenderer = new HMDStereoRender(device, descriptor, 10);

	// 12/14 폰트는 얻는 함수를 따로 만들어놧었다. 그거 쓰면 코드 중복을 제거 가능
	// 로딩이 느리니까 일단 꺼놓고 필요해지면 살리기
	//getNormalFont12();
	//getNormalFont14();

	//사운든는 우선순위 가장 낮음
	audio->startUp();
}

void Lib::shutDown()
{
	delete(stereoRenderer);
	audio->shutDown();
	headTracker->shutDown();
	eventReceiver->shutDown();
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
