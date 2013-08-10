// Ŭnicode please 
#include "stdafx.h"
#include "entrypoint.h"
#include "util/audio_manager.h"
#include "irr/debug_drawer.h"
#include "irr/debug_draw_manager.h"
#include "irr/head_tracker.h"
#include "irr/hmd_event_receiver.h"
#include "base/lib.h"
#include "console/console_function.h"

// sequence
#include "game/sequence.h"
#include "game/sequence_factory.h"

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
	//_CrtSetBreakAlloc(27254);

	//게임에서 사용할 최조 시퀀스를 선택한다
	//단, 생성까지 처리해버리면 시망한다(생성은 엔진객체가 필요하기 때문)
	//그렇다고 시퀀스 선택을 엔진생성 다음으로 넘기면
	//엔진 GUI쪽으로 포커스가 넘어가버리니까 귀찮아서 시퀀스 번호만 먼저 고르게함
	SequenceFactory seqFactory;
	bool forceFullscreen = false;
	SequenceType seqType = seqFactory.select(&forceFullscreen);

	EngineParam engineParam;
	if(forceFullscreen) {
		engineParam.fullscreen = 1;
	}

	bool initResult = Lib::startUp(engineParam);
	if(initResult == false) {
		return 1;
	}
	
	IrrlichtDevice *device = Lib::device;
	IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	ISceneManager* smgr = device->getSceneManager();

	
	std::unique_ptr<Sequence> sequence = seqFactory.create(seqType);

	DebugDrawer debugDrawer;

	int lastFPS = -1;
	// 첫번째 프레임 가동에서 시간이 오래 걸릴수 있으니까
	// 씬 변환이후 첫번쨰 프레임은 dt=0으로 생각하고 돌린다
	bool isFirstRun = true;
	u32 then = device->getTimer()->getTime();
	// Render loop
	while(device->run()) {
		int frameDeltaTime = 0;
		if(isFirstRun) {
			isFirstRun = false;
			then = device->getTimer()->getTime();
			frameDeltaTime = 0;
		} else {
			// Work out a frame delta time.
			const u32 now = device->getTimer()->getTime();
			frameDeltaTime = now - then;	// Time in milliseconds
			then = now;
		}

		// Read-Write Head Tracking Sensor Value to Camera
		Lib::headTracker->update();
		if(Lib::headTracker->isConnected()) {
			SHeadTrackingEvent evt = Lib::headTracker->getValue();
			Lib::eventReceiver->OnEvent(evt);
		}

		sequence->update(frameDeltaTime);
		{
			driver->beginScene(true, true, video::SColor(255, 10, 10, 10));
			sequence->draw();
			
			// debug render. 실제 렌더링 이후에 그려야됨
			debugDrawer.drawAll(*g_debugDrawMgr);
			guienv->drawAll();
			g_console->RenderConsole(guienv, driver, frameDeltaTime);

			driver->endScene();	//render end
		}

		//debug draw mgr의 업데이트를 나중에 처리해야 1프레임만 렌더링되는 객체도
		//제대로 렌더링된다
		g_debugDrawMgr->updateAll(frameDeltaTime);
		
		int fps = driver->getFPS();
		if (lastFPS != fps) {
			core::stringw str = L"Collision detection example - Irrlicht Engine [";
			str += driver->getName();
			str += "] FPS:";
			str += fps;

			device->setWindowCaption(str.c_str());
			lastFPS = fps;
		}
	}
	//shut down scene before device drop!
	sequence.reset(nullptr);

	Lib::shutDown();
	
	device->drop();	

	return 0;
}
