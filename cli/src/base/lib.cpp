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

	//preload font
	const char *fontList[] = {
		//"res/font_14.xml",
		//"res/font_12.xml",	//12는 작아서 안씀
		"res/console-14.xml",
	};
	const int fontListSize = getArraySize(fontList);
	for(int i = 0 ; i < fontListSize ; ++i) {
		const char *fontName = fontList[i];
		guienv->getFont(fontName);
	}

	console = g_console;
	setUpConsole(device);
	initConsoleVar();
	initConsoleFunction();
	
	audio->startUp();

	//Oculus Rift Head Tracking
	headTracker->startUp();

	eventReceiver->startUp(device);
	auto joystickDev = eventReceiver->getJoystickDev();
	joystickDev.showInfo();
	//엔진에 바로 붙은 이벤트 리스너는 이거로 강제함
	SR_ASSERT(device->getEventReceiver() == nullptr);
	device->setEventReceiver(eventReceiver);

	// 이벤트 잡는게 콘솔 이벤트 잡는거는 필수요소
	eventReceiver->addReceiver(new ConsoleEventReceiver(), 0);

	hmdEventReceiver = new HMDEventReceiver();
	eventReceiver->addReceiver(hmdEventReceiver, 0);

	//set oculus renderer
	hmdDescriptorBind.reset(new HMDDescriptorBind());
	HMDDescriptor descriptor = hmdDescriptorBind->convert();
	stereoRenderer = new HMDStereoRender(device, descriptor, 10);
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
