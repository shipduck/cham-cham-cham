// Ŭnicode please 
#include "stdafx.h"
#include "game_scene.h"
#include "scene_helper.h"

using namespace irr;
using namespace video;
using namespace gui;
using namespace core;
using namespace scene;

GameScene::GameScene(irr::IrrlichtDevice *dev)
	: Scene(dev), hmdCam(nullptr), camNode(nullptr)
{
}

GameScene::~GameScene()
{
}

void GameScene::setUp()
{
	Scene::setUp();

	Device->setWindowCaption(L"Game Scene");

	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();

	guienv->addStaticText(L"Project::Dokuro", rect<s32>(10,10,260,22), true);

	initSky();
	initCam();

	//좌표 테스트용 객체를 적절히 배치
	SMaterial material;
	material.Lighting = false;
	material.AmbientColor = SColor(255, 255, 0, 0);

	for(float x = -1000 ; x <= 1000 ; x += 100) {
		for(float z = -1000 ; z <= 1000 ; z += 100) {
			auto node = smgr->addCubeSceneNode();
			node->setPosition(vector3df(x, 0, z));
			node->getMaterial(0) = material;
		}
	}

}

void GameScene::initCam()
{
	ISceneManager* smgr = Device->getSceneManager();

	// Create Camera
	camNode = smgr->addCameraSceneNode();
	auto cursorControl = Device->getCursorControl();
	SceneNodeAnimatorCameraHMD* hmdCam = new SceneNodeAnimatorCameraHMD(Device, cursorControl, 0.3f, 0.05f, 0.0f);

	camNode->addAnimator(hmdCam);
	//TODO 카메라 위치 잡기
	camNode->setPosition(core::vector3df(0, 0, 0));
	camNode->setTarget(core::vector3df(0, 0, 1));
	camNode->setFarValue(1000.0f);
	auto camAnim = *camNode->getAnimators().begin();
	hmdCam = static_cast<SceneNodeAnimatorCameraHMD*>(camAnim);
}

void GameScene::initSky()
{
	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();

	// create skybox and skydome
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, false);
	scene::ISceneNode* skybox = smgr->addSkyBoxSceneNode(
		driver->getTexture("ext/irrlicht/media/irrlicht2_up.jpg"),
		driver->getTexture("ext/irrlicht/media/irrlicht2_dn.jpg"),
		driver->getTexture("ext/irrlicht/media/irrlicht2_lf.jpg"),
		driver->getTexture("ext/irrlicht/media/irrlicht2_rt.jpg"),
		driver->getTexture("ext/irrlicht/media/irrlicht2_ft.jpg"),
		driver->getTexture("ext/irrlicht/media/irrlicht2_bk.jpg"));
	//scene::ISceneNode* skydome=smgr->addSkyDomeSceneNode(driver->getTexture("ext/irrlicht/media/skydome.jpg"),16,8,0.95f,2.0f);
	driver->setTextureCreationFlag(video::ETCF_CREATE_MIP_MAPS, true);
}


void GameScene::shutDown()
{
	Scene::shutDown();

	//delete(hmdCam);
	//hmdCam = nullptr;
}
void GameScene::draw()
{
}

void GameScene::update(int ms)
{
	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();
}
