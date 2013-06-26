// Ŭnicode please 
#include "stdafx.h"
#include "game/debug_draw_scene.h"
#include "util/debug_draw_manager.h"

using namespace std;
using namespace irr;
using namespace scene;
using namespace video;
using namespace gui;
using namespace core;

SColor red(255, 255, 0, 0);
SColor green(255, 0, 255, 0);
SColor blue(255, 0, 0, 255);
SColor white(255, 255, 255, 255);

DebugDrawScene::DebugDrawScene(irr::IrrlichtDevice *dev)
	: Scene(dev)
{
}
DebugDrawScene::~DebugDrawScene()
{
}

void DebugDrawScene::setUp()
{
	Scene::setUp();

	Device->setWindowCaption(L"Irrlicht Engine Debug Draw");

	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();

	smgr->addCameraSceneNodeFPS();
	Device->getCursorControl()->setVisible(false);

	//duration based는 아직
	//gDebugDrawMgr->addSphere(core::vector3df(30,0,30), 5, green, 5000);
}

void DebugDrawScene::shutDown()
{
	Scene::shutDown();
}
void DebugDrawScene::update(int ms)
{
	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();

	std::wstring msg(L"hello world");

	gDebugDrawMgr->addSphere(core::vector3df(0,0,30), 10, red);
	gDebugDrawMgr->addLine(vector3df(10, 0, 30), vector3df(10, 200, 200), green, 1.0f);
	gDebugDrawMgr->addLine(vector3df(10, 0, 30), vector3df(10, 200, 300), red, 2.0f);
	matrix4 axisMat = core::matrix4();
	axisMat.setTranslation(vector3df(-20, 0, 30));
	axisMat.setRotationDegrees(vector3df(0, 0, 45));

	gDebugDrawMgr->addAxis(axisMat, 5);
	gDebugDrawMgr->addCross(vector3df(-10, 0, 30), blue, 5);
	gDebugDrawMgr->addString(vector3df(0, 0, 30), msg, blue, 2.0f);

	gDebugDrawMgr->addLine(vector2di(0, 0), vector2di(50, 100), blue, 5.0f);
	gDebugDrawMgr->addCross(vector2di(50, 100), red, 5.0f);
	gDebugDrawMgr->addCircle(vector2di(50, 100), 10.0f, green);
	gDebugDrawMgr->addString(vector2di(50, 100), msg, white);
}

void DebugDrawScene::draw()
{
}