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

	bool initResult = Lib::startUp();
	if(initResult == false) {
		return 1;
	}
	
	IrrlichtDevice *device = Lib::device;
	IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	ISceneManager* smgr = device->getSceneManager();

	//simple scene framework
	SequenceFactory seqFactory;
	std::unique_ptr<Sequence> sequence = seqFactory.create(kSequenceTitle);
	//std::unique_ptr<Sequence> sequence = seqFactory.create(kSequenceDebugDraw);

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
