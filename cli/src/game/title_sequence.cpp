// Ŭnicode please 
#include "stdafx.h"
#include "title_sequence.h"
#include "base/lib.h"

#include "irr/hmd_gui.h"

using namespace irr;
using namespace core;
using namespace video;
using namespace scene;

//TODO: 리소스를 직접 파일명이 아니라 key-real file로 연결하는 중간단계가 필요할지도?
const char *titleFileName = "res/menu/title.png";
const char *playBtnFileName = "res/menu/play_btn.png";
const char *playBtnSelectFileName = "res/menu/play_btn_select.png";
const char *quitBtnFileName = "res/menu/quit_btn.png";
const char *quitBtnSelectFileName = "res/menu/quit_btn.png";
const char *gameImgFileName = "res/menu/starship_troopers_3-_marauder_2.jpg";

scene::ISceneNode *test = nullptr;
scene::ICameraSceneNode* fpsCamera = nullptr;
hmd_ui::CylinderButtonNode *btnNode = nullptr;

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

	auto node = Lib::smgr->addEmptySceneNode(camNode);
	{
		auto tex = Lib::driver->getTexture("res/texture/sora2.png");
		//auto cylinderNode = new hmd_ui::CylinderMappingNode(Lib::smgr->getRootSceneNode(), Lib::smgr, 124);
		auto cylinderNode = new hmd_ui::CylinderTextureNode(node, Lib::smgr, 0, tex);
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
		auto cylinderNode = new hmd_ui::CylinderTextureNode(node, Lib::smgr, 0, tex);
		cylinderNode->setPosition(core::vector3df(0, 5, 0));
		cylinderNode->setRotation(core::vector3df(0, 30, 0));
		cylinderNode->radius = 30.0f;
		cylinderNode->rebuild();
		cylinderNode->drop();
	}
	{
		//button
		btnNode = new hmd_ui::CylinderButtonNode(node, Lib::smgr, 0, playBtnFileName, playBtnSelectFileName);
		btnNode->setPosition(core::vector3df(0, -5, 0));
		btnNode->setRotation(core::vector3df(0, 30, 0));
		btnNode->radius = 20.0f;
		btnNode->rebuild();
		btnNode->drop();
	}
}

TitleSequence::~TitleSequence()
{
	uiMgr.shutDown();
}

void TitleSequence::update(int ms)
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

void TitleSequence::preDraw()
{
	uiMgr.drawAll();
}