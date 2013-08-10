// Ŭnicode please 
#include "stdafx.h"
#include "cylinder_hmd_sequence.h"
#include "irr/cylinder_mapping_node.h"
#include "base/lib.h"
#include "demo_sequence_helper.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

CylinderButtonNode *btnNode = nullptr;
scene::ISceneNode *node = nullptr;

CylinderHMDSequence::CylinderHMDSequence()
{
	DemoSequenceHelper helper(this);
	helper.loadSampleCollisionMap();
	Lib::device->getCursorControl()->setVisible(false);
	auto camNode = Lib::smgr->getActiveCamera();
	SR_ASSERT(camNode != nullptr);

	node = Lib::smgr->addEmptySceneNode();
	{
		auto tex = Lib::driver->getTexture(res::texture::SORA2_PNG.c_str());
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
		auto tex = Lib::driver->getTexture(res::texture::SORA2_PNG.c_str());
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
		btnNode = new CylinderButtonNode(node, Lib::smgr, 0, res::menu::PLAY_BTN_PNG.c_str(), res::menu::PLAY_BTN_SELECT_PNG.c_str());
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
	//HMD 테스트 목적이기 일단은 카메라와 HMD는 좌표만 동기화 시키면된다
	//회전까지는 동기화 하지 않아도 될듯. 근데 카메라 좌표계 변동 로직을 확실히 하지 않아서 밀리는 느낌
	auto camNode = Lib::smgr->getActiveCamera();
	SR_ASSERT(camNode != nullptr);
	node->setPosition(camNode->getPosition());

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