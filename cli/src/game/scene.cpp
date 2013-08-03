// Ŭnicode please 
#include "stdafx.h"
#include "scene.h"
#include "base/lib.h"
#include "irr/debug_draw_manager.h"
#include "irr/hmd_stereo_render.h"
#include "irr/hmd_event_receiver.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace gui;

HMDDescriptorBind hmdDescriptorBind;

Scene::Scene()
{
	//hmd 렌더링 지원
	HMDDescriptor descriptor = hmdDescriptorBind.convert();
	renderer_.reset(new HMDStereoRender(Lib::device, descriptor, 10));
}
Scene::~Scene()
{
	renderer_.reset(nullptr);
	Lib::guienv->clear();
}

void Scene::drawAllNormal(irr::scene::ISceneManager *smgr)
{
	int w = Lib::driver->getScreenSize().Width;
	int h = Lib::driver->getScreenSize().Height;
	Lib::driver->setViewPort(core::recti(0, 0, w, h));
	Lib::smgr->drawAll();
}

void Scene::drawAllStereo(irr::scene::ISceneManager *smgr)
{
	renderer_->drawAll(smgr);
}

void Scene::draw()
{	
	// draw the 3d scene
	if(Lib::hmdEventReceiver->isSupportHMD()) {
		HMDDescriptor descriptor = hmdDescriptorBind.convert();
		renderer_->setHMD(descriptor);
		drawAllStereo(Lib::smgr);
	} else {
		drawAllNormal(Lib::smgr);
	}
	//smgr->drawAll();
}