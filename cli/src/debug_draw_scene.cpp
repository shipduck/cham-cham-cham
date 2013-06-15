// Ŭnicode please 
#include "stdafx.h"
#include "debug_draw_scene.h"
#include "debug_draw_manager.h"

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
	: Scene(dev), 
	DebugDrawMgr2d(new DebugDrawManager2D()),
	DebugDrawMgr3d(new DebugDrawManager3D())
{
}
DebugDrawScene::~DebugDrawScene()
{
}

void DebugDrawScene::setUp()
{
	Scene::setUp();

	DebugDrawMgr2d->setUp(Device);
	DebugDrawMgr3d->setUp(Device);

	Device->setWindowCaption(L"Irrlicht Engine Debug Draw");

	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();

	DebugDrawMgr3d->addSphere(core::vector3df(30,0,30), 5, green, 5000);

	auto node = smgr->addTextSceneNode(guienv->getBuiltInFont(), L"ASDF", blue);
	node->setPosition(core::vector3df(0, 10,30));
	node->setScale(core::vector3df(1000, 1000, 1000));

	smgr->addCameraSceneNodeFPS();
	//Device->getCursorControl()->setVisible(false);
}

void DebugDrawScene::shutDown()
{
	DebugDrawMgr2d->shutDown();
	DebugDrawMgr3d->shutDown();

	Scene::shutDown();
}
void DebugDrawScene::update(int ms)
{
	IVideoDriver* driver = Device->getVideoDriver();
	ISceneManager* smgr = Device->getSceneManager();
	IGUIEnvironment* guienv = Device->getGUIEnvironment();

	std::wstring msg(L"hello world");

	DebugDrawMgr3d->addSphere(core::vector3df(0,0,30), 10, red);
	DebugDrawMgr3d->addLine(vector3df(10, 0, 30), vector3df(10, 200, 200), green, 2.0f);
	matrix4 axisMat = core::matrix4();
	axisMat.setTranslation(vector3df(-20, 0, 30));
	//axisMat.setRotationDegrees(vector3df(0, 0, 45));
	DebugDrawMgr3d->addAxis(axisMat, 5);
	DebugDrawMgr3d->addCross(vector3df(-10, 0, 30), blue, 5);

	DebugDrawMgr2d->addLine(vector2di(0, 0), vector2di(50, 100), blue, 5.0f);
	DebugDrawMgr2d->addCross(vector2di(50, 100), red, 5.0f);
	DebugDrawMgr2d->addString(vector2di(50, 100), msg, white, 2.0f);

	driver->beginScene(true, true, video::SColor(255,113,113,133));

	smgr->drawAll(); // draw the 3d scene

	// debug render
	DebugDrawer2D drawer2d(Device);
	DebugDrawer3D drawer3d(Device);
	drawer2d.drawAll(*DebugDrawMgr2d);
	drawer3d.drawAll(*DebugDrawMgr3d);

	//guienv->drawAll();

	driver->endScene();	//render end

	DebugDrawMgr2d->update(ms);
	DebugDrawMgr3d->update(ms);
}

