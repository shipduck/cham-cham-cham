// Ŭnicode please 
#include "stdafx.h"
#include "scene.h"
#include "base/lib.h"
#include "irr/debug_draw_manager.h"

#include "irr/HMDStereoRender.h"
#include "irr/hmd_event_receiver.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace gui;

Scene::Scene(irr::IrrlichtDevice *dev)
	: device_(dev), 
	rootScene_(nullptr)
{
	auto smgr = device_->getSceneManager();
	rootScene_ = smgr->addEmptySceneNode();

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

	renderer_.reset(new HMDStereoRender(device_, HMD, 10));
}
Scene::~Scene()
{
	renderer_.reset(nullptr);
	SR_ASSERT(rootScene_ != nullptr);
	rootScene_->remove();
	rootScene_ = nullptr;

	auto guienv = device_->getGUIEnvironment();
	guienv->clear();
}

void Scene::drawAllNormal(irr::scene::ISceneManager *smgr)
{
	IVideoDriver* driver = device_->getVideoDriver();
	int w = driver->getScreenSize().Width;
	int h = driver->getScreenSize().Height;
	driver->setViewPort(core::recti(0, 0, w, h));
	smgr->drawAll();
}

void Scene::drawAllStereo(irr::scene::ISceneManager *smgr)
{
	renderer_->drawAll(smgr);
}

void Scene::draw()
{
	IVideoDriver* driver = device_->getVideoDriver();
	IGUIEnvironment* guienv = device_->getGUIEnvironment();
	ISceneManager* smgr = device_->getSceneManager();
	
	// draw the 3d scene
	if(Lib::hmdEventReceiver->isSupportHMD()) {
		drawAllStereo(smgr);
	} else {
		drawAllNormal(smgr);
	}
	//smgr->drawAll();
}