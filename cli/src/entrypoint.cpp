// Ŭnicode please 
#include "stdafx.h"
#include "entrypoint.h"
#include "util/audio_manager.h"
#include "irr/debug_drawer.h"
#include "irr/debug_draw_manager.h"
#include "irr/head_tracker.h"
#include "irr/hmd_event_receiver.h"
#include "irr/leapmotion.h"
#include "base/lib.h"
#include "console/console_function.h"
#include "util/cvar_key.h"

// sequence
#include "game/sequence.h"
#include "game/sequence_factory.h"

#include "util/fps_counter.h"
#include "util/event_receiver_manager.h"
#include "console/irr_console.h"
#include "console/console_event_receiver.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;

int entrypoint(int argc, char* argv[])
{	
	//detect memory leak
	//_CrtSetBreakAlloc(132879);

	//var 관련 내용 로딩을 미리 해놓는다
	startUpCVar();

	//게임에서 사용할 최조 시퀀스를 선택한다
	//단, 생성까지 처리해버리면 시망한다(생성은 엔진객체가 필요하기 때문)
	//그렇다고 시퀀스 선택을 엔진생성 다음으로 넘기면
	//엔진 GUI쪽으로 포커스가 넘어가버리니까 귀찮아서 시퀀스 번호만 먼저 고르게함
	SequenceFactory seqFactory;
	DisplayType displayType = kDisplayDefault;
	SequenceType seqType = seqFactory.select(&displayType);

	EngineParam engineParam;
	if(displayType == kDisplayFullscreen) {
		engineParam.fullscreen = 1;
	} else if(displayType == kDisplayWindow) {
		engineParam.fullscreen = 0;
	}

	bool initResult = Lib::startUp(engineParam);
	if(initResult == false) {
		return 1;
	}
	
	IrrlichtDevice *device = Lib::device;
	IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	ISceneManager* smgr = device->getSceneManager();

	Lib::sequence = std::move(seqFactory.create(seqType));

	DebugDrawer debugDrawer;
	FPSCounter fpsCounter;

	// Render loop
	irr::ITimer *timer = device->getTimer();
	irr::u32 then = timer->getTime();
	while(device->run()) {
		irr::u32 now = timer->getTime();
		irr::u32 frameDeltaTime = now - then;
		then = now;

		// Read-Write Head Tracking Sensor Value to Camera
		Lib::headTracker->update();
		if(Lib::headTracker->isConnected()) {
			SHeadTrackingEvent evt = Lib::headTracker->getValue();
			Lib::eventReceiver->OnEvent(evt);
		}

		Lib::audio->update();

#ifdef USE_LEAP_MOTION
		if(Lib::leapController->isConnected()) {
			Lib::eventReceiver->OnEvent(Lib::leapListener->getEvent());
		}
#endif

		Lib::sequence->update(frameDeltaTime);
		{
			driver->beginScene(true, true, Lib::backgroundColor);
			Lib::sequence->draw();
			
			// debug render. 실제 렌더링 이후에 그려야됨
			debugDrawer.drawAll(*g_debugDrawMgr);
			guienv->drawAll();
			g_console->RenderConsole(guienv, driver, frameDeltaTime);

			driver->endScene();	//render end
		}

		//debug draw mgr의 업데이트를 나중에 처리해야 1프레임만 렌더링되는 객체도
		//제대로 렌더링된다
		g_debugDrawMgr->updateAll(frameDeltaTime);
		
		// fps는 다음프레임에 보여주면 될듯
		fpsCounter.draw();
	}
	Lib::shutDown();
	
	device->drop();	
	shutDownCVar();
	return 0;
}