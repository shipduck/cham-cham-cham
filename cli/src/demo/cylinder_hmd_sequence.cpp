// Ŭnicode please 
#include "stdafx.h"
#include "cylinder_hmd_sequence.h"
#include "irr/cylinder_mapping_node.h"
#include "base/lib.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

CylinderButtonNode *btnNode = nullptr;

const char *playBtnFileName = "res/menu/play_btn.png";
const char *playBtnSelectFileName = "res/menu/play_btn_select.png";

CylinderHMDSequence::CylinderHMDSequence()
{
	// add fps camera
	auto camNode = Lib::smgr->addCameraSceneNodeFPS();

	auto node = Lib::smgr->addEmptySceneNode(camNode);
	{
		auto tex = Lib::driver->getTexture("res/texture/sora2.png");
		//auto cylinderNode = new hmd_ui::CylinderMappingNode(Lib::smgr->getRootSceneNode(), Lib::smgr, 124);
		auto cylinderNode = new CylinderTextureNode(node, Lib::smgr, 0, tex);
		cylinderNode->setPosition(core::vector3df(0, 5, 0));
		auto anim = Lib::smgr->createRotationAnimator(core::vector3df(0, 0.4f,0));
		cylinderNode->addAnimator(anim);
		anim->drop();
		cylinderNode->radius = 30.0f;
		cylinderNode->rebuild();
		cylinderNode->drop();
	}
	
	{
		auto tex = Lib::driver->getTexture("res/texture/sora2.png");
		//auto cylinderNode = new hmd_ui::CylinderMappingNode(Lib::smgr->getRootSceneNode(), Lib::smgr, 124);
		auto cylinderNode = new CylinderTextureNode(node, Lib::smgr, 0, tex);
		cylinderNode->setPosition(core::vector3df(0, 5, 0));
		cylinderNode->setRotation(core::vector3df(0, 30, 0));
		cylinderNode->radius = 30.0f;
		cylinderNode->rebuild();
		cylinderNode->drop();
	}
	{
		//button
		btnNode = new CylinderButtonNode(node, Lib::smgr, 0, playBtnFileName, playBtnSelectFileName);
		btnNode->setPosition(core::vector3df(0, -5, 0));
		btnNode->setRotation(core::vector3df(0, 30, 0));
		btnNode->radius = 20.0f;
		btnNode->rebuild();
		btnNode->drop();
	}
}
CylinderHMDSequence::~CylinderHMDSequence()
{
}

void CylinderHMDSequence::update(int ms)
{
	static int elapsedTime = 0;
	elapsedTime += ms;
	if((elapsedTime / 1000) % 2 == 0) {
		btnNode->selected = false;
		btnNode->rebuild();
	} else {
		btnNode->selected = true;
		btnNode->rebuild();
	}
}