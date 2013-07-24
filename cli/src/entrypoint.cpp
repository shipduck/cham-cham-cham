// Ŭnicode please 
#include "stdafx.h"
#include "entrypoint.h"
#include "util/debug_draw_manager.h"
#include "util/audio_manager.h"

#include "irr/head_tracker.h"
#include "irr/hmd_event_receiver.h"

// scene
#include "game/debug_draw_scene.h"
#include "game/game_scene.h"

#include "util/event_receiver_manager.h"
#include "game/game_console.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


// Configuration
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 800;

int mainStartUp(irr::IrrlichtDevice *device)
{
	IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	ISceneManager* smgr = device->getSceneManager();

	g_eventReceiverMgr->startUp(device);

	auto joystickDev = g_eventReceiverMgr->getJoystickDev();
	joystickDev.showInfo();

	//set debug tool
	g_debugDrawMgr->startUp(device);
	
	//initialize the console
	//g_normalFont12 = guienv->getFont("res/font_12.xml");
	g_normalFont14 = guienv->getFont("res/font_14.xml");
	setUpConsole(device);

	g_audioMgr->startUp();

	//Oculus Rift Head Tracking
	g_headTracker->startUp();

	return 0;
}

int mainShutDown()
{
	g_debugDrawMgr->shutDown();
	g_eventReceiverMgr->shutDown();
	g_audioMgr->shutDown();
	g_headTracker->shutDown();
	return 0;
}

int entrypoint(int argc, char* argv[])
{
	bool fullscreen = false;
	bool stencil = true;
	bool vsync = true;

	// Check fullscreen
	for (int i=1;i<argc;i++) fullscreen |= !strcmp("-f", argv[i]);
	//fullscreen = true;
	
	g_eventReceiverMgr->addReceiver(new ConsoleEventReceiver(), 0);
	g_hmdEventReceiver = static_cast<HMDEventReceiver*>(g_eventReceiverMgr->addReceiver(new HMDEventReceiver(), 0));
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT), 32, fullscreen, stencil, vsync, g_eventReceiverMgr);
	if (!device){
		return 1;
	}
	mainStartUp(device);
	
	IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	ISceneManager* smgr = device->getSceneManager();

	//simple scene framework
	//std::unique_ptr<Scene> scene(new DebugDrawScene(device));
	std::unique_ptr<Scene> scene(new GameScene(device));
	scene->startUp();

	

	if(g_audioMgr->isSupport()) {
		const std::string bg("res/sound/bg.wav");
		BGM bgm;
		bgm.startUp(bg);
		bgm.open();
		bgm.play();
		g_audioMgr->addBGM("bg", bgm);
	}

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
		if(g_headTracker->isConnected()) {
			SHeadTrackingEvent evt = g_headTracker->getValue();
			g_eventReceiverMgr->OnEvent(evt);
		}

		scene->update(frameDeltaTime);
		{
			driver->beginScene(true, true, video::SColor(255, 10, 10, 10));
			scene->draw();

			// debug render
			g_debugDrawMgr->drawAll();
			guienv->drawAll();	

			g_console.renderConsole(guienv, driver, frameDeltaTime);

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
	scene->shutDown();

	mainShutDown();
	
	device->drop();	

	return 0;
}
