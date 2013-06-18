// Ŭnicode please 
#include "stdafx.h"
#include "entrypoint.h"
#include "util/debug_draw_manager.h"
#include "util/head_tracking.h"
#include "util/hmd_display.h"

// scene
#include "game/sample_scene.h"
#include "game/debug_draw_scene.h"
#include "game/game_scene.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


// Configuration
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 800;
bool fullscreen = false;
bool vsync = true;

int entrypoint(int argc, char* argv[])
{
	// Check fullscreen
	for (int i=1;i<argc;i++) fullscreen |= !strcmp("-f", argv[i]);
	//fullscreen = true;
	
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT), 32, fullscreen, false, vsync);
	if (!device){ 
		return 1;
	}
	
	IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	ISceneManager* smgr = device->getSceneManager();

	HMDDisplay display;
	display.setUp(device);

	//Oculus Rift Head Tracking
	HeadTracking headTracking;
	headTracking.startUp();

	//set debug tool
	gDebugDrawMgr->setUp(device);
	gNormalFont12 = guienv->getFont("res/font_12.xml");
	gNormalFont14 = guienv->getFont("res/font_14.xml");

	//simple scene framework
	std::unique_ptr<Scene> scene(new SampleScene(device));
	//std::unique_ptr<Scene> scene(new DebugDrawScene(device));
	//std::unique_ptr<Scene> scene(new GameScene(device));
	scene->setUp();

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
		float yaw = 0;
		float pitch = 0;
		float roll = 0;
		if(headTracking.isConnected()) {
			headTracking.getValue(&yaw, &pitch, &roll);			
		}
		//hmdCam->setHeadTrackingValue(yaw, pitch, roll);

		scene->update(frameDeltaTime);

		// draw block
		driver->beginScene(true, true, video::SColor(255,113,113,133));

		// draw the 3d scene
		display.NormalDrawAll(smgr);
		//display.StereoDrawAll(smgr);
		
		scene->draw();

		// debug render
		gDebugDrawMgr->drawAll();
		guienv->drawAll();
		driver->endScene();	//render end

		//debug draw mgr의 업데이트를 나중에 처리해야 1프레임만 렌더링되는 객체도
		//제대로 렌더링된다
		gDebugDrawMgr->updateAll(frameDeltaTime);
		
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

	//clean debug tool
	gDebugDrawMgr->shutDown();

	device->drop();	
	
	//Deinitialize Oculus LibOVR
	headTracking.shutDown();
	
	return 0;
}
