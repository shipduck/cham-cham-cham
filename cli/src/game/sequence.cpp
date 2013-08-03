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

Sequence::Sequence()
{
}
Sequence::~Sequence()
{
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
	Lib::updateStereoRenderer();
	Lib::stereoRenderer->drawAll(smgr);
}

void Sequence::draw()
{	
	// draw the 3d scene
	if(Lib::hmdEventReceiver->isSupportHMD()) {
		drawAllStereo(Lib::smgr);
	} else {
		drawAllNormal(Lib::smgr);
	}
	//smgr->drawAll();
}