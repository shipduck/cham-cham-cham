// Ŭnicode please 
#include "stdafx.h"
#include "scene.h"
#include "util/debug_draw_manager.h"
#include "util/HMDStereoRender.h"

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
	HMD.hScreenSize = 0.14976;
	HMD.vScreenSize = 0.0936;
	HMD.interpupillaryDistance = 0.064;
	HMD.lensSeparationDistance = 0.064;
	HMD.eyeToScreenDistance = 0.041;
	HMD.distortionK[0] = 1.0;
	HMD.distortionK[1] = 0.22;
	HMD.distortionK[2] = 0.24;
	HMD.distortionK[3] = 0.0;

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
	drawAllNormal(smgr);
	//drawAllStereo(smgr);
	//smgr->drawAll();
}