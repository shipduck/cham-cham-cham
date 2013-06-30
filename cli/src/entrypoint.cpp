// Ŭnicode please 
#include "stdafx.h"
#include "entrypoint.h"
#include "util/debug_draw_manager.h"
#include "util/audio_manager.h"
#include "util/head_tracking.h"

// scene
#include "game/debug_draw_scene.h"
#include "game/game_scene.h"

// console
#include "irrConsole/console.h"

using namespace irr;
using namespace core;
using namespace scene;
using namespace video;
using namespace io;
using namespace gui;


// Configuration
int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 800;
irr::gui::IGUIFont* captionFont = nullptr;
array<irr::core::stringw> captionText;
IC_Console console;
u32 captionHeight = 0;

void drawCaptions(irr::IrrlichtDevice *device)
{
	static const irr::video::SColor fontColor(200,200,200,200);
	dimension2d<u32> screenDim = device->getVideoDriver()->getScreenSize();
	rect<s32> lineRect(10,10,screenDim.Width, screenDim.Height);
	for(u32 i = 0; i < captionText.size(); i++)
	{
		captionFont->draw(captionText[i].c_str(),lineRect,fontColor);
		lineRect.UpperLeftCorner.Y += captionHeight;
		lineRect.LowerRightCorner.Y += captionHeight;
	}
}

class MyEventReceiver : public irr::IEventReceiver {
public:
	bool OnEvent(const SEvent& event)
	{
		if(event.EventType == irr::EET_KEY_INPUT_EVENT ) {
			if(event.KeyInput.PressedDown) {
				if(event.KeyInput.Key == irr::KEY_ESCAPE) {
					if(console.isVisible()) {
						console.setVisible(false);
						return true;
					} else {
						//프로그램 끄기
						//setRunning(false);
						return true;
					}
				} else if(event.KeyInput.Key == IC_Console::IC_KEY_TILDE) {
					if(!console.isVisible()) {
						console.setVisible(true);
						return true;
					} else if(!event.KeyInput.Control) {
						console.setVisible(false);
						return true;
					}
				} if(console.isVisible()) {
					console.handleKeyPress(event.KeyInput.Char, event.KeyInput.Key,event.KeyInput.Shift, event.KeyInput.Control);
					return true;
				}
			}
		} else if(event.EventType == irr::EET_LOG_TEXT_EVENT) {
			console.logMessage_ANSI(event.LogEvent.Level,event.LogEvent.Text);
			return true;
		} else if(event.EventType == irr::EET_MOUSE_INPUT_EVENT) {
			return console.isVisible();
		}
		return false;
	}

};





int entrypoint(int argc, char* argv[])
{
	bool fullscreen = false;
	bool stencil = true;
	bool vsync = true;

	// Check fullscreen
	for (int i=1;i<argc;i++) fullscreen |= !strcmp("-f", argv[i]);
	//fullscreen = true;
	
	MyEventReceiver receiver;
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT), 32, fullscreen, stencil, vsync, &receiver);
	if (!device){ 
		return 1;
	}
	
	IVideoDriver* driver = device->getVideoDriver();
	IGUIEnvironment* guienv = device->getGUIEnvironment();
	ISceneManager* smgr = device->getSceneManager();

	//set debug tool
	gDebugDrawMgr->setUp(device);
	//gNormalFont12 = guienv->getFont("res/font_12.xml");
	gNormalFont14 = guienv->getFont("res/font_14.xml");

	//load the caption font
	captionFont = gNormalFont14;

	//initialize the caption array
	captionText.push_back(L"IrrConsole : Quake Style Drop Down Console Demo For Irrlicht");
	captionText.push_back(L"Author : Saurav Mohapatra (mohaps@gmail.com)");
	captionText.push_back(L"");
	captionText.push_back(L"HELP TEXT:");
	captionText.push_back(L"============================================================");
	captionText.push_back(L"     Press the tilde (~) key to toggle console");
	captionText.push_back(L"     Use the UP/DN arrow keys to access command history");
	captionText.push_back(L"     Type commands at the prompt");
	captionText.push_back(L"     To execute a command put \\ in front");
	captionText.push_back(L"     e.g. \\list");
	captionText.push_back(L"     use the \"list\" or \"help\" commands to find out more");
	captionText.push_back(L"     to start try the command \"\\help show_node\");");
	captionText.push_back(L"============================================================");
	captionText.push_back(L" ");
	captionText.push_back(L"TO QUIT THIS DEMO PRESS ESCAPE KEY");

	//initialize the caption line height
	captionHeight = gNormalFont14->getDimension(L"X").Height + 2;
	//initialize the console
	//this is how you alter some of the config params
	console.getConfig().dimensionRatios.Y = 0.8f;
	console.getConfig().fontName = "res/font_14.xml";
	//now initialize
	console.initializeConsole(device->getGUIEnvironment(), dimension2d<s32>(SCREEN_WIDTH, SCREEN_HEIGHT));

	//register common commands
	console.loadDefaultCommands(device);

	//simple scene framework
	std::unique_ptr<Scene> scene(new DebugDrawScene(device));
	//std::unique_ptr<Scene> scene(new GameScene(device));
	scene->setUp();

	//Oculus Rift Head Tracking
	HeadTracking headTracking;
	headTracking.startUp();

    
	gAudioMgr->setUp();
	if(gAudioMgr->isSupport()) {
		const std::string bg("res/sound/bg.wav");
		BGM bgm;
		bgm.setUp(bg);
		bgm.open();
		bgm.play();
		gAudioMgr->addBGM("bg", bgm);
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
		float yaw = 0;
		float pitch = 0;
		float roll = 0;
		if(headTracking.isConnected()) {
			headTracking.getValue(&yaw, &pitch, &roll);			
		}
		//hmdCam->setHeadTrackingValue(yaw, pitch, roll);

		scene->update(frameDeltaTime);

		{
			driver->beginScene(true, true, video::SColor(255, 10, 10, 10));
			scene->draw();

			// debug render
			gDebugDrawMgr->drawAll();
			guienv->drawAll();	

			console.renderConsole(guienv, driver, frameDeltaTime);
			if(!console.isVisible()) {
				drawCaptions(device);
			}

			driver->endScene();	//render end
		}

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
    
	gAudioMgr->shutDown();

	device->drop();	
	
	//Deinitialize Oculus LibOVR
	headTracking.shutDown();
	
	return 0;
}
