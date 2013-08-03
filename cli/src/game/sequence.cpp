// Ŭnicode please 
#include "stdafx.h"
#include "sequence.h"
#include "base/lib.h"
#include "irr/debug_draw_manager.h"
#include "irr/hmd_stereo_render.h"
#include "irr/hmd_event_receiver.h"

using namespace irr;
using namespace video;
using namespace scene;
using namespace gui;

HMDDescriptorBind hmdDescriptorBind;

Sequence::Sequence()
{
	//hmd 렌더링 지원
	HMDDescriptor descriptor = hmdDescriptorBind.convert();
	renderer_.reset(new HMDStereoRender(Lib::device, descriptor, 10));
}
Sequence::~Sequence()
{
	renderer_.reset(nullptr);
	Lib::guienv->clear();
}

void Sequence::drawAllNormal(irr::scene::ISceneManager *smgr)
{
	int w = Lib::driver->getScreenSize().Width;
	int h = Lib::driver->getScreenSize().Height;
	Lib::driver->setViewPort(core::recti(0, 0, w, h));
	Lib::smgr->drawAll();
}

void Sequence::drawAllStereo(irr::scene::ISceneManager *smgr)
{
	renderer_->drawAll(smgr);
}

void Sequence::draw()
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