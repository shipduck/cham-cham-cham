// Ŭnicode please 
#include "stdafx.h"
#include "hmd_gui.h"
#include "base/lib.h"

using namespace irr;
using namespace std;

namespace hmd_ui {;
Manager::Manager()
{
}

Manager::~Manager()
{
	shutDown();
}
Component *Manager::add(Component *comp)
{
	compList_.push_back(comp);
	return comp;
}
void Manager::drawAll()
{
	for(auto x : compList_) {
		x->bind();
		x->draw();
	}
	Component::unbind();
}

void Manager::shutDown()
{
	for(auto x : compList_) {
		delete(x);
	}
	compList_.clear();
}

Component::Component(int w, int h, const std::string &name)
	: smgr_(Lib::smgr->createNewSceneManager(false)),
	rtSize_(w, h),
	rt_(Lib::driver->addRenderTargetTexture(core::dimension2d<u32>(w, h), name.c_str())),
	bgColor_(0, 0, 0, 255)
{
	SR_ASSERT(w > 0);
	SR_ASSERT(h > 0);
}
Component::~Component()
{
	if(smgr_ != nullptr) {
		smgr_->drop();
		smgr_ = nullptr;
	}

	Lib::driver->removeTexture(rt_);
	rt_ = nullptr;
}

void Component::draw()
{
	smgr_->drawAll();
}
void Component::bind()
{
	// set render target texture
	Lib::driver->setRenderTarget(rt_, true, true, bgColor_);
}
void Component::unbind()
{
	Lib::driver->setRenderTarget(0, true, true, 0);
}


TestComp::TestComp()
	: Component(512, 512, "RTT1")
{
	this->setBackgroundColor(video::SColor(0, 0, 128, 128));

	const char *md2file = "ext/irrlicht/media/faerie.md2";
	const char *texfile = "ext/irrlicht/media/faerie2.bmp";
	scene::IAnimatedMeshSceneNode* fairy = getSceneMgr()->addAnimatedMeshSceneNode(
		Lib::smgr->getMesh(md2file));
	
	SR_ASSERT(fairy != nullptr);
	fairy->setMaterialTexture(0, Lib::driver->getTexture(texfile)); // set diffuse texture
	fairy->setMaterialFlag(video::EMF_LIGHTING, true); // enable dynamic lighting
	fairy->getMaterial(0).Shininess = 20.0f; // set size of specular highlights
	fairy->setPosition(core::vector3df(-10,0,-100));
	fairy->setMD2Animation(scene::EMAT_STAND);

	// add white light
	getSceneMgr()->addLightSceneNode(0, core::vector3df(-15,5,-105), video::SColorf(1.0f, 1.0f, 1.0f));

	// set ambient light
	getSceneMgr()->setAmbientLight(video::SColor(0,60,60,60));

	// add fixed camera
	getSceneMgr()->addCameraSceneNode(0, core::vector3df(10,10,-80), core::vector3df(-10,10,-100));
}

TestComp::~TestComp()
{
}

}	// namespace hmd_ui