// Ŭnicode please 
#include "stdafx.h"
#include "title_sequence.h"
#include "base/lib.h"

#include "irr/hmd_gui.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

scene::ISceneNode *test = nullptr;
scene::ICameraSceneNode* fpsCamera = nullptr;
hmd_ui::Manager uiMgr;

TitleSequence::TitleSequence()
{
	auto comp = new hmd_ui::TestComp();
	uiMgr.add(comp);

	// add fps camera
	auto camNode = Lib::smgr->addCameraSceneNodeFPS();

	{
		// create test cube
		test = Lib::smgr->addCubeSceneNode(60);

		// let the cube rotate and set some light settings
		scene::ISceneNodeAnimator* anim = Lib::smgr->createRotationAnimator(core::vector3df(0.3f, 0.3f,0));
		test->setPosition(core::vector3df(-30, 0, 100));
		test->setMaterialFlag(video::EMF_LIGHTING, false); // disable dynamic lighting
		test->addAnimator(anim);
		anim->drop();
		test->setMaterialTexture(0, comp->getTexture()); // set material of cube to render target
	}
}

TitleSequence::~TitleSequence()
{
	uiMgr.shutDown();
}

void TitleSequence::update(int ms)
{
	
}

void TitleSequence::preDraw()
{
	uiMgr.drawAll();
}