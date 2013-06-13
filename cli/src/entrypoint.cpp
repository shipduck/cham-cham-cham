// Ŭnicode please 
#include "stdafx.h"
#include "entrypoint.h"
#include "head_tracking.h"
#include "sample_scene.h"

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
float mouseSpeed = 40.0f;
float walkSpeed = 3.0f;





int entrypoint(int argc, char* argv[])
{
	// Check fullscreen
	for (int i=1;i<argc;i++) fullscreen |= !strcmp("-f", argv[i]);
	//fullscreen = true;
		
	IrrlichtDevice *device = createDevice(EDT_OPENGL, dimension2d<u32>(SCREEN_WIDTH, SCREEN_HEIGHT), 32, fullscreen, false, vsync);
	if (!device) {
		return 1;
	}
	
	IVideoDriver* driver = device->getVideoDriver();

	//Oculus Rift Head Tracking
	CHeadTracking headTracking;
	headTracking.startUp();

	//simple scene framework
	std::unique_ptr<Scene> scene(new SampleScene(device));
	scene->setUp();

	int lastFPS = -1;
	// Render loop
	while(device->run()) {
		// Read-Write Head Tracking Sensor Value to Camera
		float yaw = 0;
		float pitch = 0;
		float roll = 0;
		if(headTracking.isConnected()) {
			headTracking.getValue(&yaw, &pitch, &roll);			
		}
		//hmdCam->setHeadTrackingValue(yaw, pitch, roll);

		scene->update(16);
		

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

	device->drop();	
	
	//Deinitialize Oculus LibOVR
	headTracking.shutDown();
	
	return 0;
}
