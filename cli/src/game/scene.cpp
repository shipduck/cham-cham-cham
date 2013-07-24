// Ŭnicode please 
#include "stdafx.h"
#include "scene.h"
#include "util/debug_draw_manager.h"

#include "irr/HMDStereoRender.h"
#include "irr/hmd_event_receiver.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace gui;

Scene::Scene(irr::IrrlichtDevice *dev)
	: Device(dev)
{
}
Scene::~Scene()
{
}

void Scene::setUp()
{
	auto guienv = Device->getGUIEnvironment();
	auto smgr = Device->getSceneManager();
	smgr->clear();
	guienv->clear();

	//hmd 렌더링 지원
	HMDDescriptor HMD;
	// Parameters from the Oculus Rift DK1
	HMD.hResolution = 1280;
	HMD.vResolution = 800;
	HMD.hScreenSize = 0.14976f;
	HMD.vScreenSize = 0.0936f;
	HMD.interpupillaryDistance = 0.064f;
	HMD.lensSeparationDistance = 0.064f;
	HMD.eyeToScreenDistance = 0.041f;
	HMD.distortionK[0] = 1.0f;
	HMD.distortionK[1] = 0.22f;
	HMD.distortionK[2] = 0.24f;
	HMD.distortionK[3] = 0.0f;

	Renderer.reset(new HMDStereoRender(Device, HMD, 10));
}

void Scene::shutDown()
{
	Renderer.reset(nullptr);

	auto guienv = Device->getGUIEnvironment();
	auto smgr = Device->getSceneManager();
	smgr->clear();
	guienv->clear();
}

void Scene::drawAllNormal(irr::scene::ISceneManager *smgr)
{
	IVideoDriver* driver = Device->getVideoDriver();
	int w = driver->getScreenSize().Width;
	int h = driver->getScreenSize().Height;
	driver->setViewPort(core::recti(0, 0, w, h));
	smgr->drawAll();
}

void Scene::drawAllStereo(irr::scene::ISceneManager *smgr)
{
	Renderer->drawAll(smgr);
}

void Scene::draw()
{
	IVideoDriver* driver = Device->getVideoDriver();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();
	ISceneManager* smgr = Device->getSceneManager();
	
	// draw the 3d scene
	if(gHMDEventReceiver->isSupportHMD()) {
		drawAllStereo(smgr);
	} else {
		drawAllNormal(smgr);
	}
	//smgr->drawAll();
}