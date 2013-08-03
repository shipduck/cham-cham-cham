// Ŭnicode please 
#include "stdafx.h"
#include "base/lib.h"
#include "game/debug_draw_scene.h"
#include "irr/debug_draw_manager.h"

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

DebugDrawScene::DebugDrawScene()
{
	Lib::device->setWindowCaption(L"Irrlicht Engine Debug Draw");

	Lib::smgr->addCameraSceneNodeFPS();
	Lib::device->getCursorControl()->setVisible(false);

	g_debugDrawMgr->addSphere(core::vector3df(30,0,30), 5, green, 5000);
	g_debugDrawMgr->addSphere(core::vector3df(30,0,30), 2, red, 2000);
	g_debugDrawMgr->addSphere(core::vector3df(30,0,30), 2, blue, 2000);
}
DebugDrawScene::~DebugDrawScene()
{
}

void DebugDrawScene::update(int ms)
{
	std::wstring msg(L"hello world");
	
	g_debugDrawMgr->addSphere(core::vector3df(0,0,30), 10, red);
	g_debugDrawMgr->addLine(vector3df(10, 0, 30), vector3df(10, 200, 200), green, 1.0f);
	g_debugDrawMgr->addLine(vector3df(10, 0, 30), vector3df(10, 200, 300), red, 5.0f);

	matrix4 axisMat = core::matrix4();
	axisMat.setTranslation(vector3df(-20, 0, 30));
	axisMat.setRotationDegrees(vector3df(0, 0, 45));

	g_debugDrawMgr->addAxis(axisMat, 5);
	g_debugDrawMgr->addCross(vector3df(-10, 0, 30), blue, 5);
	g_debugDrawMgr->addString(vector3df(0, 0, 30), msg, blue, 2.0f);

	g_debugDrawMgr->addLine(vector2di(0, 0), vector2di(50, 100), blue, 5.0f);
	g_debugDrawMgr->addCross(vector2di(50, 100), red, 5.0f);
	g_debugDrawMgr->addCircle(vector2di(50, 100), 10.0f, green);
	g_debugDrawMgr->addString(vector2di(50, 100), msg, white);
}
