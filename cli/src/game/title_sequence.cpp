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
const char *quitBtnFileName = "res/menu/quit_btn.png";
const char *gameImgFileName = "res/menu/starship_troopers_3-_marauder_2.jpg";

scene::ISceneNode *test = nullptr;
scene::ICameraSceneNode* fpsCamera = nullptr;

hmd_ui::Manager uiMgr;

TitleSequence::TitleSequence()
{
	// 스프라이트 로딩. 하지만 오큘러스 대응 타이틀로 만들기 위해서 gui를 사용하지 않고 3d환경으로 구현한다
	ITexture *titleTex = Lib::driver->getTexture(titleFileName);
	ITexture *playBtnTex = Lib::driver->getTexture(playBtnFileName);
	ITexture *quitBtnTex = Lib::driver->getTexture(quitBtnFileName);
	ITexture *imgTex = Lib::driver->getTexture(gameImgFileName);

	IBillboardSceneNode *bill = nullptr;

	auto comp = new hmd_ui::TestComp();
	uiMgr.add(comp);

	// add fps camera
	auto camNode = Lib::smgr->addCameraSceneNodeFPS();

	// create test cube
	test = Lib::smgr->addCubeSceneNode(60);

	// let the cube rotate and set some light settings
	scene::ISceneNodeAnimator* anim = Lib::smgr->createRotationAnimator(core::vector3df(0.3f, 0.3f,0));

	test->setPosition(core::vector3df(-30, 0, 100));
	test->setMaterialFlag(video::EMF_LIGHTING, false); // disable dynamic lighting
	test->addAnimator(anim);
	anim->drop();
	test->setMaterialTexture(0, comp->getTexture()); // set material of cube to render target

	{
		//auto cylinderNode = new hmd_ui::CylinderMappingNode(Lib::smgr->getRootSceneNode(), Lib::smgr, 124);
		auto cylinderNode = new hmd_ui::CylinderMappingNode(camNode, Lib::smgr, 0);
		cylinderNode->setPosition(core::vector3df(0, 5, 0));
		anim = Lib::smgr->createRotationAnimator(core::vector3df(0, 1.0f,0));
		cylinderNode->addAnimator(anim);
		anim->drop();
		auto tex = Lib::driver->getTexture("res/texture/sora2.png");
		cylinderNode->radius = 30.0f;
		cylinderNode->scale = 0.02f;
		cylinderNode->setTexture(tex);
	}
	{
		//auto cylinderNode = new hmd_ui::CylinderMappingNode(Lib::smgr->getRootSceneNode(), Lib::smgr, 124);
		auto cylinderNode = new hmd_ui::CylinderMappingNode(camNode, Lib::smgr, 0);
		cylinderNode->setPosition(core::vector3df(0, 5, 0));
		cylinderNode->setRotation(core::vector3df(0, 20, 0));
		cylinderNode->addAnimator(anim);
		anim->drop();
		auto tex = Lib::driver->getTexture("res/texture/sora2.png");
		cylinderNode->radius = 30.0f;
		cylinderNode->scale = 0.02f;
		cylinderNode->setTexture(tex);
	}


	

	/*
	//rt
	test = smgr->addCubeSceneNode(60);
	auto anim = Lib::smgr->createRotationAnimator(core::vector3df(0.3f, 0.3f, 0));
	test->setPosition(vector3df(0, 0, -100));
	test->setMaterialFlag(video::EMF_LIGHTING, false);
	test->addAnimator(anim);
	anim->drop();

	rt_ = Lib::driver->addRenderTargetTexture(core::dimension2d<u32>(256, 256), "RTT1");
	test->setMaterialTexture(0, rt_);

	fixedCam = Lib::smgr->addCameraSceneNode(0, vector3df(10, 10, -80), vector3df(-10, 10, -100));
	*/
	/*
	{
		float scale = 0.1f;
		bill = smgr->addBillboardSceneNode();
		bill->setMaterialTexture(0, titleTex);
		bill->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);
		bill->setMaterialFlag(video::EMF_LIGHTING, false);
		bill->setMaterialFlag(video::EMF_ZBUFFER, false);	
		bill->setPosition(vector3df(0, 0, 100));
		auto titleSize = titleTex->getSize();
		bill->setSize(dimension2df(titleSize.Width * scale, titleSize.Height * scale));
	}
	{
		float scale = 0.1f;
		bill = smgr->addBillboardSceneNode();
		//bill->setMaterialType(video::EMT_TRANSPARENT_ALPHA_CHANNEL);

		SMaterial &mat = bill->getMaterial(0);
		mat.ColorMaterial = video::ECM_NONE;
		//mat.MaterialType = video::EMT_TRANSPARENT_MODUL;
		mat.MaterialTypeParam = 0.01f;
		mat.DiffuseColor.set(128, 255, 255, 0);

		bill->setMaterialTexture(0, playBtnTex);
		bill->setMaterialFlag(video::EMF_LIGHTING, false);
		bill->setMaterialFlag(video::EMF_ZBUFFER, false);	
		bill->setPosition(vector3df(100, 0, 100));
		auto size = playBtnTex->getSize();
		bill->setSize(dimension2df(size.Width * scale, size.Height * scale));	
	}
	*/

	//
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